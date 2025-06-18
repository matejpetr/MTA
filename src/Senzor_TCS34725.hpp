#pragma once

#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include "Sensor.hpp"

extern void Sensor_TCS34725();
extern bool Sensor_TCS34725_Init(int SDA, int SCL);


class SensorTCS34725 : public Sensor {
public:
  SensorTCS34725(int sda, int scl) :_sda(sda), _scl(scl) {}

  void update() override {
    Sensor_TCS34725();
  }

   void reset() override {
    Sensor_TCS34725_Init(_sda,_scl);
  }

   void init() override {
    
  }

private:
  int _sda, _scl;
};
