#pragma once

#include <arduino.h>
#include "Sensor.hpp"

void Antc_update(int pin);
bool Antc_init(int pin);
void Antc_reset();


class Antc : public Sensor {
public:
  Antc(int pin) :_pin(pin) {}

  
  void update() override {Antc_update(_pin);}
  void reset() override {Antc_reset();}
  bool init() override {return Antc_init(_pin);}
  const char* getType() override {return "Antc";}

private:
  int _pin;
};
