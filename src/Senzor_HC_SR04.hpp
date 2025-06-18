#pragma once

#include "Sensor.hpp"             // základní rozhraní
#include <Arduino.h>
#include <NewPing.h>

void Sensor_HCSR04(int trig, int echo, int limit);

class SensorHCSR04 : public Sensor {
public:
  SensorHCSR04(int trig, int echo, int limit)
    : _trig(trig), _echo(echo), _limit(limit) {}

  void update() override {
    Sensor_HCSR04(_trig, _echo, _limit); // správné volání funkce
  }

  void reset() override {
    // případně nastavit limity znovu
  }

  void init() override {
    // případná inicializace
  }

private:
  int _trig, _echo, _limit;
};