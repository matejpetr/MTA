#include "Senzor_DS18B20.hpp"
#include <Arduino.h>



void DS18B20_update(DallasTemperature &sensors){
  sensors.begin(); 
  delay(50);
  sensors.requestTemperatures();
  DeviceAddress addr = {0x28, 0xF8, 0xE4, 0x80, 0xE3, 0xE1, 0x3D, 0x00};
  float temperature = sensors.getTempC(addr);
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