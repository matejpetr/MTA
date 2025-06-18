#pragma once

#include "Sensor.hpp"
#include <OneWire.h>
#include <DallasTemperature.h>


void Sensor_DS18B20(DallasTemperature &sensors);

class SensorDS18B20 : public Sensor {
public:
  SensorDS18B20(OneWire* oneWire)
    : _sensors(oneWire) {
    _sensors.begin();  
  }

  void update() override {
   Sensor_DS18B20(_sensors);
  }

  void reset() override {}
  void init() override {}

private:
 
  DallasTemperature _sensors;
};
