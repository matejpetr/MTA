#include <Arduino.h>
#include "Parser.hpp"

#include "Senzor_DS18B20.hpp"
#include "Senzor_DHT11.hpp"
//#include "Ahall.hpp"    
#include "Senzor_GY_521.hpp"
//#include "HCSR04.hpp"   
//#include "HCSR501.hpp"
#include "Senzor_BMP280.hpp"
//include "Antc.hpp"
//include "PIresistance.hpp"
//include "Joystick.hpp"
//include "HallLin.hpp"
//include "MQ135.hpp"
//include "IndPNP.hpp"
//include "IndNPN.hpp"
//include "AMoisture.hpp"
//include "TTP223.hpp"
//include "AJSR04M.hpp"
//include "GP2Y0A21YK0F.hpp"
//include "Encoder.hpp"
//include "HS0038DB.hpp"
//include "MicSmall.hpp"
//include "MicBig.hpp"
//include "Heartbeat.hpp"
#include "Senzor_AnalogRead.hpp"
#include "Senzor_DigitalRead.hpp"


OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

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
  String exampleQuery = "type=dht11&id=0";
  //String exampleQuery = "type=ds18b20&id=1";
  //String exampleQuery = "type=DigitalRead&id=100&pinNo=5;";
  //String exampleQuery = "type=AnalogRead&id=200&pinNo=5;";
  //String exampleQuery = "type=Ahall&id=4&pinNo=2";
  //String exampleQuery = "type=Dhall&id=3&pinNo=2";
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
    case 0: // DS18B20
      Sensor_DS18B20(sensors);
      break;
    case 1: // DHT11
      Sensor_DHT11(oneWire);
      break;
    case 3: // DHall Sensor:
      Sensor_DigitalRead(pinNo);
      break;
    case 4: // AHall Sensor:
      //Sensor_Ahall();
      break;
    case 5: // GY521
      Sensor_GY_521();
      break;
    case 6: // FC51
      //Sensor_FC51();
      break;
    case 7: // HCSR04
      //Sensor_HCSR04();
      break;
    case 8: // HCSR501
      //Sensor_HCSR501();
      break;
    case 9: // KW113Z
      //Sensor_KW113Z();
      break;
    case 10: // BMP280
      Sensor_GY_BMP280();
      break;
    case 11: // TCS34725
      //Sensor_TCS34725();
      break;
    case 12: // VL53L0X
      //Sensor_VL53L0X();
      break;
    case 13: // InfraredR
      //Sensor_InfraredR();
      break;
    case 14: // InfraredE
      //Sensor_InfraredE();
      break;
    case 15: // Dntc
      //Sensor_Dntc();
      break;
    case 16: // Antc
      //Sensor_Antc();
      break;
    case 17: // PIresistance
      //Sensor_PIresistance();
      break;
    case 18: // Joystick
      //Sensor_Joystick();
      break;
    case 19: // HallLin
      //Sensor_HallLin();
      break;
    case 20: // MQ135
      //Sensor_MQ135();
      break;
    case 21: //IndPNP
      //Sensor_IndPNP();
      break;
    case 22: // IndNPN
      //Sensor_IndNPN();
      break;
    case 23: // Dmoisture
      //Sensor_Dmoisture();
      break;
    case 24: // AMoisture
      //Sensor_AMoisture();
      break;
    case 25: // TTP223
      //Sensor_TTP223();
      break;
    case 26: // AJSR04M
      //Sensor_AJSR04M();
      break;
    case 27: // GP2Y0A21YK0F
      //Sensor_GP2Y0A21YK0F();
      break;
    case 28: // RCWL0516
      //Sensor_RCWL0516();
      break;
    case 29: // Encoder
      //Sensor_Encoder();
      break;
    case 30: // HS0038DB
      //Sensor_HS0038DB();
      break;
    case 31: // TCRT5000
      //Sensor_TCRT5000();
      break;
    case 32: // IRflame
      //Sensor_IRflame();
      break;
    case 33: // REED
      //Sensor_REED();
      break;
    case 34: // MicSmall
      //Sensor_MicSmall();
      break;
    case 35: // MicBig
      //Sensor_MicBig();
      break;
    case 36: // MetalTouch
      //Sensor_MetalTouch();
      break;
    case 37: // Heartbeat
      //Sensor_Heartbeat();
      break;
    case 38: // Btn
      //Sensor_Btn();
      break;
    case 39: // TiltSwitch
      //Sensor_TiltSwitch();
      break;
    case 40: // Dvibration
      //Sensor_Dvibration();
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