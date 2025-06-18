#include "Senzor_DHT11.hpp"
#include <Arduino.h>


//extern DHT dht;

void Init_DHT11(int pin) {
 
  dht.begin();
}


void Sensor_DHT11(int pin){
 
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  //float f = dht.readTemperature(true); // Read temperature as Fahrenheit (isFahrenheit = true)
  //float hif = dht.computeHeatIndex(f, h); // Compute heat index in Fahrenheit (the default)
  //float hic = dht.computeHeatIndex(t, h, false); // Compute heat index in Celsius (isFahreheit = false)
  Serial.print(F("?type=dht11&id=0&humi="));
  Serial.print(h);
  Serial.print(F("&temp="));
  Serial.println(t);
}
