#include <Wire.h>
#include <Adafruit_BMP280.h>

extern TwoWire I2C; 
extern Adafruit_BMP280 bmp;  // Konstruktor s vlastním TwoWire


bool BMP280_init(int SDA, int SCL) {
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

void BMP280_update() {
  
    float cal = 42.8; //kalibrační koeficient - NUTNO upravit !!
    float t = bmp.readTemperature();
    //float p = ((bmp.readPressure()+32*100)/100)-7;
    float p = (bmp.readPressure()/100)+cal;    
    float a = bmp.readAltitude(p);
  
    Serial.print(F("?type=BMP180&id=14&press="));
    Serial.print(p);
    Serial.print(F("&altitude="));
    Serial.println(a);
  }

  void BMP280_reset(){
    bmp.begin(0x76);  
  }
