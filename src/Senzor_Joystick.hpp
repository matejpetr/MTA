#pragma once

#include <Arduino.h>
#include "Sensor.hpp"
#include "Parser.hpp"

void Joystick_update(int x, int y, int sw, int res, int threshold);
bool Joystick_init(int x, int y);
void Joystick_reset();

class Joystick : public Sensor {
public:
  Joystick(int x, int y, int sw)
    : _x(x), _y(y), _sw(sw), _res(12), _threshold(25) {
      analogReadResolution(_res);
    }

  void update() override {Joystick_update(_x, _y, _sw, _res, _threshold);}
  void reset() override { Joystick_reset(); }
  bool init() override { return Joystick_init(_x, _y); }
  const char* getType() override { return "Joystick"; }

  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if (params[i].key == "Res") _res = params[i].value.toInt();
      else if (params[i].key == "Threshold") _threshold = params[i].value.toInt();
    }
    analogReadResolution(_res);
  }

private:
  int _x, _y, _sw;
  int _res;
  int _threshold;  // v procentech (0–100)
};
