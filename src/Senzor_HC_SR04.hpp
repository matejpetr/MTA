#pragma once

#include "Sensor.hpp"             // základní rozhraní
#include <Arduino.h>
#include <NewPing.h>

void HCSR04_update(int trig, int echo, int limit);
bool HCSR04_init(int trig, int echo, int limit);
void HCSR04_reset();

class HCSR04 : public Sensor {
public:
  HCSR04(int trig, int echo, int limit)
    : _trig(trig), _echo(echo), _limit(limit) {}

  void update() override {HCSR04_update(_trig, _echo, _limit);}
  void reset() override {HCSR04_reset();}
  bool init() override {return HCSR04_init(_trig, _echo, _limit);}
  const char* getType() override {return "HCSR04";}
private:
  int _trig, _echo, _limit;
};