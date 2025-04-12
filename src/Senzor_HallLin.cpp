#include <Arduino.h>
#include "Senzor_HallLin.hpp"

void Sensor_HallLin(int pin) {
  int raw = analogRead(pin); 
  float voltage = (raw / 4095.0) * 3.3; // Přepočet napětí 

  Serial.print(F("?type=HallLin&id=19&voltage="));
  Serial.println(voltage); // Napětí (V)
}
