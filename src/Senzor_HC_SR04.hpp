#pragma once

#include "Sensor.hpp"
#include <Arduino.h>
#include <NewPing.h>
#include "Parser.hpp"

void HCSR04_update(int trig, int echo, int limit, int delayMs);
bool HCSR04_init(int trig, int echo, int limit);
void HCSR04_reset();

class HCSR04 : public Sensor {
public:
  HCSR04(int trig, int echo)
    : _trig(trig), _echo(echo), _limit(150), _delayMs(40) {}  // výchozí limit 150cm a delay 40ms

  void update() override {HCSR04_update(_trig, _echo, _limit, _delayMs);}
  void reset() override {HCSR04_reset();}
  bool init() override {return HCSR04_init(_trig, _echo, _limit);}
  const char* getType() override {return "HCSR04";}

  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if (params[i].key == "Limit") _limit = params[i].value.toInt();
      else if (params[i].key == "Delay") _delayMs = params[i].value.toInt();
    }
  }

private:
  int _trig, _echo;
  int _limit;
  int _delayMs;
};
