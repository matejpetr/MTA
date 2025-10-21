#pragma once
#include <Arduino.h>
#include <vector>
#include "Sensor.hpp"
#include "driver/gpio.h"
#include "esp_attr.h"
#include "freertos/portmacro.h"

// Kvadraturní rotační enkodér (ISR-based) - kompatibilní s ESP32
class Rencoder : public Sensor {
public:
  Rencoder(int pinA = -1, int pinB = -1)
    : _pinA(pinA), _pinB(pinB),
      _lLimit(-100), _hLimit(100),
      _dir(1),
      _unit(-1), _last(0),
      _ticks(0), _prevState(0), _position(0) {}

  bool init() override;
  void reset() override;
  std::vector<KV> update() override;
  const char* getType() override { return "Encoder"; }

  void attach(const std::vector<int>& pins) override {
    if (pins.size() >= 1) _pinB = pins[0];
    if (pins.size() >= 2) _pinA = pins[1];
  }

  void detach() override;

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
  int  _lLimit, _hLimit;
  int  _dir;

  // PCNT fallback (nevyužíváme, ale uchováváme pole)
  int      _unit;
  int16_t  _last;

  // ISR-based counting
  volatile int64_t _ticks;     // atomický akumulátor
  volatile uint8_t _prevState; // poslední AB stav
  long _position;              // poslední reportovaná pozice

  // ISR / synchronizace
  static portMUX_TYPE s_mux;
  static bool s_isrInstalled;
  static void IRAM_ATTR gpio_isr_router(void* arg);
  void IRAM_ATTR handleEdgeISR();

  bool startISR();
  void stopISR();
};
