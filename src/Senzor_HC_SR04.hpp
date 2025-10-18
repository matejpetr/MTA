#pragma once

#include <Arduino.h>
#include <vector>
#include "Sensor.hpp"

class HCSR04 : public Sensor {
public:
  HCSR04(int trig, int echo)
    : _trig(trig), _echo(echo), _limit(150), _delayMs(40) {} // default: 150 cm, 40 ms


    void attach(const std::vector<int>& pins) override {
      if (pins.size() >= 1) _trig = pins[0];
      if (pins.size() >= 2) _echo = pins[1];
    }
    void detach() override {
      if (_trig >= 0) pinMode(_trig, INPUT);
      if (_echo >= 0) pinMode(_echo, INPUT);
    }

  bool            init()   override;                 // rychlý sanity check
  void            reset()  override { /* no-op */ }
  std::vector<KV> update() override;                 // {"distance", ...}
  const char*     getType() override { return "HCSR04"; }

  // Config v HPP: jen uloží členy (bez HW volání)
  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if      (params[i].key == "Limit") _limit   = params[i].value.toInt(); // max cm
      else if (params[i].key == "Delay") _delayMs = params[i].value.toInt(); // ms mezi pings
    }
  }

private:
  int _trig, _echo;
  int _limit;     // cm
  int _delayMs;   // ms
};
