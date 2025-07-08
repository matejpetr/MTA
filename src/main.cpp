#include <Arduino.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_BMP280.h>
#include "Parser.hpp"
#include "Adafruit_TCS34725.h"
#include"ESP32Servo.h"


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


#include "Senzor_Encoder.hpp"
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

#define term3 4  // Číslo pinu (ADC1_03) 
#define term4 5  // Číslo pinu (ADC1_04) 

//3v3 i2c
#define SDA 11
#define SCL 12

//5v i2c
#define xSDA 10
//#define xSCL  neexistuje

#define MT 50 //Measuring time pro Mic senzory (ms)
#define DISTANCE 150


//sběrnice OneWire
OneWire oneWire(term1);
DallasTemperature sensors(&oneWire);

//sběrnice I2C
//extern TwoWire I2C;
//extern Adafruit_TCS34725 tcs;

DHT dht(term2,DHT11); 
//Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_4X);
Adafruit_BMP280 bmp(&I2C);
Servo myServo;




//deklarace funkcí

void SensorUPDATE(int sensorID);
void SensorUPDATE_ALL();
void SensorINIT();
void SensorRESET(int sensorID);
void SensorRESET_ALL();
//void SensorCONFIG(int sensorID); */


Sensor* SeznamSenzoru[] = {
  new DS18B20(&sensors),                      //0
  new DHT11x(term2),                          //1
  new SensorDigitalRead(term1,2,"Dhall"),     //2
  new Ahall(term2),                           //3
  new SensorDigitalRead(term1,4,"PInterrupt"),//4
  new SensorDigitalRead(term1,5, "GY521"),    //5  *zatím neexistuje
  new SensorDigitalRead(term1,6,"FC51"),      //6
  new HCSR04(term1, term2, DISTANCE),         //7
  new SensorDigitalRead(term1,8,"HCSR501"),   //8
  new SensorDigitalRead(term1,9,"KW113Z"),    //9
  new BMP280(SDA, SCL),                       //10
  new TCS34725(SDA, SCL),                     //11
  new SensorDigitalRead(term1,12,"VL53L0X"),  //12  *zatím neexistuje
  new SensorDigitalRead(term1,13,"InfraredR"),//13  *zatím neexistuje
  new SensorDigitalRead(term1,14,"InfraredE"),//14  *zatím neexistuje
  new SensorDigitalRead(term1,15,"Dntc"),     //15
  new Antc(term1),                            //16
  new PHresistance(term2),                    //17
  new Joystick(VRx,VRy,sw),                   //18
  new HallLin(term2),                         //19
  new SensorDigitalRead(term1,20,"MQ135"),    //20
  new SensorDigitalRead(term1,21,"IndPNP"),   //21
  new SensorDigitalRead(term1,22,"IndNPN"),   //22
  new SensorDigitalRead(term1,23,"DMoisture"),//23
  new SensorDigitalRead(term1,24,"AMoisture"),//24  *zatím neexistuje
  new SensorDigitalRead(term1,25,"TTP223"),   //25
  new SensorDigitalRead(term1,26,"AJSR04M"),  //26  *zatím neexistuje
  new GP2Y0A21YK0F(term1),                    //27
  new SensorDigitalRead(term1,28,"RCWL0516"), //28
  new Encoder(term3, term4),                  //29  
  new SensorDigitalRead(term1,30,"HS0038DB"), //30
  new SensorDigitalRead(term1,31,"TCRT5000"), //31
  new SensorDigitalRead(term1,32,"IRflame"),  //32
  new SensorDigitalRead(term1,33,"REED"),     //33
  new MicSmall(term1, MT),                    //34
  new MicBig(term1, MT),                      //35
  new SensorDigitalRead(term1,36,"MetalTouch"), //36
  new Heartbeat(term2,5000),                    //37 - delka mereni(ms)
  new SensorDigitalRead(term1,38,"Btn"),        //38
  new SensorDigitalRead(term1,39,"TiltSwitch"), //39
  new SensorDigitalRead(term1,40,"Dvibration"), //40
  new SensorDigitalRead(term1,41,"HGswitch"),   //41
  new SensorDigitalRead(term1,42,"Tap"),        //42
 
};

Actuator* SeznamAktuatoru[] = {
new SG90(term1,42,100), //100 
new Stepper(term1,term2,term3,term4,42,true,16), //101
new DC(term1,50,true), //102
new TwoColor(term1,term2,'R',50),
new TwoColorMini(term1,term2,'G',100),  //103
new RGB(term1,term2,term3,0,0,50),      //104
new Color7(term1,true),
new Laser(term1,true),
new BuzzP(term1,1000,500),
new BuzzA(term1,true),
};


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
      bool ResponseAll = result[4];


      if (type == "UPDATE") {
        SensorUPDATE(sensorID);
      } 
      else if (type == "RESET") {
        if (ResponseAll){
          SensorRESET_ALL();  
        } else {SensorRESET(sensorID);}
      }
      else if (type == "INIT") {SensorINIT();}
      //else if (type == "CONFIG") {SensorCONFIG();}

      serialBuffer = "";  // vyprázdní buffer
    } 
    else {serialBuffer += c;} //zapíše znak
  }

}


//požadavek UPDATE
void SensorUPDATE(int sensorID){
    SeznamSenzoru[sensorID]->update();
}

//požadavek UPDATE_ALL
void SensorUPDATE_ALL(){
int PocetSenzoru = sizeof(SeznamSenzoru) / sizeof(SeznamSenzoru[0]); 
  for (int i = 0; i < PocetSenzoru; i++) {
    SeznamSenzoru[i]->update();
  }
}


//požadavek INIT
void SensorINIT(){
    int PocetSenzoru = sizeof(SeznamSenzoru) / sizeof(SeznamSenzoru[0]); //velikost pole v bajtech/velikost jednoho prvku = počet senzorů
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
int PocetSenzoru = sizeof(SeznamSenzoru) / sizeof(SeznamSenzoru[0]); 
  for (int i = 0; i < PocetSenzoru; i++) {
    SeznamSenzoru[i]->reset();  
  }
}



