#pragma once

#include <Arduino.h>
#include "actuator.hpp"

void SG90_config(int pin,int angle, int delay);
void SG90_reset();



class SG90 : public Actuator {
public:
  SG90(int pin, int angle,int delay)
    : _pin(pin), _angle(angle), _delay(delay) {}

  void reset() override {SG90_reset();}
  void config() override {SG90_config(_pin,_angle,_delay);}
  
  
private:
  int _pin;
  int _angle;
  int _delay;
};