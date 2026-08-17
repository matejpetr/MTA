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


// externí proměnné
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


// Parsuje jednotné ID ve formátu Sxx, Axx, číslo nebo wildcard a vrací rozložené pole
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


// Inicializuje DS18B20 sběrnici na daném pinu a vytvoří OneWire a DallasTemperature instance
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


static vector<uint8_t> g_inited;


// Zavolá init pro senzor pokud ještě není inicializovaný a nasbírá hodnoty přes update
static std::vector<KV> collectFromSensor(int idx) {
  if (idx < 0) return {};
  if ((int)g_inited.size() <= idx) g_inited.resize(idx+1, 0);
  if (!g_inited[idx]) { SeznamSenzoru[idx]->init(); g_inited[idx] = 1; delay(5); }

  auto kv = SeznamSenzoru[idx]->update();
  if (kv.empty()) { delay(50);  kv = SeznamSenzoru[idx]->update(); }
  if (kv.empty()) { delay(150); kv = SeznamSenzoru[idx]->update(); }
  return kv; 
}


// Registruje všechny senzory do VSCP zařízení s onUpdate handlery
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


// Poll funkce volaná periodicky z hlavního loopu
extern "C" void VSCP_Poll() {
  vscp.poll();
}


// CONNECT hook pro připojení senzorů a aktuátorů, zavolá attach a init pro příslušné instance
void VSCP_OnConnect(const String& id, const std::vector<int>& pins) {
  ParsedId pid = parseUnifiedId(id, 'S');

  if (pid.group == 'S') {
    int idx = pid.index;
    if (idx >= 0 && idx < PocetSenzoru) {
      if ((int)g_attachedS.size() <= idx) g_attachedS.resize(idx+1, 0);

      // Uvolnění připojených pinů
      if (g_attachedS[idx]) SeznamSenzoru[idx]->detach();

      // Připojení nových pinů
      SeznamSenzoru[idx]->attach(pins);

      // Inicializace po attach
      SeznamSenzoru[idx]->init();

      g_attachedS[idx] = 1;

      // speciál pro DS18B20
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


// Overload CONNECT s jedním pinem, převede na vector a zavolá hlavní hook
extern "C" void VSCP_OnConnect(const String& id, int pin) {
  std::vector<int> v; 
  if (pin >= 0) v.push_back(pin);
  VSCP_OnConnect(id, v);
}


// DISCONNECT hook pro uvolnění HW a reset interních stavů
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


// CONFIG hook pro předání parametrů senzorům a aktuátorům, včetně DS18B20 alarmů
bool VSCP_OnConfig(const String& id, const std::map<String,String>& params) {
  bool used = false;

  ParsedId pid = parseUnifiedId(id, 'S');

  // ==== senzory ====
  if (pid.group == 'S') {
    // DS18B20 alarmy pro S00
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
    auto applyOne = [&](int idx){
      if (idx < 0 || idx >= PocetAktuatoru) return false;
      std::vector<Param> pvec;
      pvec.reserve(params.size());
      for (const auto& kv : params) {
        if (kv.first == "type" || kv.first == "id" || kv.first == "pin" || kv.first == "api") continue;
        Param p; p.key = kv.first.c_str(); p.value = kv.second.c_str();
        pvec.push_back(p);
      }
      if (!pvec.empty()) {
        SeznamAktuatoru[idx]->control(pvec.data(), (int)pvec.size());
        return true;
      }
      return false;
    };

    if (pid.index == -1) {
      // wildcard A* — aplikuj na všechny aktuátory
      bool any = false;
      for (int i = 0; i < PocetAktuatoru; ++i) any |= applyOne(i);
      return any;
    } else {
      return applyOne(pid.index);
    }
  }

  return used;
}
