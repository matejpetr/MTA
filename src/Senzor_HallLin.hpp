#pragma once

#include <Arduino.h>
#include "Sensor.hpp"
#include "Parser.hpp"

void HallLin_update(int pin, int res, String unit);
bool HallLin_init(int pin);
void HallLin_reset();

class HallLin : public Sensor {
public:
  HallLin(int pin)
    : _pin(pin), _res(12), _unit("Voltage") {
      analogReadResolution(_res);
    }

  void update() override {HallLin_update(_pin, _res, _unit);}
  void reset() override { HallLin_reset(); }
  bool init() override { return HallLin_init(_pin); }
  const char* getType() override { return "HallLin"; }

  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if (params[i].key == "Res") _res = params[i].value.toInt();
      else if (params[i].key == "Unit") _unit = params[i].value;
    }
    analogReadResolution(_res);
  }

private:
  int _pin;
  int _res;
  String _unit;
};
