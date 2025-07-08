#include "Senzor_DHT11.hpp"
#include <Arduino.h>


//extern DHT dht;


void DHT11_update(int pin){
 
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  Serial.print(F("?type=dht11&id=0&humi="));
  Serial.print(h);
  Serial.print(F("&temp="));
  Serial.println(t);
}

bool DHT11_init(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();
return (h&&t>0);

}

void DHT11_reset() {
  dht.begin();
}

