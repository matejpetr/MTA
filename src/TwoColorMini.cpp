#include "TwoColor.hpp"
#include "TwoColorMini.hpp"

static int pinR;
static int pinG;

void TwoColorMini_config(int pinRed, int pinGreen, char color, int Brightness) {
  pinR = pinRed;
  pinG = pinGreen;

  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);

  Brightness = constrain(Brightness, 0, 100);
  int pwmValue = map(Brightness, 0, 100, 0, 255);

  // Inverze PWM pro společnou anodu KY-029
  pwmValue = 255 - pwmValue;

  // Vypnutí obou barev před zapnutím
  analogWrite(pinR, 255);
  analogWrite(pinG, 255);

  
    if (color == 'r' || color == 'R') {
      analogWrite(pinR, pwmValue);
    } else if (color == 'g' || color == 'G') {
      analogWrite(pinG, pwmValue);
    }
  
}

void TwoColorMini_reset() {
  analogWrite(pinR, 255);
  analogWrite(pinG, 255);
}
