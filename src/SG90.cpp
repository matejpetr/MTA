#include "SG90.hpp"

// Interní pomocná funkce pro „kompatibilní“ volání mimo třídu.
// Nepoužívá členské servo – má svůj statický objekt,
// aby se nebilo s instancemi SG90 v poli aktuátorů.
static Servo s_globalServo;
static int   s_lastAngle = 0;

static inline int speedToDelayMs_(int speed) {
  speed = constrain(speed, 0, 100);
  return map(speed, 0, 100, 100, 0);
}

static void control_(Servo &s, int start, int end, int speedMs) {
  start = constrain(start, 0, 180);
  end   = constrain(end,   0, 180);
  if (speedMs <= 0) { s.write(end); return; }
  int step = (start < end) ? 1 : -1;
  for (int pos = start; pos != end; pos += step) {
    s.write(pos);
    delay(speedMs);
  }
  s.write(end);
}

// Volná funkce (kompatibilita se starým kódem)
void SG90_config(int pin, int angle, int speed) {
  angle = constrain(angle, 0, 180);
  int speedMs = speedToDelayMs_(speed);

  if (!s_globalServo.attached()) s_globalServo.attach(pin);
  else {
    // pokud je na jiném pinu, přepni
    // (Servo knihovna nepodporuje přímo getPin, tak odpojíme a znovu připojíme)
    s_globalServo.detach();
    s_globalServo.attach(pin);
  }

  if (angle != s_lastAngle) {
    control_(s_globalServo, s_lastAngle, angle, speedMs);
    s_lastAngle = angle;
  } else {
    s_globalServo.write(angle);
  }
}
