#pragma once

#include <Arduino.h>
#include "Sensor.hpp"

void Sensor_Joystick(int x,int y,int sw); //x - pin VRx, y - pin VRy, sw - pin SW



class SensorJoystick : public Sensor {
public:
  SensorJoystick(int x,int y,int sw) :_x(x),_y(y),_sw(sw) {}

  
  void update() override {
    Sensor_Joystick(_x, _y, _sw);
  }

   void reset() override {
    
  }

   void init() override {
    
  }

private:
  int _x, _y, _sw;
};
