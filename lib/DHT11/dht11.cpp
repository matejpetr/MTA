#include "DHT11.h"

  /**
   * Konstruktor
   * Inicializuje datový pin pro komunikaci se senzorem DHT11.
   *
   * @param pin: Číslo digitálního pinu na desce Arduino/ESP32, ke kterému je připojen senzor DHT11.
   */
DHT11::DHT11(int pin) : _pin(pin)
{
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, HIGH);
}

  /**
   * Nastavuje prodlevu mezi po sobě jdoucími měřeními senzoru.
   * Pokud tato metoda není zavolána, použije se výchozí prodleva 500 milisekund.
   *
   * @param delay: Doba zpoždění v milisekundách mezi měřeními senzoru.
   */
void DHT11::setDelay(unsigned long delay)
{
  _delayMS = delay;
}

  /**
   * Soukromá metoda pro čtení nezpracovaných dat ze senzoru DHT11.
   * Tato metoda zapouzdřuje komunikaci se senzorem a proces čtení dat,
   * a je využívána veřejnými metodami pro získání údajů o teplotě a vlhkosti.
   *
   * @param data: Pole pro ukládání nezpracovaných dat načtených ze snímače.
   * @return: Celé číslo vyjadřující stav operace čtení.
   *          Vrací 0, pokud je čtení úspěšné, DHT11::ERROR_TIMEOUT, pokud dojde k timeoutu,
   *          nebo DHT11::ERROR_CHECKSUM, pokud došlo k chybě kontrolního součtu.
   */
int DHT11::readRawData(byte data[5])
{
  delay(_delayMS);
  startSignal();
  unsigned long timeout_start = millis();

  while (digitalRead(_pin) == HIGH)
  {
    if (millis() - timeout_start > TIMEOUT_DURATION)
    {
      return DHT11::ERROR_TIMEOUT;
    }
  }

  if (digitalRead(_pin) == LOW)
  {
    delayMicroseconds(80);
    if (digitalRead(_pin) == HIGH)
    {
      delayMicroseconds(80);
      for (int i = 0; i < 5; i++)
      {
        data[i] = readByte();
        if (data[i] == DHT11::ERROR_TIMEOUT)
        {
          return DHT11::ERROR_TIMEOUT;
        }
      }

      if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))
      {
        return 0; // Success
      }
      else
      {
        return DHT11::ERROR_CHECKSUM;
      }
    }
  }
  return DHT11::ERROR_TIMEOUT;
}

  /**
   * Přečte byte dat ze senzoru DHT11.
   *
   * @return: byte dat načtených ze senzoru.
   */
byte DHT11::readByte()
{
  byte value = 0;

  for (int i = 0; i < 8; i++)
  {
    while (digitalRead(_pin) == LOW)
      ;
    delayMicroseconds(30);
    if (digitalRead(_pin) == HIGH)
    {
      value |= (1 << (7 - i));
    }
    while (digitalRead(_pin) == HIGH)
      ;
  }
  return value;
}

  /**
   * Odešle startovací signál do senzoru DHT11 a zahájí čtení dat.
   * To zahrnuje nastavení datového pinu na určitou dobu na LOW a poté HIGH,
   * a nakonec se nastaví do vstupního režimu pro čtení dat.
   */
void DHT11::startSignal()
{
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
  delay(18);
  digitalWrite(_pin, HIGH);
  delayMicroseconds(40);
  pinMode(_pin, INPUT);
}

/**
 * Čte a vrací teplotu ze senzoru DHT11.
 * Využívá metodu readRawData k načtení surových dat ze senzoru a poté z datového pole extrahuje teplotu.
 *
 * @return: celo-číselná hodnota teploty ve stupních Celsia.
 */
int DHT11::readTemperature()
{
  byte data[5];
  int error = readRawData(data);
  return data[2];
}

/**
 * Čte a vrací vlhkost ze senzoru DHT11.
 * Využívá metodu readRawData k načtení surových dat ze senzoru a poté z datového pole extrahuje vlhkost.
 *
 * @return: celo-číselná hodnota vlhkosti.
 */
int DHT11::readHumidity()
{
  byte data[5];
  int error = readRawData(data);
  if (error != 0)
  {
    return error;
  }
  return data[0];
}

  /**
   * Vrátí lidsky-čitelný error indikující problém v programu.
   *
   * @param errorCode: Samotný error.
   * @return: String erroru.
   */
String DHT11::getErrorString(int errorCode)
{
  switch (errorCode)
  {
  case DHT11::ERROR_TIMEOUT:
    return "Error 253 Reading from DHT11 timed out.";
  case DHT11::ERROR_CHECKSUM:
    return "Error 254 Checksum mismatch while reading from DHT11.";
  default:
    return "Error Unknown.";
  }
}