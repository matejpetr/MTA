#pragma once

#include <Arduino.h>
#include "Sensor.hpp"

void Sensor_DigitalRead(int pinNo);
void Specific_Sensor_DigitalRead(int digitalPin, String typeOfSensor,int sensorID);

class SensorDigitalRead : public Sensor {
public:
  SensorDigitalRead(int pin) : _pin(pin) {}

  void update() override {
    Sensor_DigitalRead(_pin);
  }

   void reset() override {
    
  }

   void init() override {
    
  }

private:
  int _pin;
};


