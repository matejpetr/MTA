#ifndef DMSU_H
#define DMSU_H

#include <Arduino.h>

#define CM 0
#define MM 1
#define M 2

class DMSU {
  public:
    DMSU(uint8_t pin);
    float read(uint8_t unit = CM);
  private:
    uint8_t _pin;
};

#endif
