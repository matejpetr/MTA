#include "Senzor_MicSmall.hpp"
#include <Arduino.h>

int refS = 50;
void MicSmall_update(int pin,int MT) {

     //referenční hodnota zvuku 
    float MaxDiff = 0;  // Maximální rozdíl mezi měřeným a referenčním zvukem
    unsigned long StartTime = millis();
    while (millis() - StartTime < MT){      // MT představuje Measuring Time (délka jednoho měření)
        float soundLevel = analogRead(pin);
        float diff = abs(soundLevel - refS);  // Absolutní rozdíl
        if (diff > MaxDiff) MaxDiff = diff;
    }

    float volume = 20.0 * log10(MaxDiff + 1); // Převod na decibely
    Serial.print(F("?type=MicSmall&id=34&volume=")); 
    Serial.println(volume);
}


bool MicSmall_init(int pin){
    const int samples = 10;
    int minVal = 4095;
    int maxVal = 0;
    long sum = 0;

    for (int i = 0; i < samples; i++) {
        int val = analogRead(pin);
        sum += val;
        if (val < minVal) minVal = val;
        if (val > maxVal) maxVal = val;
        delay(5);
    }

    int avg = sum / samples;
    int diff = maxVal - minVal;

    return(diff<10);
}

void MicSmall_reset(){}

