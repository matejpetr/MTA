#include "Senzor_Joystick.hpp"

void Joystick::calibrateCenter_(int res) {
  analogReadResolution(res);
  _maxADC = (1UL << res) - 1;

#ifdef ESP32
  // doporučená atenuace pro plný rozsah 0–3.3 V
  analogSetPinAttenuation(_x, ADC_11db);
  analogSetPinAttenuation(_y, ADC_11db);
#endif

  long sx = 0, sy = 0;
  const int N = 16;
  for (int i = 0; i < N; ++i) {
    sx += analogRead(_x);
    sy += analogRead(_y);
    delay(2);
  }
  _cx = (int)(sx / N);
  _cy = (int)(sy / N);

  _prevRes   = res;
  _calibrated = true;
}

int Joystick::pctToAdcTol_(int pct) const {
  int p = pct;
  if (p < 0)   p = 0;
  if (p > 100) p = 100;
  return (int)((p * (long)_maxADC) / 100L);
}

bool Joystick::init() {
  // první nastavení pinů
  /*if (!_pinsInited) {
    pinMode(_sw, INPUT_PULLUP);
    _pinsInited = true;
  }
  

  // kalibrace středu (pokud ještě nebyla)
  calibrateCenter_((_prevRes > 0) ? _prevRes : _res);

  // hrubá sanity-check: hodnoty v rozsahu ADC → OK
  const int xv = analogRead(_x);
  const int yv = analogRead(_y);
  return (xv >= 0 && yv >= 0);
  */
 return true;
}

std::vector<KV> Joystick::update() {
  if (!_pinsInited) {
    pinMode(_sw, INPUT_PULLUP);
    _pinsInited = true;
  }

  // kalibrace při prvním použití nebo při změně rozlišení
  if (!_calibrated || _res != _prevRes) {
    calibrateCenter_(_res);   // nech páčku uprostřed
  } else {
    analogReadResolution(_res);
  }

  const int xValue = analogRead(_x);
  const int yValue = analogRead(_y);
  const bool click = (digitalRead(_sw) == LOW);

  const int tol = pctToAdcTol_(_threshold);
  int dx = xValue - _cx;
  int dy = yValue - _cy;

  // mrtvá zóna
  if (abs(dx) < tol) dx = 0;
  if (abs(dy) < tol) dy = 0;

  // určení směru – přesně podle původního kódu (zachováno i s mapováním os)
  String direction;
  if (click) {
    direction = "CLICK";
  } else if (dx == 0 && dy == 0) {
    direction = "CENTER";
  } else if (abs(dy) >= abs(dx)) {
    direction = (dy < 0) ? "LEFT" : "RIGHT";
  } else {
    direction = (dx > 0) ? "UP" : "DOWN";
  }

  return { {"direction", direction} };
}
