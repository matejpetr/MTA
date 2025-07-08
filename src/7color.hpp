#pragma once
#include <Arduino.h>
#include "actuator.hpp"
#include "Parser.hpp"

void Color7_config(int pin, bool control);
void Color7_reset(int pin);

class Color7 : public Actuator {
public:
  Color7(int pin, bool control)
    : _pin(pin), _control(control) {}

  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if (params[i].key == "pin") _pin = params[i].value.toInt();
      else if (params[i].key == "control") _control = params[i].value == "true";
    }
    Color7_config(_pin, _control);
  }

  void reset() override { Color7_reset(_pin); }

private:
  int _pin;
  bool _control;
};
