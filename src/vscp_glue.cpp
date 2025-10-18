#include <Arduino.h>
#include "vscp_device.hpp"
#include "vscp_glue.hpp"
#include "Sensor.hpp"
#include <vector>
#include <map>
#include "Actuator.hpp"       


#include <OneWire.h>
#include <DallasTemperature.h>
#include <math.h>

using std::vector;

extern Sensor* SeznamSenzoru[];
extern int PocetSenzoru;
extern Actuator* SeznamAktuatoru[];   
extern int PocetAktuatoru;            

VSCPDevice vscp;

static std::vector<uint8_t> g_attachedS; // senzory
static std::vector<uint8_t> g_attachedA; // aktuátory

struct ParsedId {
  char group;   // 'S' nebo 'A'
  int  index;   // >=0 konkrétní prvek; -1 = wildcard '*'
};

static ParsedId parseUnifiedId(const String& raw, char defaultGroup = 'S') {
  ParsedId out{ defaultGroup, -2 };
  if (!raw.length()) return out;

  if ((raw[0] == 'S') || (raw[0] == 'A')) {
    out.group = raw[0];
    if (raw.length() >= 2 && raw[1] == '*') { out.index = -1; return out; }
    out.index = String(raw.c_str()+1).toInt();
  } else if (raw[0] == '*') {
    out.index = -1;
  } else {
    // čisté číslo – kvůli zpětné kompatibilitě beru jako Sxx
    out.group = defaultGroup;
    out.index = raw.toInt();
  }
  return out;
}

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


static vector<uint8_t> g_inited;

static std::vector<KV> collectFromSensor(int idx) {
  if (idx < 0) return {};
  if ((int)g_inited.size() <= idx) g_inited.resize(idx+1, 0);
  if (!g_inited[idx]) { SeznamSenzoru[idx]->init(); g_inited[idx] = 1; delay(5); }

  auto kv = SeznamSenzoru[idx]->update();
  if (kv.empty()) { delay(50);  kv = SeznamSenzoru[idx]->update(); }
  if (kv.empty()) { delay(150); kv = SeznamSenzoru[idx]->update(); }
  return kv; 
}

static int idToIndex_Sxx(const String& id) {
  if (id.length() != 3) return -1;
  if (id.charAt(0) != 'S') return -1;
  if (!isDigit(id.charAt(1)) || !isDigit(id.charAt(2))) return -1;
  return (id.charAt(1)-'0')*10 + (id.charAt(2)-'0');
}


extern "C" void VSCP_SetupRegisterAll() {
  g_inited.assign(PocetSenzoru, 0);
  for (int i = 0; i < PocetSenzoru; ++i) {
    const int idx = i;
    String sid = "S";
    if (idx < 10) sid += "0";
    sid += String(idx);
    vscp.onUpdate(sid, [idx](const String& /*id*/, int /*pin*/)->vector<KV> {
      return collectFromSensor(idx);
    });
  }

  // DS18B20: override pro id="0" – čteme ds_dt (pin z CONNECT) + alarmy
  // DS18B20: override už jen pro "S00"
  vscp.onUpdate(String("S00"), [](const String& /*id*/, int /*pin*/)->std::vector<KV> {
    std::vector<KV> out;
    if (!ds_dt) return out;   // není CONNECT

    ds_dt->requestTemperatures();
    float t = ds_dt->getTempCByIndex(0);
    if (t <= -126.0f || fabsf(t - 85.0f) < 0.01f) return out;

    KV a; a.k="temp"; a.v=String(t,1); out.push_back(a);
    KV b; b.k="alarm";
    if (t < ds_low_alarm)       b.v="LOW";
    else if (t > ds_high_alarm) b.v="HIGH";
    else                        b.v="OK";
    out.push_back(b);
    return out;
  });
}

extern "C" void VSCP_Poll() {
  vscp.poll();
}

// CONNECT hook (init senzoru + DS bus)
void VSCP_OnConnect(const String& id, const std::vector<int>& pins) {
  ParsedId pid = parseUnifiedId(id, 'S');

  if (pid.group == 'S') {
    int idx = pid.index;
    if (idx >= 0 && idx < PocetSenzoru) {
      if ((int)g_attachedS.size() <= idx) g_attachedS.resize(idx+1, 0);

      // když už něco bylo připojeno, nejdřív uvolni staré piny
      if (g_attachedS[idx]) SeznamSenzoru[idx]->detach();

      // 1) přivážeme nové piny
      SeznamSenzoru[idx]->attach(pins);

      // 2) až teď inicializace HW (idempotentně)
      SeznamSenzoru[idx]->init();

      g_attachedS[idx] = 1;

      // speciál pro DS18B20 (pokud používáš S00) – využij první pin
      if (pid.index == 0 && !pins.empty() && pins[0] >= 0) {
        ds_setup(pins[0]);
      }
    }
    return;
  }

  if (pid.group == 'A') {
    int idx = pid.index;
    if (idx >= 0 && idx < PocetAktuatoru) {
      if ((int)g_attachedA.size() <= idx) g_attachedA.resize(idx+1, 0);
      if (g_attachedA[idx]) SeznamAktuatoru[idx]->detach();

      SeznamAktuatoru[idx]->attach(pins); // 1) piny
      SeznamAktuatoru[idx]->init();       // 2) init po attach

      g_attachedA[idx] = 1;
    }
    return;
  }
}

extern "C" void VSCP_OnConnect(const String& id, int pin) {
  std::vector<int> v; 
  if (pin >= 0) v.push_back(pin);
  VSCP_OnConnect(id, v);
}

// NOVÉ: DISCONNECT → jen zavoláme detach, ať se HW uvolní
void VSCP_OnDisconnect(const String& id) {
  ParsedId pid = parseUnifiedId(id, 'S');
  if (pid.group == 'S') {
    int idx = pid.index;
    if (idx >= 0 && idx < PocetSenzoru) {
      SeznamSenzoru[idx]->detach();
      if ((int)g_attachedS.size() > idx) g_attachedS[idx] = 0;
    }
  } else if (pid.group == 'A') {
    int idx = pid.index;
    if (idx >= 0 && idx < PocetAktuatoru) {
      SeznamAktuatoru[idx]->detach();
      if ((int)g_attachedA.size() > idx) g_attachedA[idx] = 0;
    }
  }
}

// CONFIG hook — převede parametry na Param{key,value} a zavolá sensor->config,
// zároveň obslouží DS18B20 alarm limity (LowAlarm/HighAlarm). Vrací true pokud něco použil.
bool VSCP_OnConfig(const String& id, const std::map<String,String>& params) {
  bool used = false;

  ParsedId pid = parseUnifiedId(id, 'S');

  // ==== speciály + obecné předání senzorům ====
  if (pid.group == 'S') {
    // (Tvoje dosavadní DS18B20 LowAlarm/HighAlarm pro S00...)
    if (pid.index == 0) {
      auto itL = params.find("LowAlarm");
      if (itL != params.end()) { ds_low_alarm = String(itL->second.c_str()).toFloat(); used = true; }
      auto itH = params.find("HighAlarm");
      if (itH != params.end()) { ds_high_alarm = String(itH->second.c_str()).toFloat(); used = true; }
    }

    int idx = pid.index;
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

  // ==== aktuátory ====
  if (pid.group == 'A') {
    // wildcard A* dává smysl pro hromadné resetování / hromadné nastavení
    auto itReset = params.find("reset");
    bool doReset = (itReset != params.end()) &&
                   (itReset->second == "1" || itReset->second == "true");

    auto applyOne = [&](int idx){
      if (idx < 0 || idx >= PocetAktuatoru) return false;
      if (doReset) {
        SeznamAktuatoru[idx]->reset();
        return true;
      }
      std::vector<Param> pvec;
      pvec.reserve(params.size());
      for (const auto& kv : params) {
        if (kv.first == "type" || kv.first == "id" || kv.first == "pin" || kv.first == "api" || kv.first == "reset") continue;
        Param p; p.key = kv.first.c_str(); p.value = kv.second.c_str();
        pvec.push_back(p);
      }
      SeznamAktuatoru[idx]->config(pvec.data(), (int)pvec.size());
      return true;
    };

    if (pid.index == -1) {
      bool any = false;
      for (int i=0; i<PocetAktuatoru; ++i) any |= applyOne(i);
      return any;
    } else {
      return applyOne(pid.index);
    }
  }

  return used;
}
