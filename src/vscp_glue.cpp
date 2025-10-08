#include <Arduino.h>
#include "vscp_device.hpp"
#include "Sensor.hpp"
#include <vector>
#include <map>

//*
#include <OneWire.h>
#include <DallasTemperature.h>

using std::vector;

extern Sensor* SeznamSenzoru[];
extern int PocetSenzoru;
extern bool ResponseAll;
extern String globalBuffer;

VSCPDevice vscp;

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
}

extern "C" void VSCP_Poll() {
  vscp.poll();
}

extern "C" void VSCP_OnConnect(const String& id, int /*pin*/) {
  int idx = id.toInt();
  if (idx >= 0 && idx < PocetSenzoru) {
    SeznamSenzoru[idx]->init();   // init při CONNECT
    if ((int)g_inited.size() <= idx) g_inited.resize(idx+1, 0);
    g_inited[idx] = 1;            // ať se už nikdy nespouští v UPDATE
  }
}