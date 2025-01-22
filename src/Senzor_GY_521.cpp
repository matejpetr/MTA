#include "Senzor_GY_521.hpp"
#include <Arduino.h>

void Sensor_GY_521()
{
  
 Wire.beginTransmission(GY_521ADDRESS);
 Wire.write(GY_521POWER);
 //set to 0 to turn on | set to 0x40 to sleep
 Wire.write(0);
 Wire.endTransmission(true);
 //getting data
  Wire.beginTransmission(GY_521ADDRESS);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(GY_521ADDRESS,14,true);
  int16_t acm_x=Wire.read()<<8 | Wire.read();
  int16_t acm_y=Wire.read()<<8 | Wire.read();
  int16_t acm_z=Wire.read()<<8 | Wire.read();
  float temperatureCelsius=(Wire.read()<<8 | Wire.read())/ 340.0 + 35.00;
  int16_t gyr_x=Wire.read()<<8 | Wire.read();
  int16_t gyr_y=Wire.read()<<8 | Wire.read();
  int16_t gyr_z=Wire.read()<<8 | Wire.read();

 Wire.beginTransmission(GY_521ADDRESS);
 Wire.write(GY_521POWER);
 //set to 0 to turn on | set to 0x40 to sleep
 Wire.write(0x40);
 Wire.endTransmission(true);
}