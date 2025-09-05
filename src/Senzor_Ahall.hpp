#pragma once

#include <Arduino.h>
#include "Sensor.hpp"
#include "Parser.hpp"  

void Ahall_update(int pin, int lLimit, int hLimit,int res);
bool Ahall_init(int pin);  
void Ahall_reset();

class Ahall : public Sensor {
public:
  Ahall(int pin)
    : _pin(pin), _res(12), _lLimit(1800), _hLimit(2000) {
      analogReadResolution(_res);
    }

  void update() override { Ahall_update(_pin, _lLimit, _hLimit, _res); }
  void reset() override {Ahall_reset();}
  bool init() override {return Ahall_init(_pin);}
  const char* getType() override {return "Ahall";}

  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if (params[i].key == "Res") _res = params[i].value.toInt();
      else if (params[i].key == "LLimit")_lLimit = params[i].value.toInt();
      else if (params[i].key == "HLimit") _hLimit = params[i].value.toInt();
    }
    analogReadResolution(_res);
  }

private:
  int _pin;
  int _res;
  int _lLimit;
  int _hLimit;
};
