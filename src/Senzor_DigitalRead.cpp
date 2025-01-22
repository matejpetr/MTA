#include "Senzor_DigitalRead.hpp"
#include <Arduino.h>

void Specific_Sensor_DigitalRead(int digitalPin, String typeOfSensor,int sensorID)
{
  pinMode(digitalPin, INPUT);
  int digitalValue = digitalRead(digitalPin);
  Serial.print(F("?type="));
  Serial.print(typeOfSensor);
  Serial.print(F("&id="));
  Serial.print(sensorID);
  Serial.print(F("&value="));
  Serial.println(digitalValue);
}


void Sensor_DigitalRead(int digitalPin)
{
  pinMode(digitalPin, INPUT);
  int digitalValue = digitalRead(digitalPin);
  Serial.print(F("?type=DigitalRead&id=200&value="));
  Serial.println(digitalValue);
}

