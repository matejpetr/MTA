#include "Senzor_DigitalRead.hpp"

std::vector<KV> SensorDigitalRead::update() {
  // init() už nastavil pinMode(INPUT)
  const int digitalValue = digitalRead(_pin);

  std::vector<KV> kv;
  kv.push_back({"state", String(digitalValue)});
  return kv;
}
