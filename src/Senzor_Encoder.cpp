#include "Senzor_Encoder.hpp"

int position = 0;
int pinA_global = 0;
int pinB_global = 0;
bool encoder_initialized = false;

bool Encoder_init(int pinA, int pinB) {
    if (!encoder_initialized) {
        pinA_global = pinA;
        pinB_global = pinB;

        pinMode(pinA_global, INPUT_PULLUP);
        pinMode(pinB_global, INPUT_PULLUP);

        attachInterrupt(digitalPinToInterrupt(pinA), handleEncoder, CHANGE);
        encoder_initialized = true;
    }
    return true;
}

void Encoder_reset() {
    position = 0;
}

void Encoder_update() {
    Serial.print("?type=Encoder&id=29&position=");
    Serial.println(position);
}


void IRAM_ATTR handleEncoder() {
  bool a = digitalRead(pinA_global);
  bool b = digitalRead(pinB_global);
  if (a != b)
    position--;
  else
    position++;
}