#pragma once

#include <Arduino.h>
#include "actuator.hpp"


void IRtx_config(int pin, uint32_t code);
void IRtx_reset(int pin);

class IRtx : public Actuator {
public:
  IRtx(int pin, uint32_t code = 0x0)
    : _pin(pin), _code(code) {}

  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if (params[i].key == "code") {
        _code = parseHex32(params[i].value);
      }
    }
    IRtx_config(_pin, _code);
  }

  void reset() override {
    IRtx_reset(_pin);
  }

  // přiřazení pinu přes attach, použije první pin a inicializuje TX
  void attach(const std::vector<int>& pins) override {
    if (!pins.empty()) {
      _pin = pins[0];
      if (_pin >= 0) {
        pinMode(_pin, OUTPUT);
        IRtx_config(_pin, _code);
      }
    }
  }

  // uvolnění pinu a vypnutí TX
  void detach() override {
    if (_pin >= 0) {
      IRtx_reset(_pin);
      pinMode(_pin, INPUT);
      _pin = -1;
    }
  }

private:
  int _pin;
  uint32_t _code=0x00000000;  //initial value

  static uint32_t parseHex32(String s) {
    s.trim();
    if (s.startsWith("0x") || s.startsWith("0X")) s.remove(0, 2);
    s.toUpperCase();
    if (s.length() > 8) s = s.substring(s.length() - 8);
    uint32_t val = 0;
    for (int i = 0; i < s.length(); ++i) {
      char c = s[i];
      uint8_t n = (c >= '0' && c <= '9') ? (c - '0')
                 : (c >= 'A' && c <= 'F') ? (c - 'A' + 10)
                 : 0;
      val = (val << 4) | n;
    }
    return val;
  }
};
