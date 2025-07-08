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

  void reset() override {Stepper_reset();}
  void config() override {Stepper_config(_pin1, _pin2, _pin3, _pin4, _angle, _dir, _rpm); }

private:
  int _pin1, _pin2, _pin3, _pin4;
  int _angle;
  bool _dir;
  int _rpm;
};
