#pragma once

#include <Arduino.h>
#include <vector>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Sensor.hpp"


class DS18B20 : public Sensor {
public:
  explicit DS18B20(DallasTemperature* sensors)
    : _sensors(sensors), _res(12), _LAlarm(-55), _HAlarm(125) {}

  // Sběrnicový senzor: jednoduché ověření přítomnosti zařízení
  bool init() override {
    if (!_sensors) return false;
    return _sensors->getDeviceCount() > 0;
  }

  void reset() override {
    if (_sensors) _sensors->begin();
  }

  // Měření — vrací {"temp", ...}, {"alarm", "LOW|OK|HIGH"}
  std::vector<KV> update() override;

  const char* getType() override { return "DS18B20"; }

  // CONFIG zůstává v HPP: nastaví členy a zavolá vlastní metodu applyConfig()
  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if      (params[i].key == "Res")    _res    = params[i].value.toInt();      // 9..12
      else if (params[i].key == "LAlarm") _LAlarm = params[i].value.toInt();      // -55..125
      else if (params[i].key == "HAlarm") _HAlarm = params[i].value.toInt();      // -55..125
    }
    applyConfig();   // <<< HW aplikace mimo update(), implementovaná v .cpp
  }

private:
  void applyConfig();  // definice v .cpp

  DallasTemperature* _sensors;   // neowned
  int  _res;                     // 9..12
  int  _LAlarm;                  // -55..125
  int  _HAlarm;                  // -55..125
};
