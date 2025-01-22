#include "Senzor_DHT11.hpp"
#include <Arduino.h>

DHT11 dht11(1);

void Sensor_DHT11(OneWire &oneWire)
{
  float h = dht11.readHumidity();
  float t = dht11.readTemperature();
  //float f = dht.readTemperature(true); // Read temperature as Fahrenheit (isFahrenheit = true)
  //float hif = dht.computeHeatIndex(f, h); // Compute heat index in Fahrenheit (the default)
  //float hic = dht.computeHeatIndex(t, h, false); // Compute heat index in Celsius (isFahreheit = false)
  Serial.print(F("?type=dht11&id=0&humi="));
  Serial.print(h);
  Serial.print(F("&temp="));
  Serial.println(t);
}