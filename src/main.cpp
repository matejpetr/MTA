#include <Arduino.h>

String* parseGET(String queryString);

#include <DHT.h>
#define DHTPIN 1
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

void Sensosr_DHT11();

void setup() {
  Serial.begin(115200);
  Serial.println("M-TA: Sensor borad ready!");
}

void loop() {

  int sensorID = 0;

  delay(1000);

  // CMD Parser
  
  // Příklad GET dotazu
  String exampleQuery = "type=dht11&resolution=0.1";

  // Parsování příkladu
  String* result = parseGET(exampleQuery);

  // Zobrazení výsledného pole
  //Serial.println("--- Parsed Parameters ---");
  //Serial.println("Type: "       + result[0]);
  //Serial.println("SensorID: "   + result[1]);
  //Serial.println("Resolution: " + result[2]);

  // Call of specific sensor
  sensorID = result[1].toInt();

  // TESTING //
  // sensorID = 0;
  switch (sensorID) {
    case 0: // Sensor: 
      Sensosr_DHT11();
      break;
    case 1: // Sensor:
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

void Sensosr_DHT11()
{
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("?type=dht11&id=2&temp=nan&humi=nan"));
    return;
  }
  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  
  Serial.print(F("?type=dht11&id=2&humi="));
  Serial.print(h);
  Serial.print(F("&temp="));
  Serial.print(t);
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

