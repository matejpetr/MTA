#include "Senzor_DS18B20.hpp"
#include <Arduino.h>



void DS18B20_update(DallasTemperature &sensors){

  sensors.requestTemperatures();  
  float temperature = sensors.getTempCByIndex(0);  
  Serial.print(F("?type=ds11b20&id=0&temp="));
  Serial.println(temperature);
  
}

bool DS18B20_init(DallasTemperature &sensors) {
  int devCount = sensors.getDeviceCount();
  if (devCount > 0) {
    return true;
  } 
  else {return false;}
}

void DS18B20_reset(DallasTemperature &sensors){
sensors.begin(); 
}