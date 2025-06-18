#pragma once

#include <Arduino.h>
#include "Sensor.hpp"

void Sensor_MicBig(int pin, int MT); //pin - A0, MT - Measuring time (ms)



class SensorMicBig : public Sensor {
public:
  SensorMicBig(int pin,int time) :_pin(pin), _time(time) {}

  
  void update() override {
    Sensor_MicBig(_pin,_time);
  }

   void reset() override {
    
  }

   void init() override {
    
  }

private:
  int _pin,_time;
};
