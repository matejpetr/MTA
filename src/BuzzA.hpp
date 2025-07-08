#pragma once
#include <Arduino.h>
#include "actuator.hpp"
#include "Parser.hpp"

void BuzzA_config(int pin, bool control);
void BuzzA_reset(int pin);

class BuzzA : public Actuator {
public:
  BuzzA(int pin, bool control)
    : _pin(pin), _control(control) {}

  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if (params[i].key == "pin") _pin = params[i].value.toInt();
      else if (params[i].key == "control") _control = params[i].value == "true";
    }
    BuzzA_config(_pin, _control);
  }

  void reset() override { BuzzA_reset(_pin); }

private:
  int _pin;
  bool _control;
};
