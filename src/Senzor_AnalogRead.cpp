#include "Senzor_AnalogRead.hpp"
#include <Arduino.h>

void Specific_Sensor_AnalogRead(int analogPin, String typeOfSensor,int sensorID)
{
  pinMode(analogPin,INPUT);
  int analogValue = analogRead(analogPin);
  Serial.print(F("?type="));
  Serial.print(typeOfSensor);
  Serial.print(F("&id="));
  Serial.print(sensorID);
  Serial.print(F("&value="));
  Serial.println(analogValue);
}

void Sensor_AnalogRead(int analogPin)
{
  int analogValue = analogRead(analogPin);
  Serial.print(F("?type=AnalogRead&id=100&value="));
  Serial.println(analogValue);
}