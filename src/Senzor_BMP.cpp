#include "Senzor_BMP.hpp"
#include <Wire.h>
#include <Adafruit_BMP280.h>

extern TwoWire I2C;              // používáš v projektu
extern Adafruit_BMP280 bmp;      // sdílená instance jako doteď

// --- pomocné mapování nastavení ---
Adafruit_BMP280::sensor_sampling BMP280::mapOs(int v) {
  if (v >= 16) return Adafruit_BMP280::SAMPLING_X16;
  if (v >= 8)  return Adafruit_BMP280::SAMPLING_X8;
  if (v >= 4)  return Adafruit_BMP280::SAMPLING_X4;
  if (v >= 2)  return Adafruit_BMP280::SAMPLING_X2;
  return Adafruit_BMP280::SAMPLING_X1;
}
Adafruit_BMP280::sensor_filter BMP280::mapFilter(int v) {
  if (v >= 16) return Adafruit_BMP280::FILTER_X16;
  if (v >= 8)  return Adafruit_BMP280::FILTER_X8;
  if (v >= 4)  return Adafruit_BMP280::FILTER_X4;
  if (v >= 2)  return Adafruit_BMP280::FILTER_X2;
  return Adafruit_BMP280::FILTER_OFF;
}

// --- životní cyklus ---
bool BMP280::init() {
  // I²C piny podle CONNECT
  I2C.begin(_sda, _scl);
  // knihovna pro BMP280 – v původním kódu se volal begin(0x76) bez I2C parametru,
  // zachovám stejné chování kvůli kompatibilitě
  return bmp.begin(0x76);
}

void BMP280::reset() {
  bmp.begin(0x76);
}

void BMP280::config(Param* params, int count) {
  for (int i = 0; i < count; ++i) {
    if      (params[i].key == "Os_temp")  _os_temp  = params[i].value.toInt();
    else if (params[i].key == "Os_press") _os_press = params[i].value.toInt();
    else if (params[i].key == "Filter")   _filter   = params[i].value.toInt();
  }

  bmp.setSampling(
    Adafruit_BMP280::MODE_NORMAL,
    mapOs(_os_temp),
    mapOs(_os_press),
    mapFilter(_filter),
    Adafruit_BMP280::STANDBY_MS_500
  );
}

// --- měření ---
std::vector<KV> BMP280::update() {
  // pozn.: původní kód měl kalibrační konstantu `cal = 0`; zachovávám
  const float t = bmp.readTemperature();           // °C
  const float p = bmp.readPressure() / 100.0f;     // Pa -> hPa

  std::vector<KV> kv;
  kv.push_back({"temp",  String(t, 1)});
  kv.push_back({"press", String(p, 1)});
  return kv;
}
