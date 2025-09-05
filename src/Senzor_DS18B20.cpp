#include <Setup.hpp>
#include "Senzor_DS18B20.hpp"


DeviceAddress addr = {0x28, 0xF8, 0xE4, 0x80, 0xE3, 0xE1, 0x3D, 0x00};

void DS18B20_update(DallasTemperature &sensors,int res) {
  //sensors.begin(); 

  //delay(50);
  sensors.setResolution(addr, res);
  sensors.requestTemperatures();
  float temperature = sensors.getTempC(addr);

  // Zjisti alarmové hranice
  int lowAlarm = sensors.getLowAlarmTemp(addr);
  int highAlarm = sensors.getHighAlarmTemp(addr);

  String alarm;

  if (temperature <= lowAlarm) {
    alarm = "LOW";
  }
  else if (temperature >= highAlarm) {
    alarm = "HIGH";
  } else {
    alarm = "OK";
  }
  

  String out = "?type=ds18b20&id=0&temp=" + String(temperature, 1)+"&alarm="+alarm;
  if (ResponseAll) globalBuffer += out;
  else Serial.println(out);
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

void DS18B20_config(DallasTemperature &sensors, int res, int LAlarm, int HAlarm) {
  // Nastavení rozlišení (pouze pokud je v platném rozsahu)
  if (res >= 9 && res <= 12) {
    sensors.setResolution(addr, res);
  }

  // Nastavení alarmových teplot pro dolní mez
  if (LAlarm >= -55 && LAlarm <= 125) {
    sensors.setLowAlarmTemp(addr, LAlarm);
  }
 // Nastavení alarmových teplot pro horní mez
  if (HAlarm >= -55 && HAlarm <= 125) {
    sensors.setHighAlarmTemp(addr, HAlarm);
  }
  
  sensors.saveScratchPad(addr);
}
