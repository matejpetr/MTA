#include <Setup.hpp>
#include "Senzor_DHT11.hpp"

std::vector<KV> DHT11x::update() {
  float h = dht.readHumidity();
  float t = dht.readTemperature(_unitF);    // _unitF: false=°C, true=°F

  if (_useHI) {
    t = dht.computeHeatIndex(t, h, _unitF); // Heat Index v odpovídající jednotce
  }

  // Výstup stejně jako dřív, jen jako KVs (bez query-stringu)
  std::vector<KV> kv;
  kv.push_back({"humi", String(h, 1)});
  kv.push_back({"temp", String(t, 1)});
  return kv;
}

bool DHT11x::init(){ 
  dht.begin(); 
  return true; 
}  

void DHT11x::reset() { 
  dht.begin(); 
}

