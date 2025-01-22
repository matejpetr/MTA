#include <Arduino.h>

#include "Parser.hpp"

#include "Senzor_DHT11.hpp"
#include "Senzor_DS18B20.hpp"
#include "Senzor_GY_521.hpp"
#include "Senzor_BMP280.hpp"
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
    case 0: // Sensor: 
      Sensor_DHT11(oneWire);
      break;
    case 1: // Sensor:
      Sensor_DS18B20(sensors);
      break;
    case 3: // Digital Hall Sensor:
      Specific_Sensor_DigitalRead(pinNo,type,sensorID);
      break;
    case 4: // Analog Hall Sensor:
      Specific_Sensor_AnalogRead(pinNo,type,sensorID);
      break;
    case 5: // BMP280
      Sensor_GY_BMP280();
      break;
    case 6: // GZ_521
      Sensor_GY_521();
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