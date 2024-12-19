#include <Arduino.h>

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

  static int sensorID = 0;

  delay(1000);

  // CMD Parser

  // Call of specific sensor
  switch (sensorID) {
    case 0: // Sensor: 
      Serial.println("M-TA: Sensor " + String(sensorID) + " output is ...");
      Sensosr_DHT11();
      break;
    case 1: // Sensor:
      Serial.println("M-TA: Sensor " + String(sensorID) + " output is ...");
      break;
    case 2: // Sensor:
      Serial.println("M-TA: Sensor " + String(sensorID) + " output is ...");
      break;
    case 3: // Sensor:
      Serial.println("M-TA: Sensor " + String(sensorID) + " output is ...");
      break;
    default: // If the command is not recognized, do nothing
      Serial.println("M-TA: Sensor " + String(sensorID) + " NOT supported!");
      sensorID = -1;
      break;
  }

  sensorID++;
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
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
}
