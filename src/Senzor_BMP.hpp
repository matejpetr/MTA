#pragma once

#include "Sensor.hpp"              // základní abstraktní třída
#include <Wire.h>
#include <Arduino.h>

bool BMP280_init(int SDA, int SCL);
void BMP280_update();
void BMP280_reset();

class BMP280 : public Sensor {
public:
  BMP280(int sda, int scl) :_sda(sda), _scl(scl) {}

  bool init() override {return BMP280_init(_sda, _scl);}
  void update() override {BMP280_update();}
  void reset() override {BMP280_reset();}
  const char* getType() override {return "BMP280";}
private:
  int _sda, _scl;
  
};


