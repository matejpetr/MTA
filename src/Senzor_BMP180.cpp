#include <Wire.h>
#include <Adafruit_BMP085.h>
#include "Senzor_BMP180.hpp"

#define BMP085_ADDRESS 0x77 //defaultní I2C adresa pro ESP32

TwoWire myWire(0);  // vlastní I2C instance
Adafruit_BMP085 bmp180;

bool Sensor_BMP180_Init(int SDA, int SCL) {
  myWire.begin(SDA, SCL);

  if (!bmp180.begin(BMP085_ADDRESS, &myWire)) {
    return false;
  }
  
  return true;
}

void Sensor_BMP180() {
  int korekce = 32;

  float p = (bmp180.readPressure() + korekce * 100) / 100.0;
  float a = bmp180.readAltitude(bmp180.readPressure() + korekce * 100);

  Serial.print(F("?type=BMP180&id=14&press="));
  Serial.print(p);
  Serial.print(F("&altitude="));
  Serial.println(a);
}
