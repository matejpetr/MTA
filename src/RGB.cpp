#include "RGB.hpp"
#include <Arduino.h>

static int pinR;
static int pinG;
static int pinB;

void RGB_config(int pR, int pG, int pB, int BrigR, int BrigG, int BrigB) {
  pinR = pR;
  pinG = pG;
  pinB = pB;

  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);

  BrigR = constrain(BrigR, 0, 100);
  BrigG = constrain(BrigG, 0, 100);
  BrigB = constrain(BrigB, 0, 100);

  int pwmR = map(BrigR, 0, 100, 0, 255);
  int pwmG = map(BrigG, 0, 100, 0, 255);
  int pwmB = map(BrigB, 0, 100, 0, 255);

  analogWrite(pinR, pwmR);
  analogWrite(pinG, pwmG);
  analogWrite(pinB, pwmB);
}

void RGB_reset() {
    analogWrite(pinR, 0);
    analogWrite(pinG, 0);
    analogWrite(pinB,0);
}
