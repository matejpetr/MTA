  #pragma once

  #include <Wire.h>
  #include "Adafruit_TCS34725.h"
  #include "Sensor.hpp"

  void TCS34725_update();
  bool TCS34725_init(int SDA, int SCL);
  void TCS34725_reset();

  extern Adafruit_TCS34725 tcs;
  extern TwoWire I2C; 

  class TCS34725 : public Sensor {
  public:
    TCS34725(int sda, int scl) :_sda(sda), _scl(scl) {}

    void update() override {TCS34725_update();}
    void reset() override {TCS34725_reset();}
    bool init() override {return TCS34725_init(_sda,_scl);}
    const char* getType() override {return "TCS34725";}

  private:
    int _sda, _scl;

  };
