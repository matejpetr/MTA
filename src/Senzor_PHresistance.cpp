#include <Arduino.h>
#include "Senzor_PHresistance.hpp"

void PHresistance_update(int pin) {

  int raw = analogRead(pin);
  float voltage = (raw / 4095.0 * 3.3);
  float R = (voltage * 10000.0) / (3.3 - voltage);  //Odpor fotorezistoru
  float lux = 455.0 * pow(10000.0 / R, 0.68);  //aproximační vzorec (lze kalibrovat)

  Serial.print(F("?type=PHresistance&id=17&intensity="));
  //Serial.println(lux); // intenzita světla v luxech
  Serial.println(analogRead(pin));
}

void PHresistance_reset(){}


//nefunguje spravne
bool PHresistance_init(int pin){
  return (analogRead(pin)>1000);   

}