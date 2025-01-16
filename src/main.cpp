#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>

String* parseGET(String queryString);

void Sensor_DHT11();
#include <dht11.h>
DHT11 dht11(1);

void Sensor_DS11B20();
#define ONE_WIRE_BUS 2  // Pin pro připojení datového vodiče senzoru
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void Sensor_AnalogRead(int pinNo);

void Sensor_DigitalRead(int pinNo);

void Specific_Sensor_AnalogRead(int analogPin, String typeOfSensor,int sensorID);

void Specific_Sensor_DigitalRead(int digitalPin, String typeOfSensor,int sensorID);



//I2C.............................................

#define SDASCL 16,17
//GY_521
void Sensor_GY_521();
#define GY_521ADDRESS 0x68
#define GY_521POWER 0x6B
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
int32_t t_fine;
int16_t calib_T1, calib_T2, calib_T3;
int16_t calib_P1, calib_P2, calib_P3, calib_P4, calib_P5, calib_P6, calib_P7, calib_P8, calib_P9;
//..................................................

void setup() {
  Serial.begin(115200);
  Serial.println("M-TA: Sensor board ready!");
}

void loop() {

  int sensorID = 0;
  int pinNo = 0;
  String type = "";
  delay(1000);

  // Příklad GET dotazu
  //String exampleQuery = "type=dht11&id=0";
  //String exampleQuery = "type=ds18b20&id=1";
  //String exampleQuery = "type=DigitalRead&id=100&pinNo=5;";
  //String exampleQuery = "type=AnalogRead&id=200&pinNo=5;";
  //String exampleQuery = "type=Ahall&id=4&pinNo=2";
  String exampleQuery = "type=Dhall&id=3&pinNo=2";
  //String exampleQuery = "type=PInterrupt&id=";
  //String exampleQuery = "type=&id=";

  // Parsování příkladu
  String* result = parseGET(exampleQuery);

  // Zobrazení výsledného pole
  //Serial.println("--- Parsed Parameters ---");
  //Serial.println("Type: "       + result[0]);
  //Serial.println("SensorID: "   + result[1]);
  //Serial.println("Resolution: " + result[2]);
  //Serial.println("pinNo: "      + result[3]);

  // Call of specific sensor
  type = result[0]; //Type
  sensorID = result[1].toInt(); // SensorID
  pinNo    = result[3].toInt(); // pinNo

  switch (sensorID) {
    case 0: // Sensor: 
      Sensor_DHT11();
      break;
    case 1: // Sensor:
      Sensor_DS11B20();
      break;
    case 3: // Digital Hall Sensor:
      Specific_Sensor_DigitalRead(pinNo,type,sensorID);
      break;
    case 4: // Analog Hall Sensor:
      Specific_Sensor_AnalogRead(pinNo,type,sensorID);
      break;
    case 100: // Universal Diagital Input Read; type=DigitalRead&id=100&pinNo=x;
      Sensor_DigitalRead(pinNo);
      break;
    case 200: // Universal Analog input Read; type=AnalogRead&id=100&pinNo=x;
      Sensor_AnalogRead(pinNo);
      break;
    default: // If the command is not recognized, do nothing
      Serial.println("?id=" + String(sensorID) + "&message=NotSupported");
      sensorID = -1;
      break;
  }

}

void Specific_Sensor_AnalogRead(int analogPin, String typeOfSensor,int sensorID)
{
  pinMode(analogPin,INPUT);
  int analogValue = analogRead(analogPin);
  Serial.print(F("?type="));
  Serial.print(typeOfSensor);
  Serial.print(F("&id="));
  Serial.print(sensorID);
  Serial.print(F("&value="));
  Serial.println(analogValue);
}

void Specific_Sensor_DigitalRead(int digitalPin, String typeOfSensor,int sensorID)
{
  pinMode(digitalPin, INPUT);
  int digitalValue = digitalRead(digitalPin);
  Serial.print(F("?type="));
  Serial.print(typeOfSensor);
  Serial.print(F("&id="));
  Serial.print(sensorID);
  Serial.print(F("&value="));
  Serial.println(digitalValue);
}

void Sensor_AnalogRead(int analogPin)
{
  int analogValue = analogRead(analogPin);
  Serial.print(F("?type=AnalogRead&id=100&value="));
  Serial.println(analogValue);
}

void Sensor_DigitalRead(int digitalPin)
{
  pinMode(digitalPin, INPUT);
  int digitalValue = digitalRead(digitalPin);
  Serial.print(F("?type=DigitalRead&id=200&value="));
  Serial.println(digitalValue);
}

void Sensor_DS11B20()
{
  sensors.requestTemperatures();  // Požádej senzory o teplotu
  float temperature = sensors.getTempCByIndex(0);  // Čti první senzor
  Serial.print(F("?type=ds11b20&id=0&temp="));
  Serial.println(temperature);
}

void Sensor_DHT11()
{
  float h = dht11.readHumidity();
  float t = dht11.readTemperature();
  //float f = dht.readTemperature(true); // Read temperature as Fahrenheit (isFahrenheit = true)
  //float hif = dht.computeHeatIndex(f, h); // Compute heat index in Fahrenheit (the default)
  //float hic = dht.computeHeatIndex(t, h, false); // Compute heat index in Celsius (isFahreheit = false)
  Serial.print(F("?type=dht11&id=0&humi="));
  Serial.print(h);
  Serial.print(F("&temp="));
  Serial.println(t);
}


// Funkce pro parsování GET dotazu
String* parseGET(String queryString) {
  static String values[50]; // Pole pro ukládání hodnot: type, temp, humi

  // Inicializace prázdných hodnot
  values[0] = ""; // type
  values[1] = ""; // id
  values[2] = ""; // resolution

  // Rozdělení na jednotlivé klíč=hodnota páry
  int start = 0;
  while (start < queryString.length()) {
    int end = queryString.indexOf('&', start);
    if (end == -1) {
      end = queryString.length();
    }

    String pair = queryString.substring(start, end);
    int delimiter = pair.indexOf('=');

    if (delimiter != -1) {
      String key = pair.substring(0, delimiter);
      String value = pair.substring(delimiter + 1);

      // Uložení hodnot na základě klíče
      if (key == "type") {
        values[0] = value;
      } else if (key == "id") {
        values[1] = value;
      } else if (key == "resolution") {
        values[2] = value;
      } else if (key == "pinNo") {
        values[3] = value;
      }
    }

    start = end + 1;
  }

  return values;
}


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
