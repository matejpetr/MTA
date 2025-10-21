#pragma once
#include <Arduino.h>
#include "actuator.hpp"

void RGB_setPins(int pinR, int pinG, int pinB);
void RGB_config(int BrigR, int BrigG, int BrigB);
void RGB_reset();

class RGB : public Actuator {
public:
  RGB(int pinR = -1, int pinG = -1, int pinB = -1, int BrigR = 0, int BrigG = 0, int BrigB = 0)
    : _pinR(pinR), _pinG(pinG), _pinB(pinB),
      _BrigR(BrigR), _BrigG(BrigG), _BrigB(BrigB) {}

  // pins are taken only from attach()
  void attach(const std::vector<int>& pins) override {
    if (pins.size() >= 1) _pinR = pins[0];
    if (pins.size() >= 2) _pinG = pins[1];
    if (pins.size() >= 3) _pinB = pins[2];
    RGB_setPins(_pinR, _pinG, _pinB);
  }

  void detach() override {
    // safe shutdown and release pins
    RGB_reset();
    if (_pinR >= 0) { digitalWrite(_pinR, LOW); pinMode(_pinR, INPUT); _pinR = -1; }
    if (_pinG >= 0) { digitalWrite(_pinG, LOW); pinMode(_pinG, INPUT); _pinG = -1; }
    if (_pinB >= 0) { digitalWrite(_pinB, LOW); pinMode(_pinB, INPUT); _pinB = -1; }
    RGB_setPins(-1, -1, -1);
  }

  // config only adjusts brightness values (angle/speed style kept minimal)
  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if (params[i].key == "BrigR") _BrigR = params[i].value.toInt();
      else if (params[i].key == "BrigG") _BrigG = params[i].value.toInt();
      else if (params[i].key == "BrigB") _BrigB = params[i].value.toInt();
    }
    RGB_config(_BrigR, _BrigG, _BrigB);
  }

  void reset() override { RGB_reset(); }

private:
  int _pinR = -1;
  int _pinG = -1;
  int _pinB = -1;
  int _BrigR = 0;
  int _BrigG = 0;
  int _BrigB = 0;
};
