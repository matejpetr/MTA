#pragma once

#include <Arduino.h>
#include "Sensor.hpp"
#include "Parser.hpp"

void MicSmall_update(int pin, int MT, int res);
bool MicSmall_init(int pin);
void MicSmall_reset();

class MicSmall : public Sensor {
public:
  MicSmall(int pin, int time)
    : _pin(pin), _time(time), _res(12) {
    analogReadResolution(_res);
  }

  void update() override {MicSmall_update(_pin, _time, _res);}
  void reset() override { MicSmall_reset(); }
  bool init() override { return MicSmall_init(_pin); }
  const char* getType() override { return "MicSmall"; }

  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if (params[i].key == "Res") _res = params[i].value.toInt();
      else if (params[i].key == "MT") _time = params[i].value.toInt();
    }
    analogReadResolution(_res);
  }

private:
  int _pin;
  int _time;
  int _res;
};
