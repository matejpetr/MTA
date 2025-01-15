#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

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