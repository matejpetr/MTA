#include "Senzor_DigitalRead.hpp"
#include <Arduino.h>




void Sensor_DigitalRead(int Pin, int id, const char* type)
{
  pinMode(Pin, INPUT);
  int digitalValue = digitalRead(Pin);
  Serial.print(F("?type="));
  Serial.print(type);
  Serial.print(F("&id="));
  Serial.print(id);
  Serial.print(F("&state="));
  Serial.println(digitalValue);

}

