#include <Arduino.h>
#include "BuzzP.hpp"


void BuzzP_config(int pin, int freq, int duration){
 tone(pin,freq,duration);
}


void BuzzP_reset(int pin){
 analogWrite(pin,0);
} 