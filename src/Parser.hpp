#pragma once
#include <Arduino.h>

struct Param {
  String key;
  String value;
};

String* parseGET(String queryString);
Param* parseKeyValueParams(String queryString, int& count);
