#include <Setup.hpp>
#include "Senzor_PHresistance.hpp"

void PHresistance_update(int pin, int res, float gain) {
  analogReadResolution(res);
  int raw = analogRead(pin);

  // Výpočet napětí dle rozlišení
  float maxAdc = pow(2, res) - 1;
  float voltage = (raw / maxAdc) * 3.3;

  // Výpočet odporu fotorezistoru (předpoklad děliče s 10k odporem)
  float R = (voltage * 10000.0) / (3.3 - voltage);

  // Výpočet intenzity osvětlení v luxech + gain korekce
  float lux = gain * (455.0 * pow(10000.0 / R, 0.68)); //klidně upravit dle potřeby

  String out = "?type=PHresistance&id=15&intensity=" + String(lux, 1);
  if (ResponseAll) globalBuffer += out;
  else Serial.println(out);
}

bool PHresistance_init(int pin) {
  return (analogRead(pin) > 1000);
}
  
void PHresistance_reset() {
}
