#include <Arduino.h>
#include "Senzor_Ahall.hpp"
int val;


void Ahall_update(int pin) {
  
  String polarity;
  val = analogRead(pin);

  if (val > 2000) {     // porovnávání vstupní hodnoty vůči referenční
    polarity = "SOUTH";
  } else if (val < 1800) {
    polarity = "NORTH";
  } else {
    polarity = "NO MAGNET";
  }

  Serial.print(F("?type=Ahall&id=3&polarity="));
  Serial.println(polarity);
  
}



bool Ahall_init(int pin) {
  return(analogRead(pin)>1800);
}




void Ahall_reset(){
val = 0;
}

void Ahall_config(){
  
}
