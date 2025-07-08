#pragma once

#include <Arduino.h>
#include "actuator.hpp"

void TwoColor_config(int pinRed, int pinGreen, char color, int Brightness);
void TwoColor_reset();

class TwoColor : public Actuator {
public:
  TwoColor(int pinRed, int pinGreen, char color, int Brightness)
    : _pinRed(pinRed), _pinGreen(pinGreen), _color(color), _Brightness(Brightness) {}

  void config() override {
    TwoColor_config(_pinRed, _pinGreen, _color, _Brightness);
  }

  void reset() override {
    TwoColor_reset();
  }

private:
  int _pinRed;
  int _pinGreen;
  char _color;             

  int _Brightness;  
};
