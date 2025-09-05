#pragma once
#include "Sensor.hpp"
#include "Parser.hpp"
#include <Arduino.h>

bool Encoder_init(int pinA, int pinB);
void Encoder_update(int lLimit, int hLimit);
void Encoder_reset();
extern int direction_factor;


class Rencoder : public Sensor {
public:
    Rencoder(int pinA, int pinB)
        : _pinA(pinA), _pinB(pinB),
          _lLimit(-100), _hLimit(100) {}

    bool init() override { return Encoder_init(_pinA, _pinB); }
    void update() override { Encoder_update(_lLimit, _hLimit); }
    void reset() override { Encoder_reset(); }
    const char* getType() override { return "Encoder"; }

    void config(Param* params = nullptr, int count = 0) override {
        for (int i = 0; i < count; ++i) {
            if (params[i].key == "Direction") {
                if (params[i].value == "reverse") direction_factor = -1;
                else direction_factor = 1;
            }
            else if (params[i].key == "LowAlarm") _lLimit = params[i].value.toInt();
            else if (params[i].key == "HighAlarm") _hLimit = params[i].value.toInt();
        }
    }

private:
    int _pinA, _pinB;
    int _lLimit;
    int _hLimit;
};
