#pragma once
#include <Arduino.h>
#include <vector>
#include "Sensor.hpp"

class SensorDigitalRead : public Sensor {
public:
  SensorDigitalRead(uint8_t pin, int id, const char* type)
    : _pin(pin), _id(id), _type(type) {}

  bool init() override{return true;};

  void reset() override{};

  std::vector<KV> update() override;

  const char* getType() override { return "Digital"; }

private:
  uint8_t     _pin;
  int         _id;           // id si klidně necháváme, i když ho nepoužíváme
  const char* _type;         // jen kvůli kompatibilitě s tvým kódem
};
