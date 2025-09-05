#pragma once

#include <Arduino.h>
#include "Sensor.hpp"
#include "Parser.hpp"

bool BMP180_init(int SDA, int SCL);
void BMP180_update(float gain);
void BMP180_reset();

class BMP180 : public Sensor {
public:
  BMP180(int sda, int scl) : _sda(sda), _scl(scl), _gain(1.0f) {}

  bool init() override { return BMP180_init(_sda, _scl); }
  void update() override { BMP180_update(_gain); }
  void reset() override { BMP180_reset(); }
  const char* getType() override { return "BMP180"; }

  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if (params[i].key == "Gain") _gain = params[i].value.toFloat();
    }
  }

private:
  int _sda, _scl;
  float _gain;
};
