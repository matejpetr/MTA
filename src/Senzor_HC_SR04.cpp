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

  int dist1 = sonar.ping_cm();
  delay(250);  // čekání 250 ms
  int dist2 = sonar.ping_cm();

  if (dist1 == 0 || dist2 == 0) return false; // neplatné měření

  int diff = abs(dist2 - dist1);
  return (diff <= 4); //maximální rozdíl 4cm
}

void HCSR04_reset(){}


