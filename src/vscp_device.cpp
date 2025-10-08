#include "vscp_device.hpp"
extern "C" void VSCP_OnConnect(const String& id, int pin);  // forward deklarace v global scope


String VSCPDevice::readLineNonBlocking() {
  static String buf;
  while (Serial.available()) {
    int b = Serial.read();
    if (b < 0) break;
    char c = (char)b;

    // 10 = '\n' (LF), 13 = '\r' (CR) — vyhneme se problémovým uvozovkám
    if (b == 10 || b == 13) {
      if (buf.length()) { String out = buf; buf = ""; return out; }
    } else {
      buf += c;
    }
  }
  return String();
}

std::map<String,String> VSCPDevice::parseQuery(const String& q) {
  std::map<String,String> kv;
  int i = (q.length() && q[0]=='?') ? 1 : 0;
  while (i < q.length()) {
    int amp = q.indexOf('&', i); if (amp < 0) amp = q.length();
    int eq  = q.indexOf('=', i);
    if (eq > i && eq < amp) {
      String key = q.substring(i, eq);
      String val = q.substring(eq+1, amp);
      kv[key] = val;
    }
    i = amp + 1;
  }
  return kv;
}

void VSCPDevice::sendOK(const String& id, const std::vector<KV>& kvs) {
  if (id.length() > 0) {
    Serial.print("?id="); Serial.print(id);
    Serial.print("&status=1");
  } else {
    Serial.print("?status=1");
  }
  for (auto& p : kvs) {
    Serial.print("&"); Serial.print(p.k); Serial.print("=");
    Serial.print(p.v);
  }
  Serial.println();
}

void VSCPDevice::sendERR(const String& id, int code, const String& msg) {
  if (id.length() > 0) {
    Serial.print("?id="); Serial.print(id);
    Serial.print("&status=0");
  } else {
    Serial.print("?status=0");
  }
  Serial.print("&error="); Serial.print(msg);
  Serial.print("&code="); Serial.println(code);
}

void VSCPDevice::handleRequest(const String& line) {
  auto kv = parseQuery(line);
  auto itType = kv.find("type");
  if (itType == kv.end()) return;
  String type = itType->second.c_str();

  if (type == "INIT")        { handleINIT(kv); return; }
  if (type == "CONNECT")     { handleCONNECT(kv); return; }
  if (type == "DISCONNECT")  { handleDISCONNECT(kv); return; }
  if (type == "UPDATE")      { handleUPDATE(kv); return; }

  auto itId = kv.find("id");
  String id = (itId != kv.end()) ? String(itId->second.c_str()) : String("");
  sendERR(id, 400, "unknown_type");
}

void VSCPDevice::poll() {
  String line = readLineNonBlocking();
  if (!line.length()) return;
  handleRequest(line);
}

void VSCPDevice::handleINIT(const std::map<String,String>& kv) {
  auto itApi = kv.find("api");
  if (itApi != kv.end()) {
    String want = itApi->second.c_str();
    if (want != String(VSCP_API_VERSION)) {
      sendERR("", 409, String("api_mismatch_have=") + VSCP_API_VERSION + "_want=" + want);
      return;
    }
  }
  sendOK("");
}

void VSCPDevice::handleCONNECT(const std::map<String,String>& kv) {
  auto itId  = kv.find("id");
  auto itPin = kv.find("pin");
  if (itId == kv.end() || itPin == kv.end()) {
    sendERR("", 400, "missing_id_or_pin");
    return;
  }
  String id = itId->second.c_str();
  int pin   = String(itPin->second.c_str()).toInt();

  if (pin < 0) { sendERR(id, 400, "invalid_pin"); return; }
  if (idToPin.find(id) != idToPin.end()) { sendERR(id, 409, "already_connected"); return; }

  idToPin[id] = pin;
  VSCP_OnConnect(id, pin);
  sendOK(id);
}

void VSCPDevice::handleDISCONNECT(const std::map<String,String>& kv) {
  auto itId = kv.find("id");
  if (itId == kv.end()) { sendERR("", 400, "missing_id"); return; }
  String id = itId->second.c_str();
  auto it = idToPin.find(id);
  if (it == idToPin.end()) { sendERR(id, 404, "not_connected"); return; }
  idToPin.erase(it);
  sendOK(id);
}

void VSCPDevice::handleUPDATE(const std::map<String,String>& kv) {
  auto itId = kv.find("id");
  if (itId == kv.end()) { sendERR("", 400, "missing_id"); return; }
  String id = itId->second.c_str();

  auto hit = updateHandlers.find(id);
  if (hit == updateHandlers.end()) { sendERR(id, 404, "unknown_id"); return; }

#if VSCP_REQUIRE_CONNECT
  if (getPin(id) < 0) { sendERR(id, 409, "not_connected"); return; }
#endif

  int pin = getPin(id);
  std::vector<KV> data;
  try {
    data = hit->second(id, pin);
  } catch (...) {
    sendERR(id, 500, "update_exception");
    return;
  }

  if (data.empty()) { sendERR(id, 204, "no_content"); return; }
  sendOK(id, data);
}
/*
int VSCPDevice::getPin(const String& id) const {
  auto it = idToPin.find(id);
  return (it == idToPin.end()) ? -1 : it->second;
}
  */