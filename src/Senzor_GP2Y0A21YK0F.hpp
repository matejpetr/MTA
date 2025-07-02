#pragma once

#include <Arduino.h>
#include "Sensor.hpp"

void GP2Y0A21YK0F_update(int pin);
bool GP2Y0A21YK0F_init(int pin);
void GP2Y0A21YK0F_reset();

class GP2Y0A21YK0F : public Sensor {
public:
  GP2Y0A21YK0F(int pin) :_pin(pin) {}

  
  void update() override {GP2Y0A21YK0F_update(_pin);}
  void reset() override {GP2Y0A21YK0F_reset();}
  bool init() override {return GP2Y0A21YK0F_init(_pin);}
  const char* getType() override {return "GP2Y0A21YK0F";}
private:
  int _pin;
};
