#pragma once

#include <Arduino.h>
#include <vector>
#include "Sensor.hpp"


class Ahall : public Sensor {
public:
  Ahall(int pin)
    : _pin(pin), _res(12), _lLimit(1800), _hLimit(2000), _val(0) {
      analogReadResolution(_res);
    }

  // API senzoru
  std::vector<KV> update() override;      // vrací {"val", ...}, {"polarity", ...}
  void            reset()  override { _val = 0; }
  bool            init()   override { return true; }   // nesběrnicový
  const char*     getType() override { return "Ahall"; }

  // Konfigurace: jen rozlišení a limity
  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if      (params[i].key == "Res")     _res    = params[i].value.toInt();
      else if (params[i].key == "LLimit")  _lLimit = params[i].value.toInt();
      else if (params[i].key == "HLimit")  _hLimit = params[i].value.toInt();
    }
    analogReadResolution(_res);
  }

private:
  int _pin;
  int _res;
  int _lLimit;
  int _hLimit;
  int _val;        // poslední hodnota pro reset
};
