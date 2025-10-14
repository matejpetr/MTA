#pragma once

#include <Arduino.h>
#include <vector>
#include "Sensor.hpp"


class MicSmall : public Sensor {
public:
  MicSmall(int pin, int time_ms)
    : _pin(pin), _time(time_ms), _res(12) {
    analogReadResolution(_res);
  }

  bool            init()   override;                 // rychlý sanity check (původní logika)
  void            reset()  override { /* no-op */ }
  std::vector<KV> update() override;                 // {"volume", ...}
  const char*     getType() override { return "MicSmall"; }

  // Config: jen nastaví parametry (stejně jako dřív: Res, MT)
  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if      (params[i].key == "Res") _res  = params[i].value.toInt();
      else if (params[i].key == "MT")  _time = params[i].value.toInt();   // ms
    }
    analogReadResolution(_res);
  }

private:
  int _pin;
  int _time;   // délka měření v ms
  int _res;    // ADC rozlišení v bitech

  static constexpr int REF_BASE = 50;   // původní refS
};
