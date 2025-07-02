#include <Arduino.h>
#include "Senzor_Ahall.hpp"
int val;


void Ahall_update(int pin) {
  
  String polarity;
  val = analogRead(pin);

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



bool Ahall_init(int pin) {
  int vzorky = 5;
  int sum = 0;
  for (int i = 0; i < vzorky; i++) {
    sum += analogRead(pin);
    delay(5);  
  }
  int avg = sum / vzorky;

  return (avg >= 1850 && avg <= 1950);
}




void Ahall_reset(){
val = 0;
}

void Ahall_config(){
  
}
