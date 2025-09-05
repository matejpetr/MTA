#pragma once

#include <DHT.h>
#include "Sensor.hpp"
#include "Parser.hpp"  

void DHT11_update(int pin, bool unit, bool HI);
bool DHT11_init();

extern DHT dht;

class DHT11x : public Sensor {
public:
  DHT11x(int pin)
    : _pin(pin), _unit(false), _HI(false) {
      dht.begin();
    }

  void update() override {
    DHT11_update(_pin, _unit, _HI);
  }

  void reset() override {}
  bool init() override { return DHT11_init(); }
  const char* getType() override { return "DHT11"; }

  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if (params[i].key == "Unit") _unit = (params[i].value == "F");
      else if (params[i].key == "HI") _HI = (params[i].value == "true");
    }
  }

private:
  int _pin;
  bool _unit;
  bool _HI;
};
