#pragma once
#include <Arduino.h>
#include "actuator.hpp"

void Laser_setPin(int pin);
void Laser_config(bool control);
void Laser_reset();

class Laser : public Actuator {
public:
  Laser(int pin = -1, bool control = false)
    : _pin(pin), _control(control) {}

  void attach(const std::vector<int>& pins) override {
    if (pins.size() >= 1) {
      _pin = pins[0];
      Laser_setPin(_pin);
    }
  }

  void detach() override {
    // bezpečně vypnout a uvolnit pin
    Laser_reset();
    if (_pin >= 0) {
      digitalWrite(_pin, LOW);
      pinMode(_pin, INPUT);
      _pin = -1;
      Laser_setPin(-1);
    }
  }

  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if (params[i].key == "control") _control = params[i].value == "true";
    }
    // používá pin nastavený v attach()
    Laser_config(_control);
  }

  void reset() override { Laser_reset(); }

private:
  int _pin;
  bool _control;
};
