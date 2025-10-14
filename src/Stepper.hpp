#pragma once
#include <Arduino.h>
#include "actuator.hpp"


void Stepper_config(int pin1, int pin2, int pin3, int pin4, int angle, bool dir, int rpm);
void Stepper_reset();

class Stepper : public Actuator {
public:
  Stepper(int pin1, int pin2, int pin3, int pin4, int angle, bool dir, int rpm)
    : _pin1(pin1), _pin2(pin2), _pin3(pin3), _pin4(pin4),
      _angle(angle), _dir(dir), _rpm(rpm) {}

  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if (params[i].key == "pin1") _pin1 = params[i].value.toInt();
      else if (params[i].key == "pin2") _pin2 = params[i].value.toInt();
      else if (params[i].key == "pin3") _pin3 = params[i].value.toInt();
      else if (params[i].key == "pin4") _pin4 = params[i].value.toInt();
      else if (params[i].key == "angle") _angle = params[i].value.toInt();
      else if (params[i].key == "dir") _dir = (params[i].value == "true");
      else if (params[i].key == "rpm") _rpm = params[i].value.toInt();
    }
    Stepper_config(_pin1, _pin2, _pin3, _pin4, _angle, _dir, _rpm);
  }

  void reset() override { Stepper_reset(); }

private:
  int _pin1, _pin2, _pin3, _pin4;
  int _angle;
  bool _dir;
  int _rpm;
};
