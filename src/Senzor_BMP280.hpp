#include <Wire.h>


//GY_BMP280
void Sensor_GY_BMP280RealValue(int32_t rPres,int32_t rTemp);
void Sensor_GY_BMP280();
#define GY_BMP280ADDRESS 0x76
#define GY_BMP280_POWERMODE 0xF4
#define GY_BMP280_TMP 0xFA
#define GY_BMP280_PRESS 0xF7
//sanpling settings
#define GY_BMP280_REG_CONFIG 0xF5
//calibration 
void Sensor_GY_BMP280CALIB();

//..................................................