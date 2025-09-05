#pragma once

#include <Arduino.h>
#include "Sensor.hpp"
#include "Parser.hpp"

void PHresistance_update(int pin, int res, float gain);
bool PHresistance_init(int pin);
void PHresistance_reset();

class PHresistance : public Sensor {
public:
  PHresistance(int pin)
    : _pin(pin), _res(12), _gain(1.0f) {
    analogReadResolution(_res);
  }

  void update() override {PHresistance_update(_pin, _res, _gain);}
  bool init() override {return PHresistance_init(_pin);}
  void reset() override {PHresistance_reset();}
  const char* getType() override {return "PHresistance";}

  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if (params[i].key == "Res")_res = params[i].value.toInt();
      else if (params[i].key == "Gain")_gain = params[i].value.toFloat();
    }
    analogReadResolution(_res);
  }

private:
  int _pin;
  int _res;
  float _gain;
};
