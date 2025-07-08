#include "Laser.hpp"
#include"Arduino.h"



void Laser_config(int pin, bool control){

 digitalWrite(pin,control);
}


void Laser_reset(int pin){
 digitalWrite(pin,0);
}

