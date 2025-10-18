#include "vscp_device.hpp"
#include <vector>
#include "vscp_glue.hpp"

// Hooks defined in vscp_glue.cpp
extern "C" void VSCP_OnConnect(const String& id, int pin);
bool VSCP_OnConfig(const String& id, const std::map<String,String>& params);

// nahraď existující verzi
static std::vector<int> parsePinsList(String pinsRaw) {
  std::vector<int> pins;
  pinsRaw.trim();
  pinsRaw.replace(" ", "");
  if (!pinsRaw.length()) return pins;

  int start = 0;
  while (start < pinsRaw.length() && pins.size() < 4) {  // <<< AŽ 4 PINY
    int comma = pinsRaw.indexOf(',', start);
    String tok = (comma >= 0) ? pinsRaw.substring(start, comma) : pinsRaw.substring(start);
    if (tok.length()) {
      int v = tok.toInt();
      if (v >= 0) pins.push_back(v);
    }
    if (comma < 0) break;
    start = comma + 1;
  }
  return pins;
}



static bool validEsp32Pin(int p) {
  /*
  if (p >= 6 && p <= 11) return false;   // flash-piny
  return (p >= 0 && p <= 39);
  */
 return true;
}

static bool validIdSAxx(const String& id) {
  return id.length() == 3 &&
         (id.charAt(0) == 'S' || id.charAt(0) == 'A') &&
         isDigit(id.charAt(1)) && isDigit(id.charAt(2));
}




int VSCPDevice::getPin(const String& id) const {
  auto it = idToPin.find(id);
  return (it == idToPin.end()) ? -1 : it->second;
}

String VSCPDevice::readLineNonBlocking() {
  static String buf;
  while (VSCP_STREAM.available()) {
    int b = VSCP_STREAM.read();
    if (b < 0) break;
    char c = (char)b;

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
    VSCP_STREAM.print("?id="); VSCP_STREAM.print(id);
    VSCP_STREAM.print("&status=1");
  } else {
    VSCP_STREAM.print("?status=1");
  }
  for (auto& p : kvs) {
    VSCP_STREAM.print("&"); VSCP_STREAM.print(p.k); VSCP_STREAM.print("=");
    VSCP_STREAM.print(p.v);
  }
  VSCP_STREAM.println();
}

void VSCPDevice::sendERR(const String& id, int code, const String& msg) {
  if (id.length() > 0) {
    VSCP_STREAM.print("?id="); VSCP_STREAM.print(id);
    VSCP_STREAM.print("&status=0");
  } else {
    VSCP_STREAM.print("?status=0");
  }
  VSCP_STREAM.print("&error="); VSCP_STREAM.print(msg);
  VSCP_STREAM.print("&code="); VSCP_STREAM.println(code);
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
  if (type == "CONFIG")      { handleCONFIG(kv); return; } 
  //if (type == "RESET")       { handleRESET(kv); return; } 

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
  auto itId   = kv.find("id");
  auto itPins = kv.find("pins"); // nový parametr (1–2 piny, čárkou)
  auto itPin  = kv.find("pin");  // starý parametr (fallback: 1 pin)

  if (itId == kv.end() || (itPins == kv.end() && itPin == kv.end())) {
    sendERR("", 400, "missing_id_or_pins");
    return;
  }

  String id = itId->second.c_str();

  // Povolit Sxx i Axx (např. S07, A02)
  auto validIdSAxx = [](const String& s){
    return s.length() == 3 &&
           (s.charAt(0) == 'S' || s.charAt(0) == 'A') &&
           isDigit(s.charAt(1)) && isDigit(s.charAt(2));
  };
  if (!validIdSAxx(id)) { sendERR(id, 422, "invalid_id_format"); return; }

  // Rozparsuj piny
  std::vector<int> pins;
  if (itPins != kv.end()) {
    String pinsRaw = itPins->second.c_str();
    pinsRaw.replace(" ", "");
    int start = 0;
    while (start < pinsRaw.length() && pins.size() < 2) {
      int comma = pinsRaw.indexOf(',', start);
      String tok = (comma >= 0) ? pinsRaw.substring(start, comma) : pinsRaw.substring(start);
      if (tok.length()) {
        int v = tok.toInt();
        if (v >= 0) pins.push_back(v);
      }
      if (comma < 0) break;
      start = comma + 1;
    }
  } else {
    int p = String(itPin->second.c_str()).toInt();
    if (p >= 0) pins.push_back(p);
  }

  if (pins.empty() || pins.size() > 4) { sendERR(id, 400, "invalid_pins_count"); return; }

  auto cvalidEsp32Pin = [](int p){
    //if (p >= 6 && p <= 11) return false; // flash piny
    //return (p >= 0 && p <= 39);
    return (p>0);
  };
  for (int p : pins) {
    if (!validEsp32Pin(p)) { sendERR(id, 400, "invalid_pin"); return; }
  }

  // Ulož „hlavní“ pin kvůli kontrole připojení (platí pro Sxx i Axx)
  idToPin[id] = pins[0];

  // Pošli piny do senzoru/aktuátoru (umožňuje dynamickou změnu pinů za běhu)
  VSCP_OnConnect(id, pins);

  sendOK(id);
}


  // *** DYNAMICKÉ PŘEPOJENÍ ***
  // Už neblokuj opakované CONNECT (dřív 409). Prostě přepiš.
  // Pokud si chceš piny držet, změň mapu na std::map<String,std::vector<int>> idToPins;
  // idToPins[id] = pins;

  // vícepinový hook -> uvnitř zavolá Sensor/Actuator::attach(pins)



void VSCPDevice::handleDISCONNECT(const std::map<String,String>& kv) {
  auto itId = kv.find("id");
  if (itId == kv.end()) { sendERR("", 400, "missing_id"); return; }
  String id = itId->second.c_str();
  auto it = idToPin.find(id);
  if (it == idToPin.end()) { sendERR(id, 404, "not_connected"); return; }
  VSCP_OnDisconnect(id);
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

void VSCPDevice::handleCONFIG(const std::map<String,String>& kv) {
  auto itId = kv.find("id");
  if (itId == kv.end()) { sendERR("", 400, "missing_id"); return; }
  String id = itId->second.c_str();

#if VSCP_REQUIRE_CONNECT
  // Vyžaduj CONNECT před CONFIG pro Sxx i Axx
  if (getPin(id) < 0) { 
    sendERR(id, 409, "not_connected");
    return;
  }
#endif

  // Vyrob mapu params bez protokolových klíčů
  std::map<String,String> params;
  for (auto &p : kv) {
    if (p.first == "type" || p.first == "id" || p.first == "pin" || p.first == "api")
      continue;
    params[p.first] = p.second;
  }
  if (params.empty()) { sendERR(id, 400, "missing_params"); return; }

  bool ok = false;
  try {
    ok = VSCP_OnConfig(id, params);
  } catch (...) {
    sendERR(id, 500, "config_exception"); 
    return;
  }

  if (!ok) { sendERR(id, 422, "config_invalid"); return; }
  sendOK(id);
}

