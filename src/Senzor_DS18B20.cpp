#include "Senzor_DS18B20.hpp"
#include <Arduino.h>



void Sensor_DS18B20(DallasTemperature &sensors)
{
  sensors.requestTemperatures();  // Požádej senzory o teplotu
  float temperature = sensors.getTempCByIndex(0);  // Čti první senzor
  Serial.print(F("?type=ds11b20&id=0&temp="));
  Serial.println(temperature);
}