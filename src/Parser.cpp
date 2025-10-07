#include "Parser.hpp"
#include <Arduino.h>

// Statické pole pro výsledky (max 10 parametrů)
static Param parsedParams[10];

static bool isDigits(const String& s) {
  for (size_t i = 0; i < s.length(); ++i) {
    if (!isDigit(s[i])) return false;
  }
  return s.length() > 0;
}

String makeId(char prefix, int idx) {
  // dvouciferné nulování: 0..9 -> 00..09, 10.. -> 10..
  String s = (idx < 10) ? "0" + String(idx) : String(idx);
  return String(prefix) + s;
}

// --- GET dotaz ---
String* parseGET(String queryString) {
  static String values[3]; // type, id, ResponseAll

  if (queryString.startsWith("?")) {
    queryString.remove(0, 1);
  }

  values[0] = "";        // type
  values[1] = "";        // id (např. "S00", "A05", "S*", "A*")
  values[2] = "false";   // ResponseAll (pokud je v query * kdekoliv)

  int start = 0;
  while (start < queryString.length()) {
    int end = queryString.indexOf('&', start);
    if (end == -1) end = queryString.length();

    String pair = queryString.substring(start, end);
    int delimiter = pair.indexOf('=');

    if (delimiter != -1) {
      String key = pair.substring(0, delimiter);
      String value = pair.substring(delimiter + 1);

      if (key == "type") values[0] = value;
      else if (key == "id") values[1] = value;

      if (value.indexOf('*') != -1) values[2] = "true";
    }

    start = end + 1;
  }

  return values;
}

bool parseUnifiedId(String idStr, char& prefix, int& index) {
  idStr.trim();
  idStr.toUpperCase();

  if (idStr.length() < 2) return false;

  prefix = idStr[0];
  if (prefix != 'S' && prefix != 'A') return false;

  if (idStr.length() == 2 && idStr[1] == '*') {
    index = -1;
    return true;
  }

  String digits = idStr.substring(1);
  if (!isDigits(digits)) return false;

  index = digits.toInt();
  return true;
}

// --- Config parametry (aktuátory+senzory) ---
Param* parseKeyValueParams(String queryString, int& count) {
  count = 0;

  if (queryString.startsWith("?")) {
    queryString.remove(0, 1);
  }

  int start = 0;
  while (start < queryString.length() && count < 10) {
    int end = queryString.indexOf('&', start);
    if (end == -1) end = queryString.length();

    String pair = queryString.substring(start, end);
    int delimiter = pair.indexOf('=');

    if (delimiter != -1) {
      String key = pair.substring(0, delimiter);
      String value = pair.substring(delimiter + 1);

      if (key != "type" && key != "id") {
        parsedParams[count].key = key;
        parsedParams[count].value = value;
        count++;
      }
    }

    start = end + 1;
  }

  return parsedParams;
}
