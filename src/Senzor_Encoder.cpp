#include <Setup.hpp>
#include "Senzor_Encoder.hpp"
#include <Encoder.h>

int direction_factor = 1;
Encoder* myEnc = nullptr;
long position = 0;

bool Encoder_init(int pinA, int pinB) {
    if (myEnc == nullptr) {
        myEnc = new Encoder(pinA, pinB);
    }
    return true;
}

void Encoder_reset() {
    if (myEnc != nullptr) {
        myEnc->write(0);
        position = 0;
    }
}

void Encoder_update(int lLimit, int hLimit) {
    if (myEnc != nullptr) {
        position = myEnc->read()/2* direction_factor;

        String alarm;
        if (position < lLimit) alarm = "LOW";
        else if (position > hLimit) alarm = "HIGH";
        else alarm = "OK";

        String out = "?type=Encoder&id=22&position=" + String(position) + "&alarm=" + alarm;
        if (ResponseAll) globalBuffer += out;
        else Serial.println(out);
    }
}