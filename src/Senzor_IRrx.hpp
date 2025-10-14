#pragma once
#include <Arduino.h>
#include <vector>
#include "Sensor.hpp"


// >>> Přidej forward deklarace GLOBÁLNĚ (mimo class IRrx)
class IRrecv;
struct decode_results;

class IRrx : public Sensor {
public:
  explicit IRrx(int pin) : _pin(pin), _dedupMs(150) {}

  bool            init()   override;
  void            reset()  override;
  std::vector<KV> update() override;
  const char*     getType() override { return "IRrx"; }

  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if (params[i].key == "dedup") {
        _dedupMs = (unsigned long) strtoul(params[i].value.c_str(), nullptr, 10);
      }
    }
  }

private:
  // helpery zůstávají stejné…
  void ensureIrRecv_();
  void startTaskIfNeeded_();
  static void taskEntry_(void* pv);
  void taskLoop_();
  static String toHex10_(uint32_t v);

  int            _pin;
  unsigned long  _dedupMs;

  // >>> Použij globální typy, NE vnořené
  IRrecv*         _irrecv = nullptr;          // z IRremoteESP8266
  void*           _task   = nullptr;          // TaskHandle_t, necháme jako void*
  decode_results* _res    = nullptr;          // z IRremoteESP8266

  // sdílená data
  volatile uint32_t      _latestVal   = 0;
  volatile bool          _haveAny     = false;
  volatile unsigned long _lastStoreMs = 0;

  uint32_t       _lastValRx = 0;
  unsigned long  _lastMsRx  = 0;
};
