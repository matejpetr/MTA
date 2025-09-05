#include <Setup.hpp>
#include "Senzor_DigitalRead.hpp"


void Sensor_DigitalRead(int Pin, int id, const char* type) {
  pinMode(Pin, INPUT);
  int digitalValue = digitalRead(Pin);
  String out = "?type=" + String(type) + "&id=" + String(id) + "&state=" + String(digitalValue);
  if (ResponseAll) globalBuffer += out;
  else Serial.println(out);
}


