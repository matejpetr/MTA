#include <Setup.hpp>
#include "Senzor_Joystick.hpp"

namespace {
  // jednoduchý stav kalibrace uložený v .cpp
  bool     g_calibrated = false;
  int      g_cx = 0, g_cy = 0;          // zkalibrovaný střed os
  int      g_prevRes = -1;              // naposledy použité rozlišení
  uint32_t g_maxADC  = 4095;            // podle res

  void calibrateCenter(int pinX, int pinY, int res) {
    // nastavení rozlišení a hranic
    analogReadResolution(res);
    g_maxADC = (1UL << res) - 1;

    #ifdef ESP32
      // pro plný rozsah 0–3.3 V (doporučeno u ESP32)
      analogSetPinAttenuation(pinX, ADC_11db);
      analogSetPinAttenuation(pinY, ADC_11db);
    #endif

    // načti několik vzorků a zprůměruj
    long sx = 0, sy = 0;
    const int N = 16;
    for (int i = 0; i < N; ++i) {
      sx += analogRead(pinX);
      sy += analogRead(pinY);
      delay(2);
    }
    g_cx = (int)(sx / N);
    g_cy = (int)(sy / N);

    g_prevRes    = res;
    g_calibrated = true;
  }

  // převod procent na ADC kroky (ochrana proti přetečení a mimo rozsah)
  inline int pctToAdcTol(int pct) {
    if (pct < 0)   pct = 0;
    if (pct > 100) pct = 100;
    return (int)((pct * (long)g_maxADC) / 100L);
  }
}

// ============= API =============

void Joystick_update(int x, int y, int sw, int res, int thresholdPct) {
  static bool pinsInited = false;
  if (!pinsInited) {
    pinMode(sw, INPUT_PULLUP);
    pinsInited = true;
  }

  // kalibrace při prvním volání nebo při změně rozlišení
  if (!g_calibrated || res != g_prevRes) {
    calibrateCenter(x, y, res);   // nech páčku uprostřed
  } else {
    analogReadResolution(res);    // res můžeš měnit dynamicky
  }

  const int xValue  = analogRead(x);
  const int yValue  = analogRead(y);
  const bool click  = (digitalRead(sw) == LOW);

  const int tol     = pctToAdcTol(thresholdPct);
  int dx = xValue - g_cx;
  int dy = yValue - g_cy;

  // mrtvá zóna
  if (abs(dx) < tol) dx = 0;
  if (abs(dy) < tol) dy = 0;

  // určení směru – vezmeme osu s větší odchylkou
  String direction;
  if (click) {
    direction = "CLICK";
  } else if (dx == 0 && dy == 0) {
    direction = "CENTER";
  } else if (abs(dy) >= abs(dx)) {
    // běžně UP = menší Y (nahoru snižuje napětí)
     direction = (dy < 0) ? "LEFT" : "RIGHT";
  } else {
    direction = (dx > 0) ? "UP" : "DOWN";
  }


  String out = "?type=Joystick&id=16&direction=" + direction;

  if (ResponseAll) globalBuffer += out;
  else Serial.println(out);


}

bool Joystick_init(int x, int y) {
  
  calibrateCenter(x, y, (g_prevRes > 0) ? g_prevRes : 12);
  // hrubá kontrola – pokud čteme čísla v rozsahu ADC, považuj za OK
  const int xv = analogRead(x);
  const int yv = analogRead(y);
  return (xv >= 0 && yv >= 0);
}

void Joystick_reset() {
  // vynutí novou kalibraci při nejbližším update()
  g_calibrated = false;
}
