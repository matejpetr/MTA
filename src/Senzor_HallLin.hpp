#pragma once

#include <Arduino.h>
#include "Sensor.hpp"

void HallLin_update(int pin);
bool HallLin_init(int pin);
void HallLin_reset();


class HallLin : public Sensor {
public:
  HallLin(int pin) :_pin(pin) {}

  
  void update() override {HallLin_update(_pin);}
  void reset() override {HallLin_reset();}
  bool init() override {return HallLin_init(_pin);}
  const char* getType() override {return "HallLin";}
private:
  int _pin;
};
