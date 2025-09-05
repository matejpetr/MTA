#include <Arduino.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_BMP280.h>
#include "Parser.hpp"
#include "Adafruit_TCS34725.h"
#include"ESP32Servo.h"

//knihovny aktuátorů
#include "Stepper.hpp"
#include "SG90.hpp"
#include "DC.hpp"
#include "TwoColor.hpp"
#include "TwoColorMini.hpp"
#include "RGB.hpp"
#include "7color.hpp"
#include "Laser.hpp"
#include "BuzzP.hpp"
#include "BuzzA.hpp"
#include "IRtx.hpp"

//knihovny Senzorů
#include "Senzor_Encoder.hpp"
#include "Senzor_DS18B20.hpp"
#include "Senzor_DHT11.hpp"
#include "Senzor_Ahall.hpp"    
#include "Senzor_GY_521.hpp"
#include "Senzor_HC_SR04.hpp"   
#include "Senzor_Antc.hpp"
#include "Senzor_PHresistance.hpp"
#include "Senzor_Joystick.hpp"
#include "Senzor_HallLin.hpp"
#include "Senzor_TCS34725.hpp"
#include "Senzor_GP2Y0A21YK0F.hpp"
#include "Senzor_MicSmall.hpp"
#include "Senzor_MicBig.hpp"
#include "Senzor_Heartbeat.hpp"
#include "Senzor_BMP180.hpp"
#include "Senzor_BMP.hpp"
#include "Senzor_AnalogRead.hpp"
#include "Senzor_DigitalRead.hpp"
#include "Senzor_IRrx.hpp"


#define VRx 15  //volitelné (nelze zapojit do HW standu)
#define VRy 7  // --||--
#define sw 17   // --||--

#define term1 15  // Číslo pinu (ADC2_05) prvního senzorického terminálu na HW standu (4pinové červené)
#define term2 7  // Číslo pinu (ADC2_04) druhého senzorického terminálu na HW standu (3pinové černé)

#define term3 4  // Číslo pinu (ADC1_03) 
#define term4 5  // Číslo pinu (ADC1_04) 

//3v3 i2c
#define SDA 11
#define SCL 12

//5v i2c
#define xSDA 10
//#define xSCL  neexistuje

#define MT 50 //Measuring time pro Mic senzory (ms)

//sběrnice OneWire
OneWire oneWire(term1);
DallasTemperature sensors(&oneWire);

//sběrnice I2C
//extern TwoWire I2C;
//extern Adafruit_TCS34725 tcs;

DHT dht(term2,DHT11); 
//Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_4X);
Adafruit_BMP280 bmp(&I2C);
Adafruit_BMP085 bmp180;
Servo myServo;




//deklarace funkcí

void SensorUPDATE(int sensorID);
void SensorUPDATE_ALL();
void SensorINIT();
void SensorRESET(int sensorID);
void SensorRESET_ALL();
//void SensorCONFIG(int sensorID); */

void ActuatorRESET_ALL();
void ActuatorRESET(String id);



Sensor* SeznamSenzoru[] = {
  new DS18B20(&sensors),                      //0
  new DHT11x(term2),                          //1
  new SensorDigitalRead(term2,2,"Dhall"),     //2
  new Ahall(term2),                           //3
  new SensorDigitalRead(term2,4,"PInterrupt"),//4
  new SensorDigitalRead(term1,5,"FC51"),      //5
  new HCSR04(term1, term2),                   //6
  new SensorDigitalRead(term1,7,"HCSR501"),   //7
  new SensorDigitalRead(term1,8,"KW113Z"),    //8
  new BMP280(SDA, SCL),                       //9
  new BMP180(SDA, SCL),                       //10
  new TCS34725(SDA, SCL),                     //11
  new IRrx(term1),                            //12  
  new SensorDigitalRead(term1,13,"Dntc"),     //13
  new Antc(term2),                            //14
  new PHresistance(term2),                    //15
  new Joystick(VRx,VRy,sw),                   //16
  new HallLin(term2),                         //17
  new SensorDigitalRead(term1,18,"MQ135"),    //18
  new SensorDigitalRead(term1,19,"DMoisture"),//19
  new SensorDigitalRead(term1,20,"TTP223"),   //20
  new GP2Y0A21YK0F(term1),                    //21
  new Rencoder(term3, term4),                 //22 
  new SensorDigitalRead(term1,23,"HS0038DB"), //23
  new SensorDigitalRead(term1,24,"TCRT5000"), //24
  new SensorDigitalRead(term1,25,"IRflame"),  //25
  new SensorDigitalRead(term2,26,"REED"),     //26
  new MicSmall(term1, MT),                    //27
  new MicBig(term1, MT),                      //28
  new SensorDigitalRead(term1,29,"MetalTouch"), //29
  new Heartbeat(term2,5000),                    //30 - delka mereni(ms)
  new SensorDigitalRead(term2,31,"Btn"),        //31
  new SensorDigitalRead(term2,32,"TiltSwitch"), //32
  new SensorDigitalRead(term2,33,"Dvibration"), //33
  new SensorDigitalRead(term2,34,"HGswitch"),   //34
  new SensorDigitalRead(term2,35,"Tap"),        //35
 
};

//velikost pole v bajtech/velikost jednoho prvku = počet senzorů
int PocetSenzoru = sizeof(SeznamSenzoru) / sizeof(SeznamSenzoru[0]); 

Actuator* SeznamAktuatoru[] = {
new SG90(term1,42,100), 
new Stepper(term1,term2,term3,term4,42,true,16), 
new DC(term1,50,true), 
new TwoColor(term3,term4,'R',50),
new TwoColorMini(term4,term3,'G',100), 
new RGB(term1,term2,term3,0,0,50),      
new Color7(term1,true),
new Laser(term1,true),
new BuzzP(term1,1000,500),
new BuzzA(term1,true),
new IRtx(term1,0),
};

//velikost pole v bajtech/velikost jednoho prvku = počet aktuátorů
int PocetAktuatoru =sizeof(SeznamAktuatoru)/sizeof(SeznamAktuatoru[0]);

String globalBuffer = "";
bool ResponseAll = false;


void setup() 
{
  Serial.begin(115200);
  Serial.setTimeout(0); 

  Encoder_init(term3,term4);
  sensors.begin();
  I2C.begin(SDA, SCL);
  tcs.begin(0x29,&I2C);
  dht.begin();
  bmp.begin(0x76);
  bmp180.begin(0x77, &I2C);   
  myServo.attach(term1);
}

String serialBuffer = "";  // buffer pro sestavení celé zprávy


void loop() {
if (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      serialBuffer.trim();
      String* result = parseGET(serialBuffer);

      String type = result[0];
      String idStr = result[1];
      ResponseAll = (result[2] == "true");
      int index = toupper(idStr[0]) - 'A';


      if (type == "UPDATE") {
        if (ResponseAll) {
          globalBuffer = "";
          SensorUPDATE_ALL();
          Serial.println(globalBuffer);
        } else {
          int id = idStr.toInt();
          if (id >= 0 && id < PocetSenzoru) {
            SeznamSenzoru[id]->update();
          }
        }
      }

      else if (type == "RESET") {
       if (idStr.length() == 1 && isAlpha(idStr[0])) {
        if (ResponseAll) {
          ActuatorRESET_ALL();
        } else {
            if (index >= 0 && index < PocetAktuatoru) {
              SeznamAktuatoru[index]->reset();
            }
        }
          
       }

       else{
        if (ResponseAll) {
          SensorRESET_ALL();
        } else {
          int id = idStr.toInt();
          if (id >= 0 && id < PocetSenzoru) {
            SeznamSenzoru[id]->reset();
          }
        }
      }
    }


      else if (type == "INIT") {
        SensorINIT();
      }

      else if (type == "CONFIG") {
        int paramCount;
        Param* params = parseKeyValueParams(serialBuffer, paramCount);

        // Pokud id je písmeno A,B,C,... mapuj na seznam aktuátorů
        if (idStr.length() == 1 && isAlpha(idStr[0])) {
          int index = toupper(idStr[0]) - 'A';
          if (index >= 0 && index < PocetAktuatoru) {
            SeznamAktuatoru[index]->config(params, paramCount);
          }
        }
        else {
          int id = idStr.toInt();
          if (id >= 0 && id < PocetSenzoru) {
          SeznamSenzoru[id]->config(params, paramCount);
    }
  }


      }

      serialBuffer = "";
    } else {
      serialBuffer += c;
    }
  }
}


//požadavek UPDATE
void SensorUPDATE(int sensorID){
    SeznamSenzoru[sensorID]->update();
}

//požadavek UPDATE_ALL
void SensorUPDATE_ALL(){ 
  for (int i = 0; i < PocetSenzoru; i++) {
    SeznamSenzoru[i]->update();
  }
}


//požadavek INIT
void SensorINIT(){
    String result = "?";

  for (int i = 0; i < PocetSenzoru; i++) {
    if (SeznamSenzoru[i]->init()) {
      result += String(i) + ":" + SeznamSenzoru[i]->getType() + ",";
    }
  }

  //odstraní poslední čárku zprávy
  if (result.endsWith(",")) {
    result.remove(result.length() - 1);
  }

  Serial.println(result);
}


//požadavek RESET
void SensorRESET(int sensorID){
  SeznamSenzoru[sensorID]->reset();
}



//požadavek RESET_ALL
void SensorRESET_ALL(){
  for (int i = 0; i < PocetSenzoru; i++) {
    SeznamSenzoru[i]->reset();  
  }
}

void ActuatorRESET_ALL(){
 for (int i = 0; i < PocetAktuatoru; i++) {
    SeznamAktuatoru[i]->reset();
  }
}

void ActuatorRESET(int sensorID){
  SeznamAktuatoru[sensorID]->reset();
}