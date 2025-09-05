#include <Setup.hpp>
#include "Senzor_HC_SR04.hpp"

void HCSR04_update(int trig, int echo, int limit, int delayMs) {
  NewPing sonar(trig, echo, limit);
  delay(delayMs);  // Wait 40ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.

  //int dist = sonar.ping_cm();
  unsigned int us = sonar.ping();
  float dist = us / 57.0f;  // cm, s desetinnými místy

  String out = "?type=HCSR04&id=6&distance=" + String(dist, 1);
  if (ResponseAll) globalBuffer += out;
  else Serial.println(out);
}

bool HCSR04_init(int trig, int echo, int limit) {
  NewPing sonar(trig, echo, limit);
  int dist = sonar.ping_cm();
  return (dist > 2);  
}

void HCSR04_reset() {
  // není potřeba žádná akce
}
