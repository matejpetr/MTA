#include "Laser.hpp"
#include <Arduino.h>

// interní pin spravovaný funkcí setPin()
static int s_pin = -1;

void Laser_setPin(int pin) {
  s_pin = pin;
}

void Laser_config(bool control) {
  if (s_pin < 0) return;
  pinMode(s_pin, OUTPUT);
  digitalWrite(s_pin, control ? HIGH : LOW);
}

void Laser_reset() {
  if (s_pin < 0) return;
  digitalWrite(s_pin, LOW);
}

