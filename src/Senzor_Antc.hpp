#pragma once
#include <Arduino.h>
#include "Sensor.hpp"

class Antc : public Sensor {
public:
  enum class Filter { Off=0, Light=1, Heavy=2 };

  explicit Antc(int pin) : _pin(pin), _res(12), _filter(Filter::Off) {
    analogReadResolution(_res);
  }

  std::vector<KV> update() override;                  
  void reset()  override {}
  bool init()   override { return true; } //non bus sensor
  const char* getType() override { return "Antc"; }

  //configuration parameters:
  void config(Param* params=nullptr, int count=0) override {
    for (int i=0; i<count; ++i) {
      if (params[i].key == "Res") _res = params[i].value.toInt();
      else if (params[i].key == "Filter")
        _filter = static_cast<Filter>(params[i].value.toInt());
    }
    analogReadResolution(_res);
  }

private:
  static float computeTemp(int adc, int resBits);
  int samplesFor(Filter f) const {
    return (f == Filter::Heavy) ? 10 : (f == Filter::Light ? 3 : 1);
  }

  int _pin;
  int _res;
  Filter _filter;
};
