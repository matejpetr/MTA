#pragma once

#include <Arduino.h>
#include "Sensor.hpp"
#include "Parser.hpp"
#include "Setup.hpp"

void IRrx_update(int pin, unsigned long dedupMs);
void IRrx_reset();
void IRrx_init();

class IRrx : public Sensor {
public:
  explicit IRrx(int pin)
    : _pin(pin), _dedupMs(150) {}

  void update() override { IRrx_update(_pin, _dedupMs); }
  void reset()  override { IRrx_reset(); }
  bool init()   override { IRrx_init(); return true; }

  const char* getType() override { return "IRrx"; }
  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if (params[i].key == "dedup") {
        _dedupMs = (unsigned long) strtoul(params[i].value.c_str(), nullptr, 10);
      }
    }
  }

private:
  int _pin;
  unsigned long _dedupMs;
};
