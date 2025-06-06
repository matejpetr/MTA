#include "GP2Y0A21YK0F.h"

DMSU::DMSU(uint8_t pin) {
  _pin = pin;
}

float DMSU::read(uint8_t unit) {
  float distance = 0;
  delay(50);
  int raw = analogRead(_pin);
  float voltage = (3.3 * raw) / 4095.0;  // ESP32: 12bit ADC, 3.3V

  // Kvadratická rovnice (inverzní vztah): výstup je v 1/cm
  float inv_d = 0.0070634 * voltage * voltage + 0.0259679 * voltage + 0.0025236; //0.0070634 * voltage * voltage + 0.0259679 * voltage + 0.0025236;
  distance = 1.0 / inv_d;

  if(unit == CM) return distance;
  if(unit == MM) return distance * 10;
  return distance * 0.01;  // metry
}
