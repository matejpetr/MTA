#pragma once
#include <Arduino.h>
#include <ESP32Servo.h>
#include "Actuator.hpp"

// Volné funkce pro kompatibilitu
void SG90_setPin(int pin);            // pin nastaví attach()
void SG90_config(int angle, int speed); // konfigurace bez parametru pinu

class SG90 : public Actuator {
public:
  SG90(int pin, int angle, int speed)
    : _pin(pin), _angle(angle), _speed(speed) {}

  // Přijímá params z CONFIG (piny řeší CONNECT -> attach)
  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if (params[i].key == "angle") _angle = params[i].value.toInt();
      else if (params[i].key == "speed") _speed = params[i].value.toInt();
    }
    // Aplikuj na členské servo
    apply_();
  }

  // HW init až PO attach()
  void init() override {
    if (_pin < 0) return;
    if (_servo.attached()) _servo.detach();
    _servo.attach(_pin);
    // srovnáme do neutrálu rychle (bez zdržení)
    writeSmooth_(0, 0);
    _lastAngle = 0;
    SG90_setPin(_pin); // zajisti konzistenci volné funkce
  }

  // Reset = znovu-inicializace (ne deklarace!)
  void reset() override { init(); }

  // CONNECT -> dynamické přepojení pinu
  void attach(const std::vector<int>& pins) override {
    if (!pins.empty()) {
      int newPin = pins[0];
      if (newPin != _pin) {
        if (_servo.attached()) _servo.detach();
        if (_pin >= 0) pinMode(_pin, INPUT);
        _pin = newPin;
        _servo.attach(_pin);
        SG90_setPin(_pin); // pin pro volnou funkci je brán pouze z attach()
      }
    }
  }

  // Bezpečné uvolnění při DISCONNECT
  void detach() override {
    if (_servo.attached()) _servo.detach();
    if (_pin >= 0) pinMode(_pin, INPUT);
    SG90_setPin(-1); // uvolnit interní pin
  }

private:
  // Převod speed(0..100) -> delay mezi kroky (ms), 0 = nejrychleji
  static int speedToDelayMs_(int speed) {
    speed = constrain(speed, 0, 100);
    // tvoje původní map: (0 → 100 ms) vs (100 → 0 ms)
    return map(speed, 0, 100, 100, 0);
  }

  void writeSmooth_(int targetAngle, int speedMs) {
    targetAngle = constrain(targetAngle, 0, 180);
    if (speedMs <= 0) { _servo.write(targetAngle); return; }
    int step = (_lastAngle < targetAngle) ? 1 : -1;
    for (int pos = _lastAngle; pos != targetAngle; pos += step) {
      _servo.write(pos);
      delay(speedMs);
    }
    _servo.write(targetAngle);
  }

  void apply_() {
    if (_pin < 0) return;
    if (!_servo.attached()) _servo.attach(_pin);
    int d = speedToDelayMs_(_speed);
    _angle = constrain(_angle, 0, 180);
    if (_angle != _lastAngle) {
      writeSmooth_(_angle, d);
      _lastAngle = _angle;
    } else {
      _servo.write(_angle);
    }
  }

  Servo _servo;      // ČLEN třídy (žádný globál)
  int   _pin   = -1;
  int   _angle = 0;   // 0..180
  int   _speed = 0;   // 0..100
  int   _lastAngle = 0;
};