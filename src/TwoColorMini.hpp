#pragma once
#include <Arduino.h>
#include <vector>
#include "actuator.hpp"

void TwoColorMini_setPins(int pinRed, int pinGreen);
void TwoColorMini_config(char color, int Brightness);
void TwoColorMini_reset();

class TwoColorMini : public Actuator {
public:
  TwoColorMini(int pinRed = -1, int pinGreen = -1, char color = 'r', int Brightness = 0)
    : _pinRed(pinRed), _pinGreen(pinGreen), _color(color), _Brightness(Brightness) {}

  void attach(const std::vector<int>& pins) override {
    if (pins.size() >= 1) _pinRed   = pins[0];
    if (pins.size() >= 2) _pinGreen = pins[1];
    // nastavit globální driver piny — config už pak použije tyto piny
    TwoColorMini_setPins(_pinRed, _pinGreen);
  }

  void detach() override {
    // bezpečně vypnout a uvolnit piny pokud jsou validní
    TwoColorMini_reset();
    if (_pinRed >= 0) { digitalWrite(_pinRed, LOW); pinMode(_pinRed, INPUT); _pinRed = -1; }
    if (_pinGreen >= 0) { digitalWrite(_pinGreen, LOW); pinMode(_pinGreen, INPUT); _pinGreen = -1; }
  }

  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if (params[i].key == "color") _color = params[i].value.charAt(0);
      else if (params[i].key == "Brig") _Brightness = params[i].value.toInt();
    }
    TwoColorMini_config(_color, _Brightness);
  }

  void reset() override { TwoColorMini_reset(); }

private:
  int _pinRed   = -1;
  int _pinGreen = -1;
  char _color   = 'r';
  int  _Brightness = 0;
};
