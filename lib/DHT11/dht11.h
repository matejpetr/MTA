#ifndef DHT11_h
#define DHT11_h

#include "Arduino.h"

/**
 * DHT11 Class
 * Poskytuje metody pro čtení údajů o teplotě a vlhkosti ze snímače DHT11.
 */
class DHT11
{
public:
  /**
   * Konstruktor
   * Inicializuje datový pin pro komunikaci se senzorem DHT11.
   *
   * @param pin: Číslo digitálního pinu na desce Arduino/ESP32, ke kterému je připojen senzor DHT11.
   */
  DHT11(int pin);

  /**
   * Nastavuje prodlevu mezi po sobě jdoucími měřeními senzoru.
   * Pokud tato metoda není zavolána, použije se výchozí prodleva 500 milisekund.
   *
   * @param delay: Doba zpoždění v milisekundách mezi měřeními senzoru.
   */
  void setDelay(unsigned long delay);

  /**
   * Přečte a vrátí hodnotu vlhkosti ze senzoru DHT11.
   *
   * @return: celo-číselnou vlhkost
   */
  int readHumidity();

  /**
   * Přečte a vrátí hodnotu teploty ze senzoru DHT11.
   *
   * @return: celo-číselnou teplotu v celsiích
   */
  int readTemperature();

  // Konstanty
  static const int ERROR_CHECKSUM = 254;    // Error indikující chybu v přicházejících hodnotách.
  static const int ERROR_TIMEOUT = 253;     // Error indikující timeout běhěm chodu programu.
  static const int TIMEOUT_DURATION = 1000; // Čas (v milisekundách), udávající jak dlouho má program čekat než pošle TIMEOUT error.

  /**
   * Vrátí lidsky-čitelný error indikující problém v programu.
   *
   * @param errorCode: Samotný error.
   * @return: String erroru.
   */
  static String getErrorString(int errorCode);

private:
  int _pin;                     // Pin na kterém je DHT11.
  unsigned long _delayMS = 500; // Výchozí prodleva v milisekundách mezi odečty ze snímače.

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
  int readRawData(byte data[5]);

  /**
   * Přečte byte dat ze senzoru DHT11.
   *
   * @return: byte dat načtených ze senzoru.
   */
  byte readByte();

  /**
   * Odešle startovací signál do senzoru DHT11 a zahájí čtení dat.
   * To zahrnuje nastavení datového pinu na určitou dobu na LOW a poté HIGH,
   * a nakonec se nastaví do vstupního režimu pro čtení dat.
   */
  void startSignal();
};
#endif
