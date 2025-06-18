#pragma once

#include <Arduino.h>
#include "Sensor.hpp"

void Sensor_PHresistance(int pin);

class SensorPHresistance : public Sensor {
public:
  SensorPHresistance(int pin)
    : _pin(pin) {}

  void update() override {
    Sensor_PHresistance(_pin);
  }

  void reset() override {
  
  }

  void init() override {
   
  }

private:
  int _pin;
};
