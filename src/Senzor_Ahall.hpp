#pragma once

#include <Arduino.h>
#include "Sensor.hpp"

void Sensor_Ahall(int pin);


class SensorAhall : public Sensor {
public:
  SensorAhall(int pin) :_pin(pin) {}

  
  void update() override {
    Sensor_Ahall(_pin);
  }

   void reset() override {
    
  }

   void init() override {
    
  }

private:
  int _pin;
};
