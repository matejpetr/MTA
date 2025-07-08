#pragma once
#include "Sensor.hpp"
#include <Arduino.h>

bool Encoder_init(int pinA, int pinB);
void Encoder_update();
void Encoder_reset();

void IRAM_ATTR handleEncoder();

class Encoder : public Sensor {
public:
    Encoder(int pinA, int pinB) : _pinA(pinA), _pinB(pinB) {}
    bool init() override { return Encoder_init(_pinA, _pinB); }
    void update() override { Encoder_update(); }
    void reset() override { Encoder_reset(); }
    const char* getType() override { return "Encoder"; }

private:
    int _pinA, _pinB;
};
