#pragma once

#include <Arduino.h>
#include "Sensor.hpp"

void MicBig_update(int pin, int MT); //pin - A0, MT - Measuring time (ms)
bool MicBig_init(int pin);
void MicBig_reset();


class MicBig : public Sensor {
public:
  MicBig(int pin,int time) :_pin(pin), _time(time) {}

  
  void update() override {MicBig_update(_pin,_time);}
  void reset() override {MicBig_reset();}
  bool init() override {return MicBig_init(_pin);}
  const char* getType() override {return "MicBig";}

private:
  int _pin,_time;
};
