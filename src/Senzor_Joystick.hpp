#pragma once

#include <Arduino.h>
#include "Sensor.hpp"

void Joystick_update(int x,int y,int sw); //x - pin VRx, y - pin VRy, sw - pin SW
bool Joystick_init(int x,int y);
void Joystick_reset();


class Joystick : public Sensor {
public:
  Joystick(int x,int y,int sw) :_x(x),_y(y),_sw(sw) {}

  
  void update() override {Joystick_update(_x, _y, _sw);}
  void reset() override {Joystick_reset();}
  bool init() override {return Joystick_init(_x, _y);}
  const char* getType() override {return "Joystick";}
  
private:
  int _x, _y, _sw;
};
