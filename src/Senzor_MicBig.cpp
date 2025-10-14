#include "Senzor_MicBig.hpp"
#include <math.h>

bool MicBig::init() {
  // původní „MicBig_init“: klidový rozptyl musí být malý
  const int samples = 10;
  int minVal = (1 << _res) - 1;
  int maxVal = 0;
  long sum = 0;

  for (int i = 0; i < samples; ++i) {
    int val = analogRead(_pin);
    sum += val;
    if (val < minVal) minVal = val;
    if (val > maxVal) maxVal = val;
    delay(5);
  }

  const int diff = maxVal - minVal;
  return (diff < 10);   // stejné kritérium jako dřív
}

std::vector<KV> MicBig::update() {
  analogReadResolution(_res);

  float maxDiff = 0.0f;
  const unsigned long start = millis();

  while (millis() - start < (unsigned long)_time) {
    const float sample = analogRead(_pin);
    const float diff   = fabsf(sample - (float)REF_BASE);
    if (diff > maxDiff) maxDiff = diff;
  }

  const float volume = 20.0f * log10f(maxDiff + 1.0f);

  return { {"volume", String(volume, 1)} };
}
