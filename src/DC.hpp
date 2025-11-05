#pragma once
#include <Arduino.h>
#include "actuator.hpp"
#include <vector>

void DC_config(int pin, int Speed, bool state);
void DC_reset();

class DC : public Actuator {
public:
  DC(int pin, int Speed, bool state)
    : _pin(pin), _Speed(Speed), _state(state) {}

  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if (params[i].key == "Speed") _Speed = params[i].value.toInt();
      else if (params[i].key == "state") _state = (params[i].value == "true");
    }
    DC_config(_pin, _Speed, _state);
  }

  void reset() override { DC_reset(); }

  // přiřazení pinu přes attach, použije první pin a aplikuje konfiguraci
  void attach(const std::vector<int>& pins) override {
    if (!pins.empty()) {
      _pin = pins[0];
      if (_pin >= 0) {
        pinMode(_pin, OUTPUT);
        DC_config(_pin, _Speed, _state);
      }
    }
  }

  // uvolnění pinu a zastavení motoru
  void detach() override {
    if (_pin >= 0) {
      DC_reset();
      pinMode(_pin, INPUT);
      _pin = -1;
    }
  }

private:
  int _pin;
  int _Speed;
  bool _state;
};
