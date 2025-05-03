#include <Wire.h>
#include <Adafruit_BMP085.h>
#include "Senzor_BMP180.hpp"

#define BMP085_ADDRESS 0x77

// Vytvoříme globální objekty
TwoWire myWire(0);  // vlastní I2C instance
Adafruit_BMP085 bmp180;

bool Sensor_BMP180_Init(int SDA, int SCL) {
  myWire.begin(SDA, SCL);

  if (!bmp180.begin(BMP085_ADDRESS, &myWire)) {
    Serial.println("BMP180 se nepodařilo inicializovat.");
    return false;
  }

  Serial.println("BMP180 inicializován.");
  return true;
}

void Sensor_BMP180() {
  int korekce = 32;

  long raw = bmp180.readPressure();
  float corrected = (raw + korekce * 100) / 100.0;
  float altitude = bmp180.readAltitude(raw + korekce * 100);

  Serial.print(F("?type=BMP180&id=14&press="));
  Serial.print(corrected);
  Serial.print(F("&altitude="));
  Serial.println(altitude);
}
