#pragma once

#include<Arduino.h>
#include "Sensor.hpp"

void Sensor_Heartbeat(int pin, int time);    //time je délka měření v ms   
 

class SensorHeartbeat : public Sensor {
public:
  SensorHeartbeat(int pin,int time) :_pin(pin), _time(time) {}

  
  void update() override {
    Sensor_Heartbeat(_pin,_time);
  }

   void reset() override {
    
  }

   void init() override {
    
  }

private:
  int _pin,_time;
};
