#pragma once

//#include <dht11.h>
#include <DHT.h>
#include "OneWire.h"
#include "Sensor.hpp"

void Sensor_DHT11(int pin);
void Init_DHT11(int pin);
extern DHT dht;

class SensorDHT11 : public Sensor {
public:
  SensorDHT11(int pin)
    : _pin(pin) {dht.begin();}

  void update() override {
    Sensor_DHT11(_pin);
  }

  void reset() override {
    // případný reset snímače
  }

  void init() override {
  Init_DHT11(_pin);
  }

private:
  int _pin;
};
