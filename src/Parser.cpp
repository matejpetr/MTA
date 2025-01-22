#include "Parser.hpp"
#include <Arduino.h>


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