#include <Arduino.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_BMP280.h>
#include "Parser.hpp"
#include "Adafruit_TCS34725.h"


#include "Senzor_DS18B20.hpp"
#include "Senzor_DHT11.hpp"
#include "Senzor_Ahall.hpp"    
#include "Senzor_GY_521.hpp"
#include "Senzor_HC_SR04.hpp"   
//#include "Senzor_HCSR501.hpp"
#include "Senzor_BMP280.hpp"
#include "Senzor_Antc.hpp"
#include "Senzor_PHresistance.hpp"
#include "Senzor_Joystick.hpp"
#include "Senzor_HallLin.hpp"
#include "Senzor_TCS34725.hpp"
//#include "Senzor_IndPNP.hpp"
//#include "Senzor_IndNPN.hpp"
//#include "Senzor_AMoisture.hpp"
//#include "Senzor_TTP223.hpp"
//#include "Senzor_AJSR04M.hpp"
#include "Senzor_GP2Y0A21YK0F.hpp"
//#include "Senzor_Encoder.hpp"
//#include "Senzor_HS0038DB.hpp"
#include "Senzor_MicSmall.hpp"
#include "Senzor_MicBig.hpp"
#include "Senzor_Heartbeat.hpp"
#include "Senzor_BMP180.hpp"
#include "Senzor_BMP.hpp"
#include "Senzor_AnalogRead.hpp"
#include "Senzor_DigitalRead.hpp"


#define VRx 15  //volitelné (nelze zapojit do HW standu)
#define VRy 7  // --||--
#define sw 17   // --||--

#define term1 15  // Číslo pinu (ADC2_05) prvního senzorického terminálu na HW standu (4pinové červené)
#define term2 7  // Číslo pinu (ADC2_04) druhého senzorického terminálu na HW standu (3pinové černé)

//3v3 i2c
#define SDA 11
#define SCL 12

//5v i2c
#define xSDA 10
//#define xSCL  neexistuje

#define MT 50 //Measuring time pro Mic senzory (ms)
#define DISTANCE 150


//sběrnice OneWire
OneWire oneWire(15);
DallasTemperature sensors(&oneWire);

//sběrnice I2C
TwoWire I2C(0); 

DHT dht(term2,DHT11); 


void SensorUPDATE(int sensorID);
//void SensorUPDATE_ALL(int sensorID);
void SensorINIT(int sensorID);
//void SensorRESET(int sensorID);
//void SensorRESET_ALL(int sensorID);
//void SensorCONFIG(int sensorID); */


Sensor* sensorTable[] = {
  new SensorDS18B20(&oneWire),    //0
  new SensorDHT11(term2),         //1
  new SensorDigitalRead(term1),   //2
  new SensorAhall(term2),         //3
  new SensorDigitalRead(term1),   //4
  new SensorDigitalRead(term1),   //5  *zatím neexistuje
  new SensorDigitalRead(term1),   //6
  new SensorHCSR04(term1, term2, DISTANCE), //7
  new SensorDigitalRead(term1),   //8
  new SensorDigitalRead(term1),   //9
  new SensorBMP280(SDA, SCL),     //10
  new SensorTCS34725(SDA, SCL),   //11
  new SensorDigitalRead(term1),   //12  *zatím neexistuje
  new SensorDigitalRead(term1),   //13  *zatím neexistuje
  new SensorDigitalRead(term1),   //14  *zatím neexistuje
  new SensorDigitalRead(term1),   //15
  new SensorAntc(term1),          //16
  new SensorPHresistance(term1),  //17
  new SensorJoystick(VRx,VRy,sw), //18
  new SensorHallLin(term2),       //19
  new SensorDigitalRead(term1),   //20
  new SensorDigitalRead(term1),   //21
  new SensorDigitalRead(term1),   //22
  new SensorDigitalRead(term1),   //23
  new SensorDigitalRead(term1),   //24  *zatím neexistuje
  new SensorDigitalRead(term1),   //25
  new SensorDigitalRead(term1),   //26  *zatím neexistuje
  new SensorGP2Y0A21YK0F(term1),  //27
  new SensorDigitalRead(term1),   //28
  new SensorDigitalRead(term1),   //29  *zatím neexistuje
  new SensorDigitalRead(term1),   //30
  new SensorDigitalRead(term1),   //31
  new SensorDigitalRead(term1),   //32
  new SensorDigitalRead(term1),   //33
  new SensorMicSmall(term1, MT),  //34
  new SensorMicBig(term1, MT),    //35
  new SensorDigitalRead(term1),   //36
  new SensorHeartbeat(term2,5000),//37 - delka mereni(ms)
  new SensorDigitalRead(term1),   //38
  new SensorDigitalRead(term1),   //39
  new SensorDigitalRead(term1),   //40
  new SensorDigitalRead(term1),   //41
  new SensorDigitalRead(term1),   //42
 
};

void setup() 
{
  Serial.begin(115200);
  Serial.println("M-TA: Sensor board ready!");
  Serial.setTimeout(0); 
  //Sensor_BMP180_Init(SDA, SCL);  
  //Sensor_BMP280_Init(SDA, SCL);
  Sensor_TCS34725_Init(SDA,SCL);
  dht.begin();
  Sensor_BMP280_Init(SDA,SCL);
}

String serialBuffer = "";  // buffer pro sestavení celé zprávy


void loop() {
  if (Serial.available()) {
    char c = Serial.read();

    if (c == '\n') {
      serialBuffer.trim();
      String* result = parseGET(serialBuffer);

      String type = result[0];
      int sensorID = result[1].toInt();
      bool ResponseAll = result[4] == "true";


      if (type == "UPDATE") {
        SensorUPDATE(sensorID);
      } else {
        
      }
      if (type == "RESET") {
        //SensorRESET(sensorID);
      } else {
        
      }
      if (type == "INIT") {
        SensorINIT(sensorID);
      } else {
       
      }

      serialBuffer = "";  // vyprázdní buffer
    } else {
      serialBuffer += c;  // zapíše znak
    }
  }

}


void SensorUPDATE(int sensorID){
    sensorTable[sensorID]->update();
}

void SensorINIT(int sensorID){
  sensorTable[sensorID]->init();
}

/*
void SensorUPDATE(int sensorID){
switch (sensorID) {
    case 0: // DS18B20
      Sensor_DS18B20(sensors);
      break;
    case 1: // DHT11
      Sensor_DHT11(oneWire);
      break;
    case 2: // DHall Sensor:
      Sensor_DigitalRead(term2);
      break;
    case 3: // AHall Sensor:
      Sensor_Ahall(term2);
      break;
    case 4: // Phinterrupt
      Sensor_DigitalRead(term2);
      break;
    case 5: // GY521
      Sensor_GY_521();
      break;
    case 6: // FC51
      Sensor_DigitalRead(term2);
      break;
    case 7: // HCSR04
      Sensor_HCSR04(15,7,DISTANCE);  // upravit piny
      break;
    case 8: // HCSR501
      Sensor_DigitalRead(term2);
      break;
    case 9: // KW113Z
      //Sensor_KW113Z();
      break;
    case 10: // BMP280
      //Sensor_GY_BMP280();
        Sensor_BMP280();
      break;
    case 11: // TCS34725
      Sensor_TCS34725(SDA,SCL);
      break;
    case 12: // VL53L0X
      //Sensor_VL53L0X();
      break;
    case 13: // InfraredR
      //Sensor_InfraredR();
      break;
    case 14: // InfraredE     
      Sensor_BMP180();        // upravit cislovani - provizorni
      break;
    case 15: // Dntc
      Sensor_DigitalRead(term1);
      break;
    case 16: // Antc
      Sensor_Antc(term2);
      break;
    case 17: // PIresistance
      Sensor_PHresistance(term2);
      break;
    case 18: // Joystick
      Sensor_Joystick(VRx,VRy,sw);
      break;
    case 19: // HallLin
      Sensor_HallLin(term1);
      break;
    case 20: // MQ135
      Sensor_DigitalRead(term1);
      break;
    case 21: // IndPNP
      //Sensor_IndPNP();
      break;
    case 22: // IndNPN
      //Sensor_IndNPN();
      break;
    case 23: // Dmoisture
      Sensor_DigitalRead(term1);
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
      Sensor_GP2Y0A21YK0F();
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
      Sensor_DigitalRead(term2);
      break;
    case 33: // REED
      Sensor_DigitalRead(term2);
      break;
    case 34: // MicSmall
      Sensor_MicSmall(term1, MT);
      break;
    case 35: // MicBig
      Sensor_MicBig(term1, MT);
      break;
    case 36: // MetalTouch
      Sensor_DigitalRead(term2);
      break;
    case 37: // Heartbeat
      Sensor_Heartbeat(term2, 5000);
      break;
    case 38: // Btn
      Sensor_DigitalRead(term1);
      break;
    case 39: // TiltSwitch
      Sensor_DigitalRead(term1);
      break;
    case 40: // Dvibration
      Sensor_DigitalRead(term1);
      break;
    case 41: // HGswitch
      Sensor_DigitalRead(term1);
      break;
    case 42: // Tap
      Sensor_DigitalRead(term1);
      break;
      
    case 100: // Universal Diagital Input Read; type=DigitalRead&id=100&pinNo=x;
      Sensor_DigitalRead(term2);
      break;
    case 200: // Universal Analog input Read; type=AnalogRead&id=100&pinNo=x;
      Sensor_AnalogRead(term1);
      break;
    default: // If the command is not recognized, do nothing
      Serial.println("?id=" + String(sensorID) + "&message=NotSupported");
      sensorID = -1;
      break;
  }
}
void SensorRESET(int sensorID){

  
}
*/
