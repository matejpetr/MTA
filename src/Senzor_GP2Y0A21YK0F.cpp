#include <Arduino.h>
#include "Senzor_GP2Y0A21YK0F.hpp"
#include <GP2Y0A21YK0F.h>


void GP2Y0A21YK0F_update(int pin){
  DMSU sharp(pin);
  
  float distance = sharp.read(0); // 0 pro cm, 1 pro mm

  //ošetření hodnot mimo měřicí rozsah
  if (distance > 80) {
    distance = 81;
  } else if (distance < 20) {
    distance = 19;
  }
  
  Serial.print(F("?type=GP2Y0A21YK0F&id=27&distance="));
  Serial.println(distance); // vzdálenost v cm 

}

bool GP2Y0A21YK0F_init(int pin) {
  DMSU sharp(pin);
  
  float first = sharp.read(0);
  delay(10);
  float last = sharp.read(0);

  // Rozdíl mezi první a poslední hodnotou
  float diff = abs(first - last);

  bool inRange = (first >= 20.0 && first <= 80.0); //hodnoty jsou ve správném rozsahu
  bool stable = (diff < 5.0); //rozdíl mezi hodnotami není moc velký (např. 5 cm)

  return inRange && stable;
}

void GP2Y0A21YK0F_reset(){
  
}



