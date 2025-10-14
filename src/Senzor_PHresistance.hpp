#pragma once

#include <Arduino.h>
#include <vector>
#include "Sensor.hpp"


class PHresistance : public Sensor {
public:
  explicit PHresistance(int pin)
    : _pin(pin), _res(12), _gain(1.0f) {
    analogReadResolution(_res);
  }

  bool            init()   override;                 // stejné jako dřív (>1000)
  void            reset()  override { /* no-op */ }
  std::vector<KV> update() override;                 // {"intensity", ...}
  const char*     getType() override { return "PHresistance"; }

  // Config v HPP: jen uloží Res + Gain
  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if      (params[i].key == "Res")  _res  = params[i].value.toInt();
      else if (params[i].key == "Gain") _gain = params[i].value.toFloat();
    }
    analogReadResolution(_res);
  }

private:
  int   _pin;
  int   _res;    // ADC bity
  float _gain;   // násobek na výstupu (kalibrace)
};
