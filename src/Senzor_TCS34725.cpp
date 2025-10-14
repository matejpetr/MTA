#include "Senzor_TCS34725.hpp"

// Pokud je nemáš globálně jinde, můžou být definované zde:
TwoWire I2C(0);
Adafruit_TCS34725 tcs(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_4X);

static uint8_t mapItime_(int itime_ms) {
  switch (itime_ms) {
    case 2:   return TCS34725_INTEGRATIONTIME_2_4MS;
    case 50:  return TCS34725_INTEGRATIONTIME_50MS;
    case 101: return TCS34725_INTEGRATIONTIME_101MS;
    case 199: return TCS34725_INTEGRATIONTIME_199MS;
    case 300: return TCS34725_INTEGRATIONTIME_300MS;
    case 401: return TCS34725_INTEGRATIONTIME_401MS;
    case 499: return TCS34725_INTEGRATIONTIME_499MS;
    case 600: default: return TCS34725_INTEGRATIONTIME_600MS;
  }
}
static tcs34725Gain_t mapGain_(int g) {
  switch (g) {
    case 1:  return TCS34725_GAIN_1X;
    case 4:  return TCS34725_GAIN_4X;
    case 16: return TCS34725_GAIN_16X;
    case 60: default: return TCS34725_GAIN_60X;
  }
}

bool TCS34725::init() {
  I2C.begin(_sda, _scl);
  if (!tcs.begin(0x29, &I2C)) return false;
  applyConfig_();         // po initu rovnou aplikuj poslední config
  return true;
}

void TCS34725::reset() {
  // reinit na stejné sběrnici/adrese
  tcs.begin(0x29, &I2C);
  applyConfig_();
}

void TCS34725::applyConfig_() {
  // mapování parametrů na enumy knihovny
  tcs.setIntegrationTime(mapItime_(_itime));
  tcs.setGain(mapGain_(_gain));
}

std::vector<KV> TCS34725::update() {
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);
  if (c == 0) c = 1; // ochrana před dělením nulou

  // normalizace a korekce (zachováno z původního kódu)
  float rn = (float)r / (c + 1);
  float gn = (float)g / (c + 1);
  float bn = (float)b / (c + 1);

  const float R_CORR = 0.7f;
  const float G_CORR = 1.1f;
  const float B_CORR = 1.7f;

  rn *= R_CORR; gn *= G_CORR; bn *= B_CORR;

  float maxRGB = rn;
  if (gn > maxRGB) maxRGB = gn;
  if (bn > maxRGB) maxRGB = bn;
  if (maxRGB > 1.0f) {
    rn /= maxRGB; gn /= maxRGB; bn /= maxRGB;
  }

  const uint8_t r8 = (uint8_t)constrain(rn * 255.0f, 0.0f, 255.0f);
  const uint8_t g8 = (uint8_t)constrain(gn * 255.0f, 0.0f, 255.0f);
  const uint8_t b8 = (uint8_t)constrain(bn * 255.0f, 0.0f, 255.0f);

  std::vector<KV> kv;
  kv.push_back({"R", String(r8)});
  kv.push_back({"G", String(g8)});
  kv.push_back({"B", String(b8)});
  return kv;
}
