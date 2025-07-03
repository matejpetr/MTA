#include "Senzor_HC_SR04.hpp"
#include <Arduino.h>


void HCSR04_update(int trig, int echo, int limit){

  float last_value = 0;
  NewPing sonar(trig, echo, limit); 
  delay(40);  // Wait 40ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  int dist = sonar.ping_cm();  
  Serial.print(F("?type=HCSR04M&id=7&distance=")); 
  Serial.println(dist);
}

bool HCSR04_init(int trig, int echo, int limit) {
 NewPing sonar(trig, echo, limit); 
 int dist = sonar.ping_cm();   
 return (dist>2);
}


void HCSR04_reset(){}




