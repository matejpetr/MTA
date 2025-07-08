#include <Arduino.h>
#include "Senzor_HallLin.hpp"

void HallLin_update(int pin) {
  int raw = analogRead(pin); 
  float voltage = (raw / 4095.0) * 3.3; // Přepočet napětí 

  Serial.print(F("?type=HallLin&id=19&voltage="));
  Serial.println(voltage); // Napětí (V)
}

//spatne
bool HallLin_init(int pin) {
  int raw1 = analogRead(pin);
  float voltage1 = (raw1 / 4095.0) * 3.3;
  delay(10);
  int raw2 = analogRead(pin);
  float voltage2 = (raw2 / 4095.0) * 3.3;

  float diff = abs(voltage1 - voltage2);

  bool inRange = (voltage1 >= 0.9 && voltage1 <= 2);
  bool stable = (diff < 0.3);  // maximální povolená odchylka 0.3V

  return (inRange && stable);
}



void HallLin_reset() {}