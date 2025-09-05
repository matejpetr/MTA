#pragma once

#include <Arduino.h>
#include "Sensor.hpp"
#include "Parser.hpp"

void MicBig_update(int pin, int MT, int res);
bool MicBig_init(int pin);
void MicBig_reset();

class MicBig : public Sensor {
public:
  MicBig(int pin, int time)
    : _pin(pin), _time(time), _res(12) {
      analogReadResolution(_res);
    }

  void update() override {MicBig_update(_pin, _time, _res);}
  void reset() override { MicBig_reset(); }
  bool init() override { return MicBig_init(_pin); }
  const char* getType() override { return "MicBig"; }

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
