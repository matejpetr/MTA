#pragma once

#include <Arduino.h>
#include "actuator.hpp"

void TwoColorMini_config(int pinRed, int pinGreen, char color,  int Brightness);
void TwoColorMini_reset();

class TwoColorMini : public Actuator {
public:
  TwoColorMini(int pinRed, int pinGreen, char color, int Brightness)
    : _pinRed(pinRed), _pinGreen(pinGreen), _color(color), _Brightness(Brightness) {}

    
  void config() override {TwoColorMini_config(_pinRed, _pinGreen, _color, _Brightness);}
  void reset() override {TwoColorMini_reset();}

private:
  int _pinRed;
  int _pinGreen;
  char _color;             
  int _Brightness;  
};
