  #pragma once

  #include "Sensor.hpp"
  #include <OneWire.h>
  #include <DallasTemperature.h>


  void DS18B20_update(DallasTemperature &sensors);
  bool DS18B20_init(DallasTemperature &sensors);
  void DS18B20_reset(DallasTemperature &sensors);

 class DS18B20 : public Sensor {
public:
  DS18B20(DallasTemperature* sensors)
    : _sensors(sensors) {}

  void update() override {DS18B20_update(*_sensors);}
  bool init() override {return DS18B20_init(*_sensors);}
  void reset() override {DS18B20_reset(*_sensors);}
  const char* getType() override {return "DS18B20";}
private:
  DallasTemperature* _sensors;
};
