#pragma once
#include <Arduino.h>
#include <vector>
#include "Sensor.hpp"


class HallLin : public Sensor {
public:
  explicit HallLin(int pin)
    : _pin(pin), _res(12), _unit("Voltage") {
      analogReadResolution(_res);
    }

  // stejné API
  std::vector<KV> update() override;
  void            reset() override { /* no-op */ }
  bool            init()  override;                 // validace jako dřív
  const char*     getType() override { return "HallLin"; }

  // config jen nastaví členy (bez HW volání)
  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if      (params[i].key == "Res")  _res  = params[i].value.toInt();
      else if (params[i].key == "Unit") _unit = params[i].value;
    }
    analogReadResolution(_res);
  }

private:
  int    _pin;
  int    _res;
  String _unit;   // "ADC" | "Voltage" | "Induction"
};
