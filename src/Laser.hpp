#pragma once

#include <Arduino.h>
#include "actuator.hpp"

void Laser_config(int pin, bool control);
void Laser_reset(int pin);

class Laser : public Actuator {
public:
  Laser(int pin, bool control)
    : _pin(pin), _control(control) {}

  void config() override {Laser_config(_pin, _control);}
  void reset() override {Laser_reset(_pin);}

private:
  int _pin;
  int _control;
};
