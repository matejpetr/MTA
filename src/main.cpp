#include "libs.hpp"
#include "GVL.hpp"
#include "Parser.hpp"


// --- Deklarace funkcí (beze změny rozhraní vůči třídám) ---
void SensorUPDATE(int sensorID);
void SensorUPDATE_ALL();
void SensorINIT();
void SensorRESET(int sensorID);
void SensorRESET_ALL();

void ActuatorRESET_ALL();
void ActuatorRESET(int actuatorID);


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
  new BMP280(xSDA, xSCL),                       // 9
  new BMP180(xSDA, xSCL),                       // 10
  new TCS34725(xSDA, xSCL),                     // 11
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
  I2C.begin(xSDA, xSCL);
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

      char prefix = 0;
      int idx = -2; // -1 = all, >=0 = index, -2 = neplatné
      bool idOk = parseUnifiedId(idStr, prefix, idx);

      // --- UPDATE: jen senzory ---
      if (type == "UPDATE") {
        if (!idOk || (prefix != 'S' && idx != -1)) {
          
        } else {
          if (idx == -1) {
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
