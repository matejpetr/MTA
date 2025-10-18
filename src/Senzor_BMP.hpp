#pragma once

#include <Arduino.h>
#include <vector>
#include "Sensor.hpp"
#include <Wire.h>

#include <Adafruit_BMP280.h>
extern TwoWire I2C;

class BMP280 : public Sensor {
public:
  BMP280(int sda, int scl)
    : _sda(sda), _scl(scl), _os_temp(2), _os_press(16), _filter(4) {}

    void attach(const std::vector<int>& pins) override {
    if (pins.size() >= 1) _sda = pins[0];
    if (pins.size() >= 2) _scl = pins[1];
  }

    void detach() override {
    if (_sda >= 0) pinMode(_sda, INPUT);
    if (_scl >= 0) pinMode(_scl, INPUT);
    I2C.end();
  }
  

  bool init() override;
  std::vector<KV> update() override;
  void reset() override;
  const char* getType() override { return "BMP280"; }

  // Konfigurace: oversampling teploty/tlaku + filtr
  void config(Param* params = nullptr, int count = 0) override;

private:
  // pomocná: mapování číselných hodnot na enumy knihovny
  static Adafruit_BMP280::sensor_sampling mapOs(int v);
  static Adafruit_BMP280::sensor_filter   mapFilter(int v);

  int _sda, _scl;
  int _os_temp;   // 1,2,4,8,16 (default 2)
  int _os_press;  // 1,2,4,8,16 (default 16)
  int _filter;    // 0(off),2,4,8,16 (default 4)
};
