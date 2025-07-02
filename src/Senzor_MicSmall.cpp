#include "Senzor_MicSmall.hpp"
#include <Arduino.h>
float ref = 100;

void MicSmall_update(int pin,int MT) {

     //referenční hodnota zvuku 
    float MaxDiff = 0;  // Maximální rozdíl mezi měřeným a referenčním zvukem
    unsigned long StartTime = millis();
    while (millis() - StartTime < MT){      // MT představuje Measuring Time (délka jednoho měření)
        float soundLevel = analogRead(pin);
        float diff = abs(soundLevel - ref);  // Absolutní rozdíl
        if (diff > MaxDiff) MaxDiff = diff;
    }

    float volume = 20.0 * log10(MaxDiff + 1); // Převod na decibely
    Serial.print(F("?type=MicSmall&id=34&volume=")); 
    Serial.println(volume);
}


bool MicSmall_init(int pin){
    return (analogRead(pin)<ref);
}
void MicSmall_reset(){}

