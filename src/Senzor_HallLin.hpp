#pragma once

#include <Arduino.h>
#include "Sensor.hpp"

void Sensor_HallLin(int pin);


class SensorHallLin : public Sensor {
public:
  SensorHallLin(int pin) :_pin(pin) {}

  
  void update() override {
    Sensor_HallLin(_pin);
  }

   void reset() override {
    
  }

   void init() override {
    
  }

private:
  int _pin;
};
