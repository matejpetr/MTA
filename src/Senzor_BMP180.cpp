#include "Senzor_BMP180.hpp"
#include <Adafruit_BMP085.h>

// Sdílíš globální instanci sběrnice/senzoru tak jako doteď:
extern TwoWire I2C;
extern Adafruit_BMP085 bmp180;

bool BMP180::init() {
  I2C.begin(_sda, _scl);
  return bmp180.begin(0x77, &I2C);
}

void BMP180::reset() {
  // re-init senzoru na stejné adrese a sběrnici
  bmp180.begin(0x77, &I2C);
}

std::vector<KV> BMP180::update() {
  // čtení surového tlaku (Pa)
  const float pressure_raw = bmp180.readPressure();

  // kalibrace jako v původním kódu:
  // (pressure_raw + CAL_OFFSET*100) / 100.0 -> mbar (hPa)
  float pressure = (pressure_raw + CAL_OFFSET * 100.0f) / 100.0f;

  // aplikuj gain
  pressure *= _gain;

  // výpočet nadmořské výšky – knihovna očekává tlak v Pa
  const float altitude = bmp180.readAltitude(pressure_raw + CAL_OFFSET * 100.0f);

  // vrať data jako KVs (žádné query-stringy)
  std::vector<KV> kv;
  kv.push_back({"press",    String(pressure, 1)});   // hPa
  kv.push_back({"altitude", String(altitude, 1)});   // m
  return kv;
}
