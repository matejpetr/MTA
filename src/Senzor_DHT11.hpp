#pragma once

//#include <dht11.h>
#include <DHT.h>
#include "OneWire.h"
#include "Sensor.hpp"

void DHT11_update(int pin);
bool DHT11_init();

extern DHT dht;

class DHT11x : public Sensor {
public:
  DHT11x(int pin)
    : _pin(pin) {dht.begin();}

  void update() override {DHT11_update(_pin);}
  void reset() override {}
  bool init() override {return DHT11_init();}
  const char* getType() override {return "DHT11";}
private:
  int _pin;
};
