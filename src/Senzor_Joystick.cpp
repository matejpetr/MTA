#include <Arduino.h>
#include "Senzor_Joystick.hpp"

void Joystick_update(int x, int y, int sw) {
    
    int BtnState;
    int threshold = 1000;  // Threshold sloužící jako mez pro změnu stavu
    int xValue = analogRead(x);
    int yValue = analogRead(y);

    pinMode(sw, INPUT_PULLUP);  // Nastavení tlačítka jako vstup s pull-up odporem
    BtnState = digitalRead(sw);  

    // Určování směru Joysticku na základě porovnávání vstupních hodnot s Thresholdem
    String direction = (BtnState == LOW) ? "CLICK" :    
                       (xValue > (2500 + threshold)) ? "UP" :
                       (xValue < (2500 - threshold)) ? "DOWN" :
                       (yValue > (2500 + threshold)) ? "RIGHT" :
                       (yValue < (2500 - threshold)) ? "LEFT" :
                       "CENTER";   

    Serial.print(F("?type=Joystick&id=18&direction="));
    Serial.println(direction);
}

bool Joystick_init(int x, int y) {
  int xValue = analogRead(x);
  int yValue = analogRead(y);

  return (xValue >= 2850 && xValue <= 3000) &&
         (yValue >= 2850 && yValue <= 3000);
}


void Joystick_reset(){}