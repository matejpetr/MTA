#include "SG90.hpp"

// interní statický pin pro volné funkce
static int s_pin = -1;

// Interní pomocná servo instance (pro volnou funkci kompatibility)
static Servo s_globalServo;
static int   s_lastAngle = 0;

void SG90_setPin(int pin) {
  s_pin = pin;
}

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

// Volná funkce (kompatibilita) — používá pin nastavený v attach() přes SG90_setPin()
void SG90_config(int angle, int speed) {
  if (s_pin < 0) return;            // pin musí být nastaven attach()
  angle = constrain(angle, 0, 180);
  int speedMs = speedToDelayMs_(speed);

  if (!s_globalServo.attached()) s_globalServo.attach(s_pin);
  else {
    // pokud je na jiném pinu, přepni
    s_globalServo.detach();
    s_globalServo.attach(s_pin);
  }

  if (angle != s_lastAngle) {
    control_(s_globalServo, s_lastAngle, angle, speedMs);
    s_lastAngle = angle;
  } else {
    s_globalServo.write(angle);
  }
}