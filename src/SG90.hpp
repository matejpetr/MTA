#pragma once

#include <Arduino.h>
#include "actuator.hpp"

void SG90_config(int pin,int angle, int speed);
void SG90_reset();



class SG90 : public Actuator {
public:
  SG90(int pin, int angle,int speed)
    : _pin(pin), _angle(angle), _speed(speed) {}

  void reset() override {SG90_reset();}
  void config() override {SG90_config(_pin,_angle,_speed);}
  
  
private:
  int _pin;
  int _angle;
  int _speed;
};