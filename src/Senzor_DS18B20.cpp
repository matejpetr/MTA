#include "Senzor_DS18B20.hpp"
#include <Arduino.h>



void Sensor_DS18B20(DallasTemperature &sensors){

  sensors.requestTemperatures();  
  float temperature = sensors.getTempCByIndex(0);  
  Serial.print(F("?type=ds11b20&id=0&temp="));
  Serial.println(temperature);
}