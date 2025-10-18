#include "TwoColor.hpp"
#include <Arduino.h>

// držíme poslední použité piny pro reset(); hlídáme -1
static int s_pinR = -1;
static int s_pinG = -1;

// mapujeme 0..100 -> 0..255 (když pošleš mimo rozsah, ořežeme)
static inline int clamp100(int v) { return v < 0 ? 0 : (v > 100 ? 100 : v); }
static inline int mapToPWM(int brig01_100) {
  brig01_100 = clamp100(brig01_100);
  return map(brig01_100, 0, 100, 0, 255);
}

void TwoColor_config(int pinRed, int pinGreen, char color, int Brightness) {
  s_pinR = pinRed;
  s_pinG = pinGreen;

  // nastav režim jen když je pin platný
  if (s_pinR >= 0) pinMode(s_pinR, OUTPUT);
  if (s_pinG >= 0) pinMode(s_pinG, OUTPUT);

  const int pwmValue = mapToPWM(Brightness);

  // zhasni obě větve, ale pouze pokud pin existuje
  if (s_pinR >= 0) analogWrite(s_pinR, 0);
  if (s_pinG >= 0) analogWrite(s_pinG, 0);

  // rozsvit vybranou barvu (pokud má pin)
  if (color == 'r' || color == 'R') {
    if (s_pinR >= 0) analogWrite(s_pinR, pwmValue);analogWrite(s_pinG, 0);
  } else if (color == 'g' || color == 'G') {
    if (s_pinG >= 0) analogWrite(s_pinG, pwmValue);analogWrite(s_pinR, 0);;
  } else {
    // neznámá barva -> nech zhasnuto
  }
}

void TwoColor_reset() {
  if (s_pinR >= 0) analogWrite(s_pinR, 0);
  if (s_pinG >= 0) analogWrite(s_pinG, 0);
}
