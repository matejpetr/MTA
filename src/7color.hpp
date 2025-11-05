#pragma once
#include <Arduino.h>
#include "actuator.hpp"
#include <vector>

void Color7_config(int pin, bool control);
void Color7_reset(int pin);

class Color7 : public Actuator {
public:
  // konstruktor, pin lze předat nebo přiřadit přes attach
  Color7(int pin = -1, bool control = false)
    : _pin(pin), _control(control) {
    if (_pin >= 0) {
      pinMode(_pin, OUTPUT);
      Color7_config(_pin, _control);
    }
  }

  // Konfigurační parametry
  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if (params[i].key == "pin") _pin = params[i].value.toInt();
      else if (params[i].key == "control") _control = params[i].value == "true";
    }
    Color7_config(_pin, _control);
  }

  void reset() override { Color7_reset(_pin); }

  // přiřazení pinu přes attach, použije první pin a inicializuje zařízení
  void attach(const std::vector<int>& pins) override {
    if (!pins.empty()) {
      _pin = pins[0];
      if (_pin >= 0) {
        pinMode(_pin, OUTPUT);
        Color7_config(_pin, _control);
      }
    }
  }

  // uvolnění pinu a vypnutí kontroly
  void detach() override {
    if (_pin >= 0) {
      Color7_reset(_pin);
      pinMode(_pin, INPUT);
      _pin = -1;
    }
  }

private:
  int _pin;
  bool _control;
};
