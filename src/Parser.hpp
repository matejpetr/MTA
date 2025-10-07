#pragma once
#include <Arduino.h>

struct Param {
  String key;
  String value;
};

String* parseGET(String queryString);
static bool isDigits(const String& s);
Param* parseKeyValueParams(String queryString, int& count);
bool parseUnifiedId(String idStr, char& prefix, int& index);
String makeId(char prefix, int idx);