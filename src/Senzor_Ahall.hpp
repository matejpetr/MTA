#pragma once

#include <Arduino.h>
#include <vector>
#include "Sensor.hpp"

class Ahall : public Sensor {
public:
  // pin se NEPRIŘAZUJE v konstruktoru pro produkční použití — vezme se pouze z attach()
  explicit Ahall(int pin = -1)
    : _pin(pin), _res(12), _lLimit(1800), _hLimit(2000), _val(0) {
    // nezapisovat analogReadResolution zde — aplikujeme to při attach/config
  }

  // API senzoru
  std::vector<KV> update() override;      // vrací {"val", ...}, {"polarity", ...}
  void            reset()  override { _val = 0; }
  bool            init()   override { return true; }   // nesběrnicový
  const char*     getType() override { return "Ahall"; }

  // Konfigurace: jen rozlišení a limity (nezmění pin)
  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if      (params[i].key == "Res")     _res    = params[i].value.toInt();
      else if (params[i].key == "LLimit")  _lLimit = params[i].value.toInt();
      else if (params[i].key == "HLimit")  _hLimit = params[i].value.toInt();
    }
    // aplikuj rozlišení ADC; pokud je pin přiřazen, ujisti se, že je nastaven jako vstup
    analogReadResolution(_res);
    if (_pin >= 0) {
      pinMode(_pin, INPUT);
    }
  }

  // PIN se bere pouze z attach()
  void attach(const std::vector<int>& pins) override {
    if (!pins.empty()) {
      _pin = pins[0];
      if (_pin >= 0) {
        pinMode(_pin, INPUT);
        analogReadResolution(_res);
      }
    }
  }

  // detach: bezpečné uvolnění pinu (high-impedance)
  void detach() override {
    if (_pin >= 0) {
      pinMode(_pin, INPUT); // high impedance
      _pin = -1;
    }
  }

private:
  int _pin;
  int _res;
  int _lLimit;
  int _hLimit;
  int _val;        // poslední hodnota pro reset
};
