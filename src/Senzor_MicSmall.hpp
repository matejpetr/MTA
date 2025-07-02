#pragma once

#include <Arduino.h>
#include "Sensor.hpp"

void MicSmall_update(int pin, int MT); //pin - A0, MT - Measuring time (ms)
bool MicSmall_init(int pin);
void MicSmall_reset();



class MicSmall : public Sensor {
public:
  MicSmall(int pin,int time) :_pin(pin), _time(time) {}

  
  void update() override {MicSmall_update(_pin,_time);}
  void reset() override {MicSmall_reset();}
  bool init() override {return MicSmall_init(_pin);}
  const char* getType() override {return "MicSmall";}

private:
  int _pin,_time;
};
