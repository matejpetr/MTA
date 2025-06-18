#pragma once

#include "Sensor.hpp"              // základní abstraktní třída
#include <Wire.h>
#include <Arduino.h>

extern bool Sensor_BMP280_Init(int SDA, int SCL);
extern void Sensor_BMP280();

class SensorBMP280 : public Sensor {
public:
  SensorBMP280(int sda, int scl) :_sda(sda), _scl(scl) {}

  void init() override {
     
  }

  void update() override {
    Sensor_BMP280();                
  }

  void reset() override {
   Sensor_BMP280_Init(_sda, _scl);
  }

private:
  int _sda, _scl;
  
};


