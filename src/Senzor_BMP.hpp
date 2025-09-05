#pragma once

#include "Sensor.hpp"
#include <Wire.h>
#include <Arduino.h>
#include "Parser.hpp"

bool BMP280_init(int SDA, int SCL);
void BMP280_update();
void BMP280_reset();
void BMP280_config(int os_temp, int os_press, int filter);

class BMP280 : public Sensor {
public:
  BMP280(int sda, int scl)
    : _sda(sda), _scl(scl), _os_temp(2), _os_press(16), _filter(4) {}

  bool init() override { return BMP280_init(_sda, _scl); }
  void update() override { BMP280_update(); }
  void reset() override { BMP280_reset(); }
  const char* getType() override { return "BMP280"; }
  
  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if (params[i].key == "Os_temp") _os_temp = params[i].value.toInt();
      else if (params[i].key == "Os_press") _os_press = params[i].value.toInt();
      else if (params[i].key == "Filter") _filter = params[i].value.toInt();
    }
    BMP280_config(_os_temp, _os_press, _filter);
  }

  

private:
  int _sda, _scl;
  int _os_temp;
  int _os_press;
  int _filter;
};
