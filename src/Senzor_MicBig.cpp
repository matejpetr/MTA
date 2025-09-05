#include <Setup.hpp>
#include "Senzor_MicBig.hpp"

int refB = 50;

void MicBig_update(int pin, int MT, int res) {
  analogReadResolution(res);

  float MaxDiff = 0;
  unsigned long StartTime = millis();

  while (millis() - StartTime < MT) {
    float soundLevel = analogRead(pin);
    float diff = abs(soundLevel - refB);
    if (diff > MaxDiff) MaxDiff = diff;
  }

  float volume = 20.0 * log10(MaxDiff + 1);

  String out = "?type=MicBig&id=28&volume=" + String(volume, 1);
  if (ResponseAll) globalBuffer += out;
  else Serial.println(out);
}

void MicBig_reset() {

}

bool MicBig_init(int pin) {
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

  return (diff < 10); 
}
