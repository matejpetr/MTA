#include <Wire.h>
#include <Adafruit_BMP280.h>

extern TwoWire I2C; 

Adafruit_BMP280 bmp(&I2C);  // Konstruktor s vlastním TwoWire

bool Sensor_BMP280_Init(int SDA, int SCL) {
    I2C.begin(SDA, SCL);  

    if (!bmp.begin(0x76)) {
        return false;
    }
  
       bmp.setSampling(
        Adafruit_BMP280::MODE_NORMAL,
        Adafruit_BMP280::SAMPLING_X2,
        Adafruit_BMP280::SAMPLING_X16,
        Adafruit_BMP280::FILTER_X16,
        Adafruit_BMP280::STANDBY_MS_500);

    return true;  
  }

void Sensor_BMP280() {
    
    float t = bmp.readTemperature();
    float p = ((bmp.readPressure()+32*100)/100)-7;
    float a = bmp.readAltitude(p);
  
    Serial.print(F("?type=BMP180&id=14&press="));
    Serial.print(p);
    Serial.print(F("&altitude="));
    Serial.println(a);
  }
