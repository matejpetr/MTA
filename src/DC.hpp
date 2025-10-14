#pragma once
#include <Arduino.h>
#include "actuator.hpp"


void DC_config(int pin, int Speed, bool state);
void DC_reset();

class DC : public Actuator {
public:
  DC(int pin, int Speed, bool state)
    : _pin(pin), _Speed(Speed), _state(state) {}

  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if (params[i].key == "pin") _pin = params[i].value.toInt();
      else if (params[i].key == "Speed") _Speed = params[i].value.toInt();
      else if (params[i].key == "state") _state = (params[i].value == "true");
    }
    DC_config(_pin, _Speed, _state);
  }

  void reset() override { DC_reset(); }

private:
  int _pin;
  int _Speed;
  bool _state;
};
