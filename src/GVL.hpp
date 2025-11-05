#include "libs.hpp"

constexpr uint8_t VRx = 15; 
constexpr uint8_t VRy = 7;  
constexpr uint8_t sw = 17;

constexpr uint8_t term1 = 15;   // ADC2_05
constexpr uint8_t term2 = 7;    // ADC2_04
constexpr uint8_t term3 = 4;    // ADC1_03
constexpr uint8_t term4 = 5;    // ADC1_04

// 3v3 i2c
constexpr uint8_t xSDA = 11;    //SDA_01
constexpr uint8_t xSCL = 12;    //SCL_01

constexpr uint8_t MT = 50; // Časové okno ve kterém se měří maximální hodnota z mikrofonu (ms)

OneWire oneWire(term1);
DallasTemperature sensors(&oneWire);

Adafruit_BMP280 bmp(&I2C);
Adafruit_BMP085 bmp180;
