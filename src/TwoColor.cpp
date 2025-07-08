#include "TwoColor.hpp"
#include "Arduino.h"

static int pinR;
static int pinG;

void TwoColor_config(int pinRed, int pinGreen, char color, int Brightness) {
  pinR = pinRed;
  pinG = pinGreen;

  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);

  Brightness = constrain(Brightness, 0, 100);
  int pwmValue = map(Brightness, 0, 100, 0, 255);

  
  if (color == 'r' || color == 'R') {
    analogWrite(pinR, pwmValue);
  } else if (color == 'g' || color == 'G') {
    analogWrite(pinG, pwmValue);
  }
  
}

void TwoColor_reset() {
    analogWrite(pinR, 0);
    analogWrite(pinG, 0);
}
