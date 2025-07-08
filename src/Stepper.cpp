#include "Stepper.hpp"
#include <Arduino.h>
#include <CheapStepper.h>

CheapStepper stepper;
bool direction;

void Stepper_config(int pin1, int pin2,int pin3, int pin4,int angle, bool dir, int rpm){

    stepper = CheapStepper(pin1, pin2, pin3, pin4);
    stepper.setRpm(rpm); 
    //defaultně 16.25 rpm, rozsah rpm: 6-24rpm
    //ideální rozsah pro správný chod: 10-22rpm
    direction=dir;
    stepper.moveDegrees (direction, angle); //true- po směru hodin, false proti
}

void Stepper_reset(){
    direction=false;
    stepper.moveDegrees (direction, 0);
}

