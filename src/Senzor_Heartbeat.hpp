#pragma once

#include<Arduino.h>
#include "Sensor.hpp"

void Heartbeat_update(int pin, int time);    //time je délka měření v ms   
bool Heartbeat_init(int pin);
void Heartbeat_reset();

class Heartbeat : public Sensor {
public:
  Heartbeat(int pin,int time) :_pin(pin), _time(time) {}

  
  void update() override {Heartbeat_update(_pin,_time);}
  void reset() override {Heartbeat_reset();}
  bool init() override {return Heartbeat_init(_pin);}
  const char* getType() override {return "Heartbeat";}

private:
  int _pin,_time;
};
