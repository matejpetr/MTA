#include "Senzor_BMP280.hpp"
#include <Arduino.h>
#include "OneWire.h"

int32_t t_fine;
int16_t calib_T1, calib_T2, calib_T3;
int16_t calib_P1, calib_P2, calib_P3, calib_P4, calib_P5, calib_P6, calib_P7, calib_P8, calib_P9;

void Sensor_GY_BMP280CALIB(){
 Wire.beginTransmission(GY_BMP280ADDRESS);
 Wire.write(0x88);   
 Wire.endTransmission();
 Wire.requestFrom(GY_BMP280ADDRESS, 24);

 calib_T1 = Wire.read() | (Wire.read() << 8);
 calib_T2 = Wire.read() | (Wire.read() << 8);
 calib_T3 = Wire.read() | (Wire.read() << 8);
 calib_P1 = Wire.read() | (Wire.read() << 8);
 calib_P2 = Wire.read() | (Wire.read() << 8);
 calib_P3 = Wire.read() | (Wire.read() << 8);
 calib_P4 = Wire.read() | (Wire.read() << 8);
 calib_P5 = Wire.read() | (Wire.read() << 8);
 calib_P6 = Wire.read() | (Wire.read() << 8);
 calib_P7 = Wire.read() | (Wire.read() << 8);
 calib_P8 = Wire.read() | (Wire.read() << 8);
 calib_P9 = Wire.read() | (Wire.read() << 8);
}

void Sensor_GY_BMP280RealValue(int32_t rPres,int32_t rTemp)
{
  int32_t var = ((((rTemp >> 3) - ((int32_t)calib_T1 << 1))) * ((int32_t)calib_T2)) >> 11;
  int32_t var0 = (((((rTemp >> 4) - ((int32_t)calib_T1)) * ((rTemp >> 4) - ((int32_t)calib_T1))) >> 12) * ((int32_t)calib_T3)) >> 14;
  t_fine = var + var0; // t_fine is an intermediate value for temperature calculation


float t = (t_fine * 5 + 128) >> 8;  // t_fine is in 0.01 degrees Celsius
t /= 100.0; // Convert to °C

   int64_t var1 = ((int64_t)t_fine) - 128000;
int64_t var2 = var1 * var1 * (int64_t)calib_P6;
var2 = var2 + ((var1 * (int64_t)calib_P5) << 17);
var2 = var2 + (((int64_t)calib_P4) << 35);
var1 = ((var1 * var1 * (int64_t)calib_P3) >> 8) + ((var1 * (int64_t)calib_P2) << 12);
var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)calib_P1) >> 33;


int64_t p = 1048576 - rPres;  
p = (((p<<31)-var2)*3125)/var1;
var1 = (((int64_t)calib_P9) * (p>>13)*(p>>13)) >>25;
var2= (((int64_t)calib_P8) *p)>>19;
p = ((p + var1+var2)>>8)+(((int64_t)calib_P7)<<4);


// Convert p to hPa (final pressure in hPa)
p = p / 256.0/100 ;  

 
}
void Sensor_GY_BMP280()
{


 //mode
 Wire.beginTransmission(GY_BMP280ADDRESS);
 Wire.write(GY_BMP280_POWERMODE);
 //defines imaging and power mode
 Wire.write(10001001);
 Wire.endTransmission(true);
 // forced mode mesurment mesurses data and turns to sleep mode after a while



   Wire.beginTransmission(GY_BMP280ADDRESS);
  Wire.write(GY_BMP280_REG_CONFIG);
  Wire.write(1);  // defines filter
  Wire.endTransmission();
 //getting data
 delay(100);
   Wire.beginTransmission(GY_BMP280ADDRESS);
  Wire.write(GY_BMP280_PRESS);  // Starting from presure register 
  Wire.endTransmission();
  Wire.requestFrom(GY_BMP280ADDRESS, 6);
  int32_t rawPress = Wire.read() << 12 | Wire.read() << 4 | Wire.read() >> 4;
  int32_t rawTemp = Wire.read() << 12 | Wire.read() << 4 | Wire.read() >> 4;
  Sensor_GY_BMP280RealValue(rawPress,rawTemp);

}
