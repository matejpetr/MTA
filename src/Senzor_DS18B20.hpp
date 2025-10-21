#pragma once

#include <Arduino.h>
#include <vector>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Sensor.hpp"


class DS18B20 : public Sensor {
public:
  explicit DS18B20(DallasTemperature* sensors)
    : _sensors(sensors), _res(12), _LAlarm(-55), _HAlarm(125),
      _busPin(-1), _attached(false) {}

  // Sběrnicový senzor: jednoduché ověření přítomnosti zařízení
  bool init() override;
  void reset() override;

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

  // PIN se bere pouze z attach()
  void attach(const std::vector<int>& pins) override {
    if (!pins.empty()) {
      int newPin = pins[0];
      if (newPin != _busPin) {
        _busPin = newPin;
      }
      if (_busPin >= 0) {
        // připravit pin pro 1-wire sběrnici
        pinMode(_busPin, INPUT_PULLUP);
        // pokud máme instanci DallasTemperature, inicializuj sběrnici
        if (_sensors) {
          _sensors->begin();
          applyConfig();
        }
        _attached = true;
      }
    }
  }

  // detach: bezpečné uvolnění pinu a přerušení čtení
  void detach() override {
    if (_attached) {
      // není bezpečné mazat _sensors (neowned), jen uvolnit pin
      if (_busPin >= 0) {
        pinMode(_busPin, INPUT);
      }
      _attached = false;
      // necháme _busPin hodnotu zachovanou nebo nastavit -1 dle potřeby:
      // _busPin = -1;
    }
  }

private:
  void applyConfig();  // definice v .cpp

  DallasTemperature* _sensors;   // neowned
  int  _res;                     // 9..12
  int  _LAlarm;                  // -55..125
  int  _HAlarm;                  // -55..125

  // attach/detach state
  int  _busPin;
  bool _attached;
};
