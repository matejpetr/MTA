#include <Arduino.h>
#include"Senzor_Antc.hpp"

void Sensor_Antc(int pin) {
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
