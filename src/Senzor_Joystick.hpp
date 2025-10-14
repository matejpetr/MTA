#pragma once

#include <Arduino.h>
#include <vector>
#include "Sensor.hpp"


class Joystick : public Sensor {
public:
  Joystick(int x, int y, int sw)
    : _x(x), _y(y), _sw(sw),
      _res(12), _threshold(25),
      _calibrated(false), _cx(0), _cy(0),
      _prevRes(-1), _maxADC(4095), _pinsInited(false) {
    analogReadResolution(_res);
  }

  // init: provede kalibraci středu a rychlou kontrolu vstupů
  bool init() override;

  // reset: zneplatní kalibraci – další update ji provede znovu
  void reset() override { _calibrated = false; }

  // update: vrací {"direction","CENTER|CLICK|UP|DOWN|LEFT|RIGHT"}
  std::vector<KV> update() override;

  const char* getType() override { return "Joystick"; }

  // config: Res (ADC bity), Threshold (0–100 % mrtvé zóny)
  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if      (params[i].key == "Res")       _res       = params[i].value.toInt();
      else if (params[i].key == "Threshold") _threshold = params[i].value.toInt();
    }
    analogReadResolution(_res);
  }

private:
  // pomocníci
  void  calibrateCenter_(int res);
  int   pctToAdcTol_(int pct) const;

  // piny a nastavení
  int  _x, _y, _sw;
  int  _res;         // ADC rozlišení v bitech
  int  _threshold;   // % mrtvé zóny (0–100)

  // stav kalibrace
  bool     _calibrated;
  int      _cx, _cy;
  int      _prevRes;
  uint32_t _maxADC;

  bool _pinsInited;
};
