#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

String* parseGET(String queryString);

void Sensosr_DHT11();
#include <DHT.h>
#define DHTPIN 1
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

void Sensosr_DS11B20();
#define ONE_WIRE_BUS 2  // Pin pro připojení datového vodiče senzoru
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);



void setup() {
  Serial.begin(115200);
  Serial.println("M-TA: Sensor borad ready!");
}

void loop() {

  int sensorID = 0;

  delay(1000);

  // Příklad GET dotazu
  String exampleQuery = "type=dht11&id=0&resolution=0.1";

  // Parsování příkladu
  String* result = parseGET(exampleQuery);

  // Zobrazení výsledného pole
  //Serial.println("--- Parsed Parameters ---");
  //Serial.println("Type: "       + result[0]);
  //Serial.println("SensorID: "   + result[1]);
  //Serial.println("Resolution: " + result[2]);

  // Call of specific sensor
  sensorID = result[1].toInt();

  sensorID = 1;

  switch (sensorID) {
    case 0: // Sensor: 
      Sensosr_DHT11();
      break;
    case 1: // Sensor:
      Sensosr_DS11B20();
      break;
    case 2: // Sensor:
      break;
    case 3: // Sensor:
      break;
    default: // If the command is not recognized, do nothing
      Serial.println("?id=" + String(sensorID) + "&message=NotSupported");
      sensorID = -1;
      break;
  }

}

void Sensosr_DS11B20()
{
  sensors.requestTemperatures();  // Požádej senzory o teplotu
  float temperature = sensors.getTempCByIndex(0);  // Čti první senzor
  Serial.print(F("?type=ds11b20&id=0&temp="));
  Serial.println(temperature);
}

void Sensosr_DHT11()
{
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float f = dht.readTemperature(true);
  //if (isnan(h) || isnan(t) || isnan(f)) {
  //  Serial.println(F("?type=dht11&id=2&temp=nan&humi=nan"));
    //return;
  //}
  // Compute heat index in Fahrenheit (the default)
  //float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  //float hic = dht.computeHeatIndex(t, h, false);
  
  Serial.print(F("?type=dht11&id=0&humi="));
  Serial.print(h);
  Serial.print(F("&temp="));
  Serial.println(t);
}


// Funkce pro parsování GET dotazu
String* parseGET(String queryString) {
  static String values[3]; // Pole pro ukládání hodnot: type, temp, humi

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
      }
    }

    start = end + 1;
  }

  return values;
}

