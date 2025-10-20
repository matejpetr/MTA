#pragma once
#include <Arduino.h>
#include "actuator.hpp"   // nechávám malým, ať sedí na tvůj projekt

void TwoColor_config(int pinRed, int pinGreen, char color, int Brightness);
void TwoColor_reset();

class TwoColor : public Actuator {
public:
  TwoColor(int pinRed, int pinGreen, char color, int Brightness)
    : _pinRed(pinRed), _pinGreen(pinGreen), _color(color), _Brightness(Brightness) {}

  // CONNECT: může přijít 1 nebo 2 piny
  void attach(const std::vector<int>& pins) override {
    if (pins.size() >= 1) _pinRed   = pins[0];
    if (pins.size() >= 2) _pinGreen = pins[1];
    // (žádné pinMode tady – řeší volná funkce v CONFIG/reset; hlavní je nepadnout na -1)
  }

  // CONFIG: piny IGNORUJEME (bereme je výhradně z CONNECT)
  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      // if (params[i].key == "pinRed")   _pinRed = params[i].value.toInt();   // NE!
      // if (params[i].key == "pinGreen") _pinGreen = params[i].value.toInt(); // NE!
      if      (params[i].key == "color") _color = params[i].value.charAt(0);
      else if (params[i].key == "Brig")  _Brightness = params[i].value.toInt();
    }
    TwoColor_config(_pinRed, _pinGreen, _color, _Brightness);
  }

  void reset() override { TwoColor_reset(); }

  // DISCONNECT: bezpečně vypne výstupy a uvolní piny
  void detach() override {
    // zavoláme reset aby bylo vše vypnuté (implementace může zhasnout LED)
    TwoColor_reset();

    if (_pinRed >= 0) {
      // bezpečně vypneme a přepneme na vstup (uvolnění)
      digitalWrite(_pinRed, LOW);
      pinMode(_pinRed, INPUT);
      _pinRed = -1;
    }
    if (_pinGreen >= 0) {
      digitalWrite(_pinGreen, LOW);
      pinMode(_pinGreen, INPUT);
      _pinGreen = -1;
    }
  }

private:
  int  _pinRed   = -1;
  int  _pinGreen = -1;
  char _color    = 'r';
  int  _Brightness = 0;
};
