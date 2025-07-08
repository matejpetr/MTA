#include "Stepper.hpp"
#include <Arduino.h>
#include <CheapStepper.h>

CheapStepper stepper;
bool direction;
int currentPosition = 0;

void Stepper_config(int pin1, int pin2, int pin3, int pin4, int angle, bool dir, int rpm) {
    stepper = CheapStepper(pin1, pin2, pin3, pin4);
    stepper.setRpm(rpm);
    direction = dir;

    if (direction) {
        currentPosition += angle;
    } else {
        currentPosition -= angle;
    }
    stepper.moveDegrees(direction, angle);
}



void Stepper_reset() {
    if (currentPosition != 0) {
        bool back = currentPosition > 0 ? false : true;
        stepper.setRpm(16);
        stepper.moveDegrees(back, abs(currentPosition));
        currentPosition = 0;
    }
}

