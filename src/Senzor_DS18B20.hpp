  #pragma once

  #include "Sensor.hpp"
  #include <OneWire.h>
  #include <DallasTemperature.h>
  #include "Parser.hpp"


  void DS18B20_update(DallasTemperature &sensors,int res);
  bool DS18B20_init(DallasTemperature &sensors);
  void DS18B20_reset(DallasTemperature &sensors);
  void DS18B20_config(DallasTemperature &sensors,int res, int LAlarm, int HAlarm);

 class DS18B20 : public Sensor {
public:
  DS18B20(DallasTemperature* sensors)
    : _sensors(sensors),_res(12) {}

  void update() override {DS18B20_update(*_sensors,_res);}
  bool init() override {return DS18B20_init(*_sensors);}
  void reset() override {DS18B20_reset(*_sensors);}

  void config(Param* params = nullptr, int count = 0) override {
    for (int i = 0; i < count; ++i) {
      if (params[i].key == "Res") _res = params[i].value.toInt();
      else if (params[i].key == "LAlarm") _LAlarm = params[i].value.toInt();
      else if (params[i].key == "HAlarm") _HAlarm = params[i].value.toInt();
    }
    DS18B20_config(*_sensors, _res, _LAlarm, _HAlarm);
}




  const char* getType() override {return "DS18B20";}
private:
  DallasTemperature* _sensors;
 int _res, _LAlarm, _HAlarm;
};
