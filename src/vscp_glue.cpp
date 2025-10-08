#include <Arduino.h>
#include "vscp_device.hpp"
#include "Sensor.hpp"
#include "Parser.hpp"     // Param{key,value}
#include <vector>
#include <map>

#include <OneWire.h>
#include <DallasTemperature.h>
#include <math.h>

using std::vector;

extern Sensor* SeznamSenzoru[];
extern int PocetSenzoru;
extern bool ResponseAll;
extern String globalBuffer;

VSCPDevice vscp;

// --- DS18B20 (nastavíme pin přes CONNECT a hlídáme alarmy přes CONFIG) ---
static int ds_pin = -1;
static OneWire* ds_ow = nullptr;
static DallasTemperature* ds_dt = nullptr;
static float ds_low_alarm  = -9999.0f;
static float ds_high_alarm =  9999.0f;

static void ds_setup(int pin) {
  if (ds_ow) { delete ds_ow; ds_ow = nullptr; }
  if (ds_dt) { delete ds_dt; ds_dt = nullptr; }
  ds_pin = pin;
  ds_ow = new OneWire(ds_pin);
  ds_dt = new DallasTemperature(ds_ow);
  ds_dt->begin();
  ds_dt->setWaitForConversion(true); // blokuje do dokončení konverze
  ds_dt->setResolution(12);
}

// --- helpers ---
static String firstLine(const String& s) {
  int r = s.indexOf('\r');
  int n = s.indexOf('\n');
  int cut;
  if (r < 0) cut = n; else if (n < 0) cut = r; else cut = (r < n ? r : n);
  return (cut < 0) ? s : s.substring(0, cut);
}

static std::map<String,String> parseQuery(const String& q) {
  std::map<String,String> kv;
  int i = (q.length() && q[0]=='?') ? 1 : 0;
  while (i < q.length()) {
    int amp = q.indexOf('&', i); if (amp < 0) amp = q.length();
    int eq  = q.indexOf('=', i);
    if (eq > i && eq < amp) {
      String k = q.substring(i, eq);
      String v = q.substring(eq+1, amp);
      kv[k] = v;
    }
    i = amp + 1;
  }
  return kv;
}

static vector<KV> legacyToKVs(const String& legacyLine) {
  vector<KV> out;
  auto m = parseQuery(firstLine(legacyLine));
  for (const auto &p : m) {
    if (p.first == "type" || p.first == "id") continue;
    KV item; item.k = p.first; item.v = p.second;
    out.push_back(item);
  }
  return out;
}

static vector<uint8_t> g_inited;

static vector<KV> collectFromSensor(int idx) {
  if (idx < 0) return {};

  if ((int)g_inited.size() <= idx) g_inited.resize(idx+1, 0);
  if (!g_inited[idx]) {
    SeznamSenzoru[idx]->init();
    g_inited[idx] = 1;
    delay(5);
  }

  ResponseAll = true;
  globalBuffer = "";

  SeznamSenzoru[idx]->update();
  if (globalBuffer.length() == 0) {
    delay(50);
    SeznamSenzoru[idx]->update();
  }
  if (globalBuffer.length() == 0) {
    delay(150);
    SeznamSenzoru[idx]->update();
  }

  if (globalBuffer.length() == 0) return {};
  return legacyToKVs(globalBuffer);
}

extern "C" void VSCP_SetupRegisterAll() {
  g_inited.assign(PocetSenzoru, 0);
  for (int i = 0; i < PocetSenzoru; ++i) {
    const int idx = i;
    vscp.onUpdate(String(idx), [idx](const String& /*id*/, int /*pin*/)->vector<KV> {
      return collectFromSensor(idx);
    });
  }

  // DS18B20: override pro id="0" – čteme ds_dt (pin z CONNECT) + alarmy
  vscp.onUpdate(String("0"), [](const String& /*id*/, int /*pin*/)->std::vector<KV> {
    std::vector<KV> out;
    if (!ds_dt) return out;       // není CONNECT

    ds_dt->requestTemperatures();
    float t = ds_dt->getTempCByIndex(0);

    if (t <= -126.0f || fabsf(t - 85.0f) < 0.01f) return out;

    KV a; a.k="temp";  a.v = String(t,1); out.push_back(a);
    KV b; b.k="alarm";
    if (t < ds_low_alarm)      b.v = "LOW";
    else if (t > ds_high_alarm) b.v = "HIGH";
    else                        b.v = "OK";
    out.push_back(b);
    return out;
  });
}

extern "C" void VSCP_Poll() {
  vscp.poll();
}

// CONNECT hook (init senzoru + DS bus)
extern "C" void VSCP_OnConnect(const String& id, int pin) {
  int idx = id.toInt();
  if (idx >= 0 && idx < PocetSenzoru) {
    if ((int)g_inited.size() <= idx) g_inited.resize(idx+1, 0);
    if (!g_inited[idx]) { SeznamSenzoru[idx]->init(); g_inited[idx] = 1; }
  }
  if (id == "0" && pin >= 0) {
    ds_setup(pin);
  }
}

// CONFIG hook — převede parametry na Param{key,value} a zavolá sensor->config,
// zároveň obslouží DS18B20 alarm limity (LowAlarm/HighAlarm). Vrací true pokud něco použil.
bool VSCP_OnConfig(const String& id, const std::map<String,String>& params) {
  bool used = false;

  // DS18B20 alarm limity
  if (id == "0") {
    auto itL = params.find("LowAlarm");
    if (itL != params.end()) { ds_low_alarm = String(itL->second.c_str()).toFloat(); used = true; }
    auto itH = params.find("HighAlarm");
    if (itH != params.end()) { ds_high_alarm = String(itH->second.c_str()).toFloat(); used = true; }
  }

  // Předat i do obecného sensor->config (pokud existují nějaké klíče)
  int idx = id.toInt();
  if (idx >= 0 && idx < PocetSenzoru) {
    std::vector<Param> pvec;
    pvec.reserve(params.size());
    for (const auto& kv : params) {
      if (kv.first == "type" || kv.first == "id" || kv.first == "pin" || kv.first == "api") continue;
      Param p; p.key = kv.first.c_str(); p.value = kv.second.c_str();
      pvec.push_back(p);
    }
    if (!pvec.empty()) {
      SeznamSenzoru[idx]->config(pvec.data(), (int)pvec.size());
      used = true;
    }
  }

  return used;
}
