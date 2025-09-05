#include <Setup.hpp>
#include <Adafruit_BMP085.h>
#include "Senzor_BMP180.hpp"


extern TwoWire I2C;
extern Adafruit_BMP085 bmp180;

bool BMP180_init(int SDA, int SCL) {
  I2C.begin(SDA, SCL);
  return bmp180.begin(0x77, &I2C);
}

void BMP180_update(float gain) {
  float cal_offset =31.5f;  //doladit 
  float pressure_raw = bmp180.readPressure();
  float pressure = (pressure_raw + cal_offset * 100) / 100.0f;

  pressure *= gain;

  float altitude = bmp180.readAltitude(pressure_raw + cal_offset * 100);

  String out = "?type=BMP180&id=10&press=" + String(pressure, 1)+"&altitude="+ String(altitude,1);
  if (ResponseAll) globalBuffer += out;
  else Serial.println(out);
}

void BMP180_reset() {
  bmp180.begin(0x77, &I2C);
}
