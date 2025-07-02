#include <Arduino.h>
#include"Senzor_Antc.hpp"
int raw;

void Antc_update(int pin) {
    int Raw = analogRead(pin);
    double Vo, R2;
    float Temp;
    
    Vo = (double)Raw * (3.3 / 4095);    // Převod ADC hodnoty na odpovídající napětí
    R2 = 10000 * (Vo / (3.3 - Vo));

    // Použití Steinhart-Hartovy rovnice pro výpočet teploty
    Temp = log(R2);
    Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp));
    Temp = Temp - 275.9;  // Kalibrovaný převod z Kelvinů na stupně Celsia (-2.75)

    Serial.print(F("?type=Antc&id=16&temp=")); 
    Serial.println(Temp); //Teplota ve stupních
}

bool Antc_init(int pin){
  const int samples = 5;
    int sum = 0;

    for (int i = 0; i < samples; i++) {
        sum += analogRead(pin);
        delay(5);
    }
    int avg = sum / samples;
    return (avg > 500 && avg < 2500);   // nutno upravit podle potřeby
}


void Antc_reset(){
raw = 0;
}
