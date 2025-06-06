#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include "Senzor_TCS34725.hpp"


Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);
extern TwoWire I2C; 

bool Sensor_TCS34725_Init(int SDA, int SCL) {
  I2C.begin(SDA, SCL);       
  if (!tcs.begin(0x29,&I2C)) {    
   
    return false;
  }
  return true;

} 

void Sensor_TCS34725(int SDA, int SCL) {
  uint16_t r, g, b, c;

  tcs.getRawData(&r, &g, &b, &c);

  float r_norm = (float)r / c;
  float g_norm = (float)g / c;
  float b_norm = (float)b / c;

  uint8_t r8 = constrain(r_norm * 255.0, 0, 255);
  uint8_t g8 = constrain(g_norm * 255.0, 0, 255);
  uint8_t b8 = constrain(b_norm * 255.0, 0, 255);

  Serial.print(F("?type=TCS34725&id=11&R="));
  Serial.print(r8,DEC);
  Serial.print(F("&G="));
  Serial.println(g8,DEC);
  Serial.print(F("&B="));
  Serial.println(b8,DEC);


}