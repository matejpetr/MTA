#include <Setup.hpp>
#include <Wire.h>
#include <Adafruit_BMP280.h>

extern TwoWire I2C;
extern Adafruit_BMP280 bmp;

bool BMP280_init(int SDA, int SCL) {
    I2C.begin(SDA, SCL);
    return bmp.begin(0x76);
}

void BMP280_config(int os_temp, int os_press, int filter) {
  Adafruit_BMP280::sensor_sampling temp_os = Adafruit_BMP280::SAMPLING_X1;
  Adafruit_BMP280::sensor_sampling press_os = Adafruit_BMP280::SAMPLING_X1;
  Adafruit_BMP280::sensor_filter filter_enum = Adafruit_BMP280::FILTER_OFF;

  if (os_temp == 2) temp_os = Adafruit_BMP280::SAMPLING_X2;
  else if (os_temp == 4) temp_os = Adafruit_BMP280::SAMPLING_X4;
  else if (os_temp == 8) temp_os = Adafruit_BMP280::SAMPLING_X8;
  else if (os_temp >= 16) temp_os = Adafruit_BMP280::SAMPLING_X16;

  if (os_press == 2) press_os = Adafruit_BMP280::SAMPLING_X2;
  else if (os_press == 4) press_os = Adafruit_BMP280::SAMPLING_X4;
  else if (os_press == 8) press_os = Adafruit_BMP280::SAMPLING_X8;
  else if (os_press >= 16) press_os = Adafruit_BMP280::SAMPLING_X16;

  if (filter == 1) filter_enum = Adafruit_BMP280::FILTER_X2;
  else if (filter == 2) filter_enum = Adafruit_BMP280::FILTER_X4;
  else if (filter == 3) filter_enum = Adafruit_BMP280::FILTER_X8;
  else if (filter >= 4) filter_enum = Adafruit_BMP280::FILTER_X16;

  bmp.setSampling(
    Adafruit_BMP280::MODE_NORMAL,
    temp_os,
    press_os,
    filter_enum,
    Adafruit_BMP280::STANDBY_MS_500
  );
}


void BMP280_update() {
  float cal = 0;           //kalibrační koeficient UPRAVIT!!!
  float t = bmp.readTemperature();
  float p = (bmp.readPressure() / 100) + cal;
  float a = bmp.readAltitude(p);

  String out = "?type=BMP180&id=9&temp=" + String(t, 1) + "&press=" + String(p, 1);
  if (ResponseAll) globalBuffer += out;
  else Serial.println(out);
}

void BMP280_reset() {
  bmp.begin(0x76);
}
