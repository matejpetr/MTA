  #pragma once

  #include <Wire.h>
  #include "Adafruit_TCS34725.h"
  #include "Sensor.hpp"
  #include "Parser.hpp"

  void TCS34725_update();
  bool TCS34725_init(int SDA, int SCL);
  void TCS34725_reset();
  void TCS34725_config(int itime,int gain);

  extern Adafruit_TCS34725 tcs;
  extern TwoWire I2C; 

  class TCS34725 : public Sensor {
    public:
    TCS34725(int sda, int scl)
      : _sda(sda), _scl(scl), _itime(600), _gain(4) {}

    void update() override { TCS34725_update(); }
    void reset() override { TCS34725_reset(); }
    bool init() override { return TCS34725_init(_sda, _scl); }
    const char* getType() override { return "TCS34725"; }

    void config(Param* params = nullptr, int count = 0) override {
      for (int i = 0; i < count; ++i) {
       if (params[i].key == "Itime") _itime = params[i].value.toInt();
        else if (params[i].key == "Gain") _gain = params[i].value.toInt();
      }
      TCS34725_config(_itime, _gain);
  }

private:
  int _sda, _scl;
  int _itime;
  int _gain;
};