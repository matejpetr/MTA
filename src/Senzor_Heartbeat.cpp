#include <Setup.hpp>
#include "Senzor_Heartbeat.hpp"

void Heartbeat_update(int pin, int time_ms) {
  const int samp_siz = 4;       // velikost vyhlazovacího okna
  const int threshold = 4;      // počet po sobě jdoucích vzestupů nutných pro detekci pulzu
  const int min_beat_interval = 300; // minimální interval mezi údery v ms (chrání před falešnými pulzy)

  float reads[samp_siz], sum = 0;
  int ptr = 0;
  float reader, avg;
  float before = 0;
  bool rising = false;
  int rise_count = 0;
  int n;

  unsigned long last_beat = 0;
  int beat_count = 0;
  unsigned long start_time = millis();

  // Inicializace vyhlazovacího pole
  for (int i = 0; i < samp_siz; i++) {
    reads[i] = 0;
  }

  while (millis() - start_time < time_ms) {
    // Vzorkování během 20 ms pro odstranění rušení
    reader = 0;
    n = 0;
    unsigned long t0 = millis();
    do {
      reader += analogRead(pin);
      n++;
    } while (millis() - t0 < 20);
    reader /= n;

    // Vyhlazení
    sum -= reads[ptr];
    sum += reader;
    reads[ptr] = reader;
    ptr = (ptr + 1) % samp_siz;
    avg = sum / samp_siz;

    // Detekce vzestupné hrany
    if (avg > before) {
      rise_count++;
      if (!rising && rise_count > threshold) {
        unsigned long now = millis();
        if (now - last_beat > min_beat_interval) {
          beat_count++;
          last_beat = now;
        }
        rising = true;
      }
    } else {
      rising = false;
      rise_count = 0;
    }

    before = avg;
  }

  // Výpočet BPM podle počtu pulzů a času
  float duration_s = (millis() - start_time) / 1000.0;
  float bpm = (beat_count > 0 && duration_s > 0) ? (beat_count * 60.0 / duration_s) : 0;

  String out = "?type=Heartbeat&id=30&bpm=" + String(bpm);
  if (ResponseAll) globalBuffer += out;
  else Serial.println(out);
}


bool Heartbeat_init(int pin) {
  const int samples = 10;
  int valid_samples = 0;
  for (int i = 0; i < samples; i++) {
    int val = analogRead(pin);
    delay(10);
    if (val > 200 && val < 650) {  // klidně poladit
      valid_samples++;
    }
  }
  return (valid_samples > samples / 2);  // pokud více než polovina vzorků je OK, senzor považujeme za připojený
}


void Heartbeat_reset(){}