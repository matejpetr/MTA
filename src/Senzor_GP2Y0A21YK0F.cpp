#include <Arduino.h>
#include "Senzor_GP2Y0A21YK0F.hpp"
#include <GP2Y0A21YK0F.h>


void Sensor_GP2Y0A21YK0F (int pin){
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
 



