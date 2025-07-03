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
  
  float ping = sharp.read(0);

  
  
  return (ping<20);
}

void GP2Y0A21YK0F_reset(){
  
}



