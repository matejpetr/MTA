#pragma once

#include <Arduino.h>
#include "Sensor.hpp"

void Ahall_update(int pin);
bool Ahall_init(int pin);  
void Ahall_reset();


class Ahall : public Sensor {
public:
  Ahall(int pin) :_pin(pin) {}

  
  void update() override {Ahall_update(_pin);}
  void reset() override {Ahall_reset();}
  bool init() override {return Ahall_init(_pin);}
  const char* getType() override {return "Ahall";}

private:
  int _pin;
};
