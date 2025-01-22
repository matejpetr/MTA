#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2  // Pin pro připojení datového vodiče senzoru

void Sensor_DS18B20(DallasTemperature &sensors);