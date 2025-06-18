#pragma once

#include <arduino.h>
#include "Sensor.hpp"

void Sensor_Antc(int pin);


class SensorAntc : public Sensor {
public:
  SensorAntc(int pin) :_pin(pin) {}

  
  void update() override {
    Sensor_Antc(_pin);
  }

   void reset() override {
    
  }

   void init() override {
    
  }

private:
  int _pin;
};
