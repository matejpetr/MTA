#pragma once
#include <Arduino.h>
#include "actuator.hpp"


void BuzzP_config(int pin, int freq, int duration);
void BuzzP_reset(int pin);

class BuzzP : public Actuator {
public:
  BuzzP(int pin, int freq, int duration)
    : _pin(pin), _freq(freq), _duration(duration) {}

  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if (params[i].key == "pin") _pin = params[i].value.toInt();
      else if (params[i].key == "freq") _freq = params[i].value.toInt();
      else if (params[i].key == "duration") _duration = params[i].value.toInt();
    }
    BuzzP_config(_pin, _freq, _duration);
  }

  void reset() override { BuzzP_reset(_pin); }

private:
  int _pin;
  int _freq;
  int _duration;
};
