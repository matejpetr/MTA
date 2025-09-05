#pragma once

#include <Arduino.h>
#include "Sensor.hpp"
#include "Parser.hpp"

void GP2Y0A21YK0F_update(int pin, int unit, float lAlarm, float hAlarm);
bool GP2Y0A21YK0F_init(int pin);
void GP2Y0A21YK0F_reset();

class GP2Y0A21YK0F : public Sensor {
public:
  GP2Y0A21YK0F(int pin)
    : _pin(pin), _unit(0), _lAlarm(22.0), _hAlarm(78.0) {}

  void update() override {
    GP2Y0A21YK0F_update(_pin, _unit, _lAlarm, _hAlarm);
  }

  void reset() override { GP2Y0A21YK0F_reset(); }
  bool init() override { return GP2Y0A21YK0F_init(_pin); }
  const char* getType() override { return "GP2Y0A21YK0F"; }

  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if (params[i].key == "Unit") _unit = params[i].value.toInt(); // 0 = cm, 1 = mm
      else if (params[i].key == "LowAlarm") _lAlarm = params[i].value.toFloat();
      else if (params[i].key == "HighAlarm") _hAlarm = params[i].value.toFloat();
    }
  }

private:
  int _pin;
  int _unit;
  float _lAlarm;
  float _hAlarm;
};
