#pragma once

#include <Arduino.h>
#include "Sensor.hpp"
#include "Parser.hpp"

void Antc_update(int pin, int filter,int res);
bool Antc_init(int pin);
void Antc_reset();

class Antc : public Sensor {
public:
  Antc(int pin)
    : _pin(pin), _res(12), _filter(0) {
      analogReadResolution(_res);
    }

  void update() override { Antc_update(_pin, _filter,_res); }
  void reset() override { Antc_reset(); }
  bool init() override { return Antc_init(_pin); }
  const char* getType() override { return "Antc"; }

  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if (params[i].key == "Res") _res = params[i].value.toInt();
      else if (params[i].key == "Filter") _filter = params[i].value.toInt();
    }
    analogReadResolution(_res);
  }

private:
  int _pin;
  int _res;
  int _filter;
};
