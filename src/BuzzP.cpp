#include <Arduino.h>
#include "BuzzP.hpp"

static inline int chForPin(int pin) { return (pin & 0x07); }

void BuzzP_config(int pin, int freq, int duration){
 tone(pin,freq,duration);
}


void BuzzP_reset(int pin){
 analogWrite(pin,0);
} 


void BuzzP::init() {
  // Inicializace pinu jako výstup
  if (_freq <= 0) _freq = 1;              // 0 Hz neumí; 1 Hz ~ „ticho“
  const int ch = chForPin(_pin);

  // (Re)inicializuj LEDC kanál pro tento pin → odstraní "LEDC is not initialized"
  ledcDetachPin(_pin);                   // nevadí, když ještě nebyl připojen
  ledcSetup(ch, _freq, 8);               // 8bit rozlišení stačí
  ledcAttachPin(_pin, ch);
  ledcWriteTone(ch, _freq);

  if (_duration > 0) {
    delay(_duration);
    ledcWrite(ch, 0);                   // umlč
    ledcDetachPin(_pin);                 // volitelné: uvolni pin
  }
}