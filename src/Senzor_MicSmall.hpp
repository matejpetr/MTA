#pragma once

#include <Arduino.h>
#include "Sensor.hpp"

void Sensor_MicSmall(int pin, int MT); //pin - A0, MT - Measuring time (ms)



class SensorMicSmall : public Sensor {
public:
  SensorMicSmall(int pin,int time) :_pin(pin), _time(time) {}

  
  void update() override {
    Sensor_MicSmall(_pin,_time);
  }

   void reset() override {
    
  }

   void init() override {
    
  }

private:
  int _pin,_time;
};
