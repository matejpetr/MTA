#pragma once

#include <Arduino.h>
#include "Sensor.hpp"

void Sensor_DigitalRead(int Pin, int id, const char* type);


class SensorDigitalRead : public Sensor {
public:
  SensorDigitalRead(int pin, int id, const char* type)
    : _pin(pin), _id(id), _type(type) {}

  void update() override {Sensor_DigitalRead(_pin, _id, _type);}
  void reset() override {}
  bool init() override {return false;}
  const char* getType() override {return "Digital";}
  
private:
  int _pin;
  int _id;
  const char* _type;
};



