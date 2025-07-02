#pragma once

#include <Arduino.h>
#include "Sensor.hpp"

void PHresistance_update(int pin);
bool PHresistance_init(int pin);
void PHresistance_reset();

class PHresistance : public Sensor {
public:
  PHresistance(int pin)
    : _pin(pin) {}

  void update() override {PHresistance_update(_pin);}
  void reset() override {PHresistance_reset();}
  bool init() override {return PHresistance_init(_pin);}
  const char* getType() override {return "PHresistance";}

private:
  int _pin;
};
