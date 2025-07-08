#pragma once

#include <Arduino.h>
#include "actuator.hpp"

void Color7_config(int pin, bool control);
void Color7_reset(int pin);

class Color7 : public Actuator {
public:
  Color7(int pin, bool control)
    : _pin(pin),  _control(control) {}

  void config() override {Color7_config(_pin, _control);}
  void reset() override {Color7_reset(_pin);}

private:
  int _pin;
  bool _control;
};
