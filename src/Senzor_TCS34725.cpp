#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include "Senzor_TCS34725.hpp"


extern Adafruit_TCS34725 tcs;
extern TwoWire I2C; 

bool TCS34725_init(int SDA, int SCL) {
  I2C.begin(SDA, SCL);       
  if (!tcs.begin(0x29,&I2C)) {    
   
    return false;
  }
  return true;
} 


void TCS34725_update() {
  uint16_t r, g, b, c;

  tcs.getRawData(&r, &g, &b, &c);
  if (c == 0) c = 1;  // ochrana před dělením nulou

  float r_norm = (float)r / (c + 1);
  float g_norm = (float)g / (c + 1);
  float b_norm = (float)b / (c + 1);

  // Korekční koeficienty senzoru (přibližné)
  const float R_CORR = 0.6;   
  const float G_CORR = 1.0;   
  const float B_CORR = 1.4;   

  r_norm *= R_CORR;
  g_norm *= G_CORR;
  b_norm *= B_CORR;

  // Ořezání a převod na 0–255
  float maxRGB = max(r_norm, max(g_norm, b_norm));
  if (maxRGB > 1.0) {  // Normalizace nad 1
    r_norm /= maxRGB;
    g_norm /= maxRGB;
    b_norm /= maxRGB;
  }

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

void TCS34725_reset(){
  tcs.begin(0x29,&I2C);
}