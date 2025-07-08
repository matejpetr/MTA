#pragma once

#include <Arduino.h>
#include "actuator.hpp"

void RGB_config(int pinR, int pinG, int pinB, int BrigR, int BrigG, int BrigB);
void RGB_reset();

class RGB : public Actuator {
public:
  RGB(int pinR, int pinG, int pinB, int BrigR, int BrigG, int BrigB)
    : _pinR(pinR), _pinG(pinG), _pinB(pinB),
      _BrigR(BrigR), _BrigG(BrigG), _BrigB(BrigB) {}

  void config() override {RGB_config(_pinR, _pinG, _pinB, _BrigR, _BrigG, _BrigB);}
  void reset() override {RGB_reset();}

private:
  int _pinR;
  int _pinG;
  int _pinB;
  int _BrigR;
  int _BrigG;
  int _BrigB;
};
