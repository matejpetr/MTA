#include "libs.hpp"
#include "GVL.hpp"
#include <HardwareSerial.h>
#include "vscp_port.hpp"  

extern "C" void VSCP_SetupRegisterAll();
extern "C" void VSCP_Poll();



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
  new Ahall(term2),                           // 3  -- s rezistorem
  new SensorDigitalRead(term1,4,"PInterrupt"),// 4
  new SensorDigitalRead(term1,5,"FC51"),      // 5
  new HCSR04(term1, term2),                   // 6
  new SensorDigitalRead(term1,7,"HCSR501"),   // 7
  new SensorDigitalRead(term1,8,"KW113Z"),    // 8
  new BMP280(xSDA, xSCL),                     // 9
  new BMP180(xSDA, xSCL),                     // 10
  new TCS34725(xSDA, xSCL),                   // 11
  new IRrx(term1),                            // 12
  new SensorDigitalRead(term1,13,"Dntc"),     // 13
  new Antc(term2),                            // 14
  new PHresistance(term2),                    // 15
  new Joystick(VRx,VRy,sw),                   // 16
  new HallLin(term2),                         // 17 -- bez rezistoru
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
  new SensorDigitalRead(term1,29,"MetalTouch"),// 29
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



void setup()
{
  #if USE_HW_UART  //HW serial
  VSCP_STREAM.begin(VSCP_BAUD, SERIAL_8N1, VSCP_RX_PIN, VSCP_TX_PIN);
  //Serial.begin(115200);
  delay(200);
  VSCP_STREAM.println("DBG: VSCP po Serial1");
#else // USB Serial
  
  VSCP_STREAM.begin(VSCP_BAUD);
  delay(200);
  VSCP_STREAM.println("DBG: VSCP po USB Serial");
#endif

  VSCP_STREAM.println("fungujeme ?");
  VSCP_SetupRegisterAll();
  VSCP_STREAM.setTimeout(0);

  //Encoder_init(term3,term4);
  sensors.begin();
  I2C.begin(xSDA, xSCL);
  tcs.begin(0x29,&I2C);
  dht.begin();
  bmp.begin(0x76);
  bmp180.begin(0x77, &I2C);
  myServo.attach(term1);
}


void loop() {
  VSCP_Poll();
}