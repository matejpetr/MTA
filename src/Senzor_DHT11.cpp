#include <Setup.hpp>
#include "Senzor_DHT11.hpp"

void DHT11_update(int pin, bool unit, bool HI){
 
  float h = dht.readHumidity();
  float t = dht.readTemperature(unit);

  if (HI) {
    t = dht.computeHeatIndex(t, h, unit);
  }
  
  String out = "?type=dht11&id=0&humi=" + String(h,1) + "&temp=" + String(t,1);
  if (ResponseAll) globalBuffer += out;
  else Serial.println(out);
}

bool DHT11_init(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();
return (h&&t>0);

}

void DHT11_reset() {
  dht.begin();
}

