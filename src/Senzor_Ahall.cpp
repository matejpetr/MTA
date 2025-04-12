#include <Arduino.h>
#include "Senzor_Ahall.hpp"

void Sensor_Ahall(int pin) {
  
  String polarity;
  int val = analogRead(pin);

  if (val > 1950) {     // porovnávání vstupní hodnoty vůči referenční
    polarity = "SOUTH";
  } else if (val < 1850) {
    polarity = "NORTH";
  } else {
    polarity = "NO MAGNET";
  }

  Serial.print(F("?type=Ahall&id=3&polarity="));
  Serial.println(polarity);
}

