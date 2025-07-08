#pragma once

#include <Arduino.h>
#include "actuator.hpp"

void BuzzA_config(int pin, bool control);
void BuzzA_reset(int pin);

class BuzzA : public Actuator {
public:
  BuzzA(int pin, bool control)
    : _pin(pin), _control(control) {}

  void config() override {BuzzA_config(_pin, _control);}
  void reset() override {BuzzA_reset(_pin);}

private:
  int _pin;
  bool _control;
};
