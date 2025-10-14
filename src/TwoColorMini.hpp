#pragma once
#include <Arduino.h>
#include "actuator.hpp"


void TwoColorMini_config(int pinRed, int pinGreen, char color, int Brightness);
void TwoColorMini_reset();

class TwoColorMini : public Actuator {
public:
  TwoColorMini(int pinRed, int pinGreen, char color, int Brightness)
    : _pinRed(pinRed), _pinGreen(pinGreen), _color(color), _Brightness(Brightness) {}

  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if (params[i].key == "pinRed") _pinRed = params[i].value.toInt();
      else if (params[i].key == "pinGreen") _pinGreen = params[i].value.toInt();
      else if (params[i].key == "color") _color = params[i].value.charAt(0);
      else if (params[i].key == "Brig") _Brightness = params[i].value.toInt();
    }
    TwoColorMini_config(_pinRed, _pinGreen, _color, _Brightness);
  }

  void reset() override { TwoColorMini_reset(); }

private:
  int _pinRed;
  int _pinGreen;
  char _color;
  int _Brightness;
};
  