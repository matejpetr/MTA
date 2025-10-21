#pragma once

#include <Arduino.h>
#include <vector>
#include "Sensor.hpp"


class PHresistance : public Sensor {
public:
  // pin bude přiřazen pouze přes attach()
  explicit PHresistance(int pin = -1)
    : _pin(pin), _res(12), _gain(1.0f) {
    // nepovolovat HW zde — konfigurace ADC rozlišení se provede v init()/config()
  }

  bool            init()   override;                 // stejné jako dřív (>1000)
  void            reset()  override { /* no-op */ }
  std::vector<KV> update() override;                 // {"intensity", ...}
  const char*     getType() override { return "PHresistance"; }

  // Config v HPP: jen uloží Res + Gain
  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if      (params[i].key == "Res")  _res  = params[i].value.toInt();
      else if (params[i].key == "Gain") _gain = params[i].value.toFloat();
    }
    // aplikuj rozlišení ADC po změně konfigurace
    analogReadResolution(_res);
  }

  // PIN se bere pouze z attach()
  void attach(const std::vector<int>& pins) override {
    if (!pins.empty()) {
      _pin = pins[0];
      // bezpečné nastavení pinu pro měření (analog input)
      if (_pin >= 0) {
        pinMode(_pin, INPUT);
        // zajisti správné rozlišení ADC při attachu
        analogReadResolution(_res);
      }
    }
  }

  // detach: bezpečné uvolnění pinu
  void detach() override {
    if (_pin >= 0) {
      // nastavíme jako vstup bez pullu, uvolníme pin
      pinMode(_pin, INPUT);
      _pin = -1;
    }
  }

private:
  int   _pin;
  int   _res;    // ADC bity
  float _gain;   // násobek na výstupu (kalibrace)
};
