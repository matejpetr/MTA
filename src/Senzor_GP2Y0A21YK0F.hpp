#pragma once

#include <Arduino.h>
#include "Sensor.hpp"

void Sensor_GP2Y0A21YK0F(int pin);

class SensorGP2Y0A21YK0F : public Sensor {
public:
  SensorGP2Y0A21YK0F(int pin) :_pin(pin) {}

  
  void update() override {
    Sensor_GP2Y0A21YK0F(_pin);
  }

   void reset() override {
    
  }

   void init() override {
    
  }

private:
  int _pin;
};
