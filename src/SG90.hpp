#pragma once
#include <Arduino.h>
#include "actuator.hpp"

void SG90_config(int pin, int angle, int speed);
void SG90_reset(int pin);

class SG90 : public Actuator {
public:
  SG90(int pin, int angle, int speed)
    : _pin(pin), _angle(angle), _speed(speed) {}

  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if (params[i].key == "pin") _pin = params[i].value.toInt();
      else if (params[i].key == "angle") _angle = params[i].value.toInt();
      else if (params[i].key == "speed") _speed = params[i].value.toInt();
    }
    SG90_config(_pin, _angle, _speed);
  }

  void reset() override { SG90_reset(_pin); }

private:
  int _pin;
  int _angle;
  int _speed;
};
