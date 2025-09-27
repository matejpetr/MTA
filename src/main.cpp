#include <Arduino.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_BMP280.h>
#include "Parser.hpp"
#include "Adafruit_TCS34725.h"
#include "ESP32Servo.h"

// knihovny aktuátorů
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

// knihovny senzorů
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

#define VRx 15 // Číslo pinu (ADC2_05) prvního senzorického terminálu na HW standu (4pinové červené)
#define VRy 7  // Číslo pinu (ADC2_04) druhého senzorického terminálu na HW standu (3pinové černé)
#define sw 17

#define term1 15
#define term2 7
#define term3 4
#define term4 5

// 3v3 i2c
#define SDA 11
#define SCL 12

// 5v i2c
#define xSDA 10
// #define xSCL  neexistuje

#define MT 50 // Measuring time pro Mic (ms)

OneWire oneWire(term1);
DallasTemperature sensors(&oneWire);

DHT dht(term2, DHT11);
Adafruit_BMP280 bmp(&I2C);
Adafruit_BMP085 bmp180;
Servo myServo;

// --- Deklarace funkcí (beze změny rozhraní vůči třídám) ---
void SensorUPDATE(int sensorID);
void SensorUPDATE_ALL();
void SensorINIT();
void SensorRESET(int sensorID);
void SensorRESET_ALL();

void ActuatorRESET_ALL();
void ActuatorRESET(int actuatorID);

// --- Pomocné utility pro sjednocené ID ---
static inline String makeId(char prefix, int idx) {
  // dvouciferné nulování: 0..9 -> 00..09, 10.. -> 10..
  String s = (idx < 10) ? "0" + String(idx) : String(idx);
  return String(prefix) + s;
}

static bool isDigits(const String& s) {
  for (size_t i = 0; i < s.length(); ++i) {
    if (!isDigit(s[i])) return false;
  }
  return s.length() > 0;
}

/*
 * Parsuje sjednocené ID.
 * Povoleno:
 *  - "Sxx" pro senzory (např. S00, S09, S12)
 *  - "Axx" pro aktuátory (např. A00, A05)
 *  - "S*"  nebo "A*" pro zásah do všech prvků dané skupiny
 *
 * Výstupy:
 *  - prefix: 'S' nebo 'A'
 *  - index:  >=0 pro konkrétní prvek, -1 pokud jde o "všechny" 
*/

static bool parseUnifiedId(String idStr, char& prefix, int& index) {
  idStr.trim();
  idStr.toUpperCase();

  if (idStr.length() < 2) return false;

  prefix = idStr[0];
  if (prefix != 'S' && prefix != 'A') return false;

  // Všichni v dané skupině: "S*" / "A*"
  if (idStr.length() == 2 && idStr[1] == '*') {
    index = -1;
    return true;
  }

  // Konkrétní index: zbytek musí být čísla (s nulováním, např. "00", "05", "12")
  String digits = idStr.substring(1);
  if (!isDigits(digits)) return false;

  index = digits.toInt();
  return true;
}

// --- Globální seznamy ---
Sensor* SeznamSenzoru[] = {
  new DS18B20(&sensors),                      // 0
  new DHT11x(term2),                          // 1
  new SensorDigitalRead(term2,2,"Dhall"),     // 2
  new Ahall(term2),                           // 3
  new SensorDigitalRead(term1,4,"PInterrupt"),// 4
  new SensorDigitalRead(term1,5,"FC51"),      // 5
  new HCSR04(term1, term2),                   // 6
  new SensorDigitalRead(term1,7,"HCSR501"),   // 7
  new SensorDigitalRead(term1,8,"KW113Z"),    // 8
  new BMP280(SDA, SCL),                       // 9
  new BMP180(SDA, SCL),                       // 10
  new TCS34725(SDA, SCL),                     // 11
  new IRrx(term1),                            // 12
  new SensorDigitalRead(term1,13,"Dntc"),     // 13
  new Antc(term2),                            // 14
  new PHresistance(term2),                    // 15
  new Joystick(VRx,VRy,sw),                   // 16
  new HallLin(term2),                         // 17
  new SensorDigitalRead(term1,18,"MQ135"),    // 18
  new SensorDigitalRead(term1,19,"DMoisture"),// 19
  new SensorDigitalRead(term1,20,"TTP223"),   // 20
  new GP2Y0A21YK0F(term1),                    // 21
  new Rencoder(term3, term4),                 // 22
  new SensorDigitalRead(term1,23,"HS0038DB"), // 23
  new SensorDigitalRead(term1,24,"TCRT5000"), // 24
  new SensorDigitalRead(term1,25,"IRflame"),  // 25
  new SensorDigitalRead(term2,26,"REED"),     // 26
  new MicSmall(term1, MT),                    // 27
  new MicBig(term1, MT),                      // 28
  new SensorDigitalRead(term1,29,"MetalTouch"), // 29
  new Heartbeat(term2,5000),                  // 30
  new SensorDigitalRead(term2,31,"Btn"),      // 31
  new SensorDigitalRead(term2,32,"TiltSwitch"),// 32
  new SensorDigitalRead(term2,33,"Dvibration"),// 33
  new SensorDigitalRead(term2,34,"HGswitch"),  // 34
  new SensorDigitalRead(term2,35,"Tap"),       // 35
};
int PocetSenzoru = sizeof(SeznamSenzoru) / sizeof(SeznamSenzoru[0]);

Actuator* SeznamAktuatoru[] = {
  new SG90(term1,42,100),                           //0
  new Stepper(term1,term2,term3,term4,42,true,16),  //1
  new DC(term1,50,true),                            //2
  new TwoColor(term3,term4,'R',50),                 //3
  new TwoColorMini(term4,term3,'G',100),            //4
  new RGB(term1,term2,term3,0,0,50),                //5
  new Color7(term1,true),                           //6
  new Laser(term2,true),                            //7
  new BuzzP(term2,1000,500),                        //8
  new BuzzA(term2,true),                            //9
  new IRtx(term2,0),                                //10
};

int PocetAktuatoru = sizeof(SeznamAktuatoru) / sizeof(SeznamAktuatoru[0]);

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

String serialBuffer = "";

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      serialBuffer.trim();
      String* result = parseGET(serialBuffer);

      String type = result[0];
      String idStr = result[1];
      ResponseAll = (result[2] == "true");

      // sjednocené rozlišení ID
      char prefix = 0;
      int idx = -2; // -1 = all, >=0 = index, -2 = neplatné
      bool idOk = parseUnifiedId(idStr, prefix, idx);

      // --- UPDATE: jen senzory ---
      if (type == "UPDATE") {
        if (!idOk || (prefix != 'S' && idx != -1)) {
          // pokud nepřišlo Sxx/S*, nic neděláme
        } else {
          if (idx == -1) {
            // S* -> všichni senzory
            globalBuffer = "";
            SensorUPDATE_ALL();
            Serial.println(globalBuffer);
          } else if (idx >= 0 && idx < PocetSenzoru) {
            SeznamSenzoru[idx]->update();
          }
        }
      }

      // --- RESET: senzory i aktuátory ---
      else if (type == "RESET") {
        if (!idOk) {
          // nic
        } else if (idx == -1) {
          if (prefix == 'S') {
            SensorRESET_ALL();
          } else if (prefix == 'A') {
            ActuatorRESET_ALL();
          }
        } else {
          if (prefix == 'S') {
            if (idx >= 0 && idx < PocetSenzoru) SeznamSenzoru[idx]->reset();
          } else if (prefix == 'A') {
            if (idx >= 0 && idx < PocetAktuatoru) SeznamAktuatoru[idx]->reset();
          }
        }
      }

      // --- INIT: vrací seznam dostupných senzorů ve formátu Sxx:Type ---
      else if (type == "INIT") {
        SensorINIT();
      }

      // --- CONFIG: podle prefixu S/A pošli do správného seznamu ---
      else if (type == "CONFIG") {
        int paramCount;
        Param* params = parseKeyValueParams(serialBuffer, paramCount);

        if (idOk && idx >= -1) {
          if (idx == -1) {
            // hromadná CONFIG se typicky nepoužívá; nic neprovádím
          } else if (prefix == 'A') {
            if (idx >= 0 && idx < PocetAktuatoru) {
              SeznamAktuatoru[idx]->config(params, paramCount);
            }
          } else if (prefix == 'S') {
            if (idx >= 0 && idx < PocetSenzoru) {
              SeznamSenzoru[idx]->config(params, paramCount);
            }
          }
        }
      }

      serialBuffer = "";
    } else {
      serialBuffer += c;
    }
  }
}

// --- požadavky na senzory ---
void SensorUPDATE(int sensorID){
  SeznamSenzoru[sensorID]->update();
}

void SensorUPDATE_ALL(){
  for (int i = 0; i < PocetSenzoru; i++) {
    SeznamSenzoru[i]->update();
  }
}

void SensorINIT(){
  String result = "?";
  for (int i = 0; i < PocetSenzoru; i++) {
    if (SeznamSenzoru[i]->init()) {
      result += makeId('S', i) + ":" + SeznamSenzoru[i]->getType() + ",";
    }
  }
  if (result.endsWith(",")) result.remove(result.length() - 1);
  Serial.println(result);
}

void SensorRESET(int sensorID){
  SeznamSenzoru[sensorID]->reset();
}

void SensorRESET_ALL(){
  for (int i = 0; i < PocetSenzoru; i++) {
    SeznamSenzoru[i]->reset();
  }
}

// --- požadavky na aktuátory ---
void ActuatorRESET_ALL(){
  for (int i = 0; i < PocetAktuatoru; i++) {
    SeznamAktuatoru[i]->reset();
  }
}

void ActuatorRESET(int actuatorID){
  SeznamAktuatoru[actuatorID]->reset();
}
