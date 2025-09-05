#include <Setup.hpp>
#include "Senzor_GP2Y0A21YK0F.hpp"
#include <GP2Y0A21YK0F.h>

void GP2Y0A21YK0F_update(int pin, int unit, float lAlarm, float hAlarm) {
  DMSU sharp(pin);
  float distance = sharp.read(unit);  // 0 = cm, 1 = mm

  // Ořezání extrémních hodnot
  if (distance > 80) distance = 81;
  else if (distance < 20) distance = 19;

  String alarm;
  if (distance < lAlarm) alarm = "LOW";
  else if (distance > hAlarm) alarm = "HIGH";
  else {alarm = "OK";}

  String out = "?type=GP2Y0A21YK0F&id=21&distance=" + String(distance, 1) +"&alarm=" + alarm;
  if (ResponseAll) globalBuffer += out;
  else Serial.println(out);
}

bool GP2Y0A21YK0F_init(int pin) {
  DMSU sharp(pin);
  float dist = sharp.read(0);
  return (dist >= 20 && dist <= 80);
}

void GP2Y0A21YK0F_reset() {}
