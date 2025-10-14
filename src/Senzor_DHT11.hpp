#pragma once

#include <Arduino.h>
#include <vector>
#include <DHT.h>
#include "Sensor.hpp"


extern DHT dht;  // sdílená instance jako doteď (konstruovaná jinde)

class DHT11x : public Sensor {
public:
  explicit DHT11x(int pin)
    : _pin(pin), _unitF(false), _useHI(false) {
      dht.begin();                 // zachováno z původního kódu
    }

  // vrací {"humi", ...}, {"temp", ...}
  std::vector<KV> update() override;

  void        reset() override { dht.begin(); }
  bool        init()  override { return true; }       // nesběrnicový (stejně jako u ostatních)
  const char* getType() override { return "DHT11"; }

  // Konfigurace: Unit = "F" pro Fahrenheit, HI = "true" pro Heat Index
  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if      (params[i].key == "Unit") _unitF = (params[i].value == "F");
      else if (params[i].key == "HI")   _useHI = (params[i].value == "true");
    }
  }

private:
  int  _pin;
  bool _unitF;   // false = °C, true = °F
  bool _useHI;   // zda použít Heat Index
};
