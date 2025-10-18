#pragma once

#include <Arduino.h>
#include <vector>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include "Sensor.hpp"


// Pokud už I2C a tcs existují jinde jako globály, nech klidně jen extern.
// Tady je ponecháme takto:
extern TwoWire I2C;
extern Adafruit_TCS34725 tcs;

class TCS34725 : public Sensor {
public:
  TCS34725(int sda, int scl)
    : _sda(sda), _scl(scl), _itime(600), _gain(4) {}

    void attach(const std::vector<int>& pins) override {
      if (pins.size() >= 1) _sda = pins[0];
      if (pins.size() >= 2) _scl = pins[1];
    }
    void detach() override {
      if (_sda >= 0) pinMode(_sda, INPUT);
      if (_scl >= 0) pinMode(_scl, INPUT);
      tcs.disable();              // pokud je enabled
      I2C.end();    
      
      _tcsEnabled = false;
      _readyAtMs  = 0;
      _blockFirst = false;  
    }  


  bool init() override;
  void            reset()  override;         // re-begin na stejné adrese
  std::vector<KV> update() override;         // vrací R,G,B (0–255)
  const char*     getType() override { return "TCS34725"; }

  // config v HPP: nastaví hodnoty a zavolá applyConfig_() (HW až v .cpp)
  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if      (params[i].key == "Itime") _itime = params[i].value.toInt();
      else if (params[i].key == "Gain")  _gain  = params[i].value.toInt();
    }
    applyConfig_();
  }

private:
  void applyConfig_();     // implementace v .cpp (mapování itime/gain → enumy)

  int _sda, _scl;
  int _itime;   // ms: 2,50,101,199,300,401,499,600
  int _gain;    // 1,4,16,60
  bool     _tcsEnabled = false;
  uint32_t _readyAtMs  = 0;
  bool     _blockFirst = false;   // <<< nový flag: první UPDATE po init blokuje
};
