#include "Senzor_MicBig.hpp"
#include <Arduino.h>
float refValue = 50;

void MicBig_update(int pin, int MT) {
    
     //referenční hodnota zvuku 
    float MaxDiff = 0;  // Maximální rozdíl mezi měřeným a referenčním zvukem
    unsigned long StartTime = millis();
    while (millis() - StartTime < MT){      // MT představuje Measuring Time (délka jednoho měření)
        float soundLevel = analogRead(pin);
        float diff = abs(soundLevel - refValue);  // Absolutní rozdíl
        if (diff > MaxDiff) MaxDiff = diff;
    }

    float volume = 20.0 * log10(MaxDiff + 1); // Převod na decibely
     Serial.print(F("?type=MicBig&id=35&volume=")); 
     //Serial.println(volume);
    Serial.println(analogRead(pin));
}

void MicBig_reset(){}

bool MicBig_init(int pin){
    return (analogRead(pin)>50);
}
    

