#include "libs.hpp"

constexpr uint8_t VRx = 15; // Číslo pinu (ADC2_05) prvního senzorického terminálu na HW standu (4pinové červené)
constexpr uint8_t VRy = 7;  // Číslo pinu (ADC2_04) druhého senzorického terminálu na HW standu (3pinové černé)
constexpr uint8_t sw = 17;

constexpr uint8_t term1 = 15;
constexpr uint8_t term2 = 7;
constexpr uint8_t term3 = 4;
constexpr uint8_t term4 = 5;

// 3v3 i2c
constexpr uint8_t xSDA = 11;
constexpr uint8_t xSCL = 12;

// 5v i2c
//constexpr uint8_t xSDA = 10;
// constexpr uint8_t xSCL  neexistuje

constexpr uint8_t MT = 50; // Measuring time pro Mic (ms)

OneWire oneWire(term1);
DallasTemperature sensors(&oneWire);

DHT dht(term2, DHT11);
Adafruit_BMP280 bmp(&I2C);
Adafruit_BMP085 bmp180;
Servo myServo;