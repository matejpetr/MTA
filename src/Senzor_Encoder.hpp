#pragma once
#include <Arduino.h>
#include <vector>
#include <Encoder.h> 
#include "Sensor.hpp"

// Kvadraturní rotační enkodér
class Rencoder : public Sensor {
public:
  Rencoder(int pinA, int pinB)
    : _pinA(pinA), _pinB(pinB),
      _lLimit(-100), _hLimit(100),
      _dir(1), _enc(nullptr), _position(0) {}

  bool init() override;                 // vytvoří instanci enkodéru
  void reset() override;                 // vynuluje pozici
  std::vector<KV> update() override;     // vrací {"position"}, {"alarm"}
  const char* getType() override { return "Encoder"; }

  // config v HPP: jen uloží parametry (bez HW volání)
  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if      (params[i].key == "Direction")
        _dir = (params[i].value == "reverse") ? -1 : 1;
      else if (params[i].key == "LowAlarm")
        _lLimit = params[i].value.toInt();
      else if (params[i].key == "HighAlarm")
        _hLimit = params[i].value.toInt();
    }
  }

private:
  int  _pinA, _pinB;
  int  _lLimit, _hLimit;   // alarmové meze
  int  _dir;               // 1 nebo -1
  ::Encoder* _enc;           // vlastněná instance enkodéru
  long _position;          // poslední reportovaná pozice
};
