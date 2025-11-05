#pragma once

#include <Arduino.h>
#include <vector>
#include "Sensor.hpp"
#include <Wire.h>

extern TwoWire I2C;

class BMP180 : public Sensor {
public:
  // konstruktor, piny se berou pouze z attach
  BMP180(int sda = -1, int scl = -1)
    : _sda(sda), _scl(scl), _gain(1.0f) {}

  // přiřazení pinů přes attach (SDA, SCL)
  void attach(const std::vector<int>& pins) override {
    if (pins.size() >= 1) _sda = pins[0];
    if (pins.size() >= 2) _scl = pins[1];
  }

  // uvolnění pinů a ukončení I2C rozhraní
  void detach() override {
    if (_sda >= 0) pinMode(_sda, INPUT);
    if (_scl >= 0) pinMode(_scl, INPUT);
    I2C.end();
  }

  bool init() override;
  std::vector<KV> update() override;
  void reset() override;
  const char* getType() override { return "BMP180"; }

  // Konfigurační parametry (např. Gain)
  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if (params[i].key == "Gain") _gain = params[i].value.toFloat();
    }
  }

private:
  // offset pro kalibraci tlaku
  static constexpr float CAL_OFFSET = 31.5f; // mbar

  int   _sda;     
  int   _scl;     
  float _gain;    // škálovací faktor tlaku
};
