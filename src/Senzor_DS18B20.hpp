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

 
  bool init() override;
  void reset() override;
  std::vector<KV> update() override;
  const char* getType() override { return "DS18B20"; }

  // KOnfigurační parametry
  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      String k = params[i].key;
      k.trim();
      k.toLowerCase();
      if      (k == "res")    _res    = params[i].value.toInt();      // 9..12
      else if (k == "lalarm") _LAlarm = params[i].value.toInt();      // -55..125
      else if (k == "halarm") _HAlarm = params[i].value.toInt();      // -55..125
    }
    applyConfig();   
  }

  // PIN se bere pouze z attach()
  void attach(const std::vector<int>& pins) override {
    if (!pins.empty()) {
      int newPin = pins[0];
      if (newPin != _busPin) {
        _busPin = newPin;
      }
      if (_busPin >= 0) {
        pinMode(_busPin, INPUT_PULLUP);
        if (_sensors) {
          _sensors->begin();
          applyConfig();;
        }
        _attached = true;
      }
    }
  }

  // detach: bezpečné uvolnění pinu a přerušení čtení
  void detach() override {
    if (_attached) {
      if (_busPin >= 0) {
        pinMode(_busPin, INPUT);
      }
      _attached = false;
    }
  }

private:
  void applyConfig();  

  DallasTemperature* _sensors;  
  int  _res;                     // 9..12
  int  _LAlarm;                  // -55..125
  int  _HAlarm;                  // -55..125
  int  _busPin;
  bool _attached;
};
