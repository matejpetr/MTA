#pragma once

#include <Arduino.h>
#include <vector>
#include "Sensor.hpp"
#include <Wire.h>
extern TwoWire I2C;

class BMP180 : public Sensor {
public:
  BMP180(int sda, int scl) : _sda(sda), _scl(scl) {}

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
  
  const char* getType() override { return "BMP180"; }

  // Konfigurace: pouze gain (zvětšení/kalibrace výsledného tlaku)
  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if (params[i].key == "Gain") _gain = params[i].value.toFloat();
    }
  }

private:
  // pevný offset (stejně jako v původním kódu), můžeš případně udělat i konfigurovatelný
  static constexpr float CAL_OFFSET = 31.5f; // mbar -> *100 v kódu pro Pa

  int   _sda;
  int   _scl;
  float _gain = 1.0f;
};
