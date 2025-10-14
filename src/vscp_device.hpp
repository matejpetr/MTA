#pragma once
#include <Arduino.h>
#include <functional>
#include <map>
#include <vector>
#include <utility>   // std::move
#include "vscp_port.hpp"
#include "sensor.hpp"

#ifndef VSCP_API_VERSION
#define VSCP_API_VERSION "1.0"
#endif

#ifndef VSCP_REQUIRE_CONNECT
#define VSCP_REQUIRE_CONNECT 1
#endif

//struct KV { String k; String v; };

class VSCPDevice {
public:
  typedef std::function< std::vector<KV>(const String&, int) > UpdateFn;

  void onUpdate(const String& id, UpdateFn fn) { updateHandlers[id] = std::move(fn); }
  int  getPin(const String& id) const;
  void poll();

  static void sendOK(const String& id, const std::vector<KV>& kvs = std::vector<KV>());
  static void sendERR(const String& id, int code, const String& msg);

private:
  String readLineNonBlocking();
  static std::map<String,String> parseQuery(const String& q);
  void handleRequest(const String& line);

  void handleINIT(const std::map<String,String>& kv);
  void handleCONNECT(const std::map<String,String>& kv);
  void handleDISCONNECT(const std::map<String,String>& kv);
  void handleUPDATE(const std::map<String,String>& kv);
  void handleCONFIG(const std::map<String,String>& kv);  

  std::map<String, UpdateFn> updateHandlers; // id -> callback
  std::map<String, int>      idToPin;        // id -> pin
};
