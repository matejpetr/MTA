#pragma once

#include <Arduino.h>
#include "actuator.hpp"

void BuzzP_config(int pin, int freq, int duration);
void BuzzP_reset(int pin);

class BuzzP : public Actuator {
public:
  BuzzP(int pin, int freq, int duration)
    : _pin(pin), _freq(freq), _duration(duration) {}

  void config() override {BuzzP_config(_pin, _freq, _duration);}
  void reset() override {BuzzP_reset(_pin);}

private:
  int _pin;
  int _freq;
  int _duration;
};
