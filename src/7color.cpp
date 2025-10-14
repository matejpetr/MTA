#include "7color.hpp"
#include <Arduino.h>


void Color7_config(int pin, bool control){
    digitalWrite(pin, control);

}


void Color7_reset(int pin){
    digitalWrite(pin, LOW);
}