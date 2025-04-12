#include <Arduino.h>
#include "Senzor_Heartbeat.hpp"


void Sensor_Heartbeat(int pin, int time){

  int samp_siz = 5;   //samp_siz představuje velikost vzorkovacího okna
  int threshold = 6;  //velikost thesholdu představuje mez, která určuje kdy je detekován skutečný srdeční puls
  
  float reads[samp_siz], sum;
  long int now, ptr;
  float last, reader, start;
  float first, second, third, before, print_value;
  bool rising;
  int rise_count;
  int n;
  long int last_beat;
  const int max_beats = 20;  // Max počet měření za 5 sekund (rezerva)
  float bpm_readings[max_beats];
  int bpm_count = 0;

  for (int i = 0; i < samp_siz; i++)    // Inicializace
    reads[i] = 0;
  sum = 0;
  ptr = 0;
  first = second = third = 0;
  before = 0;
  rising = false;
  rise_count = 0;
  last_beat = millis();

  long int measure_start = millis();  // čas začátku měření
  while (millis() - measure_start < time) {   // Měření po stanovenou dobu
    n = 0;
    start = millis();
    reader = 0.;
    do {
      reader += analogRead(pin);
      n++;
      now = millis();
    } while (now < start + 20);
    reader /= n;       

    sum -= reads[ptr];
    sum += reader;
    reads[ptr] = reader;
    last = sum / samp_siz;

    if (last > before) {
      rise_count++;
      if (!rising && rise_count > threshold) {
        rising = true;
        first = millis() - last_beat;
        last_beat = millis();

        print_value = 60000.0 / (0.4 * first + 0.3 * second + 0.3 * third);   // Výpočet aktuálního BPM

        if (bpm_count < max_beats) {           // Uložení BPM hodnoty do pole
          bpm_readings[bpm_count] = print_value;
          bpm_count++;
        }

        third = second;
        second = first;
      }
    } else {
      rising = false;
      rise_count = 0;
    }
    before = last;

    ptr++;
    ptr %= samp_siz;
  }

  float bpm_sum = 0;  // Výpočet průměrného BPM po 5 sekundách
  for (int i = 0; i < bpm_count; i++) {
    bpm_sum += bpm_readings[i];
  }

  float bpm_avg = (bpm_count > 0) ? (bpm_sum / bpm_count) : 0;

  Serial.print(F("?type=Heartbeat&id=37&bmp="));
  Serial.println(bpm_avg); //Hodnota BPM (údery za minutu) naměřené za 5s
}

