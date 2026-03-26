#include "vscp_device.hpp"
#include <vector>
#include "vscp_glue.hpp"

// Hooks defined in vscp_glue.cpp
extern "C" void VSCP_OnConnect(const String& id, int pin);
bool VSCP_OnConfig(const String& id, const std::map<String,String>& params);

// access globals defined in main.cpp
extern Sensor* SeznamSenzoru[];
extern int PocetSenzoru;
extern Actuator* SeznamAktuatoru[];
extern int PocetAktuatoru;

// Odstraní neplatné znaky (ponechá jen tisknutelné ASCII 32-126), ořízne před '?' a trim
static String ensureStartsWithQuestion(String s) {
  // Strip non-printable chars (keep only ASCII 32-126)
  String out;
  out.reserve(s.length());
  for (size_t i = 0; i < s.length(); i++) {
    char c = s[i];
    if (c >= 32 && c <= 126) out += c;
  }
  out.trim();
  
  if (!out.length()) return String();
  if (out.charAt(0) == '?') return out;
  
  int pos = out.indexOf('?');
  return (pos >= 0) ? out.substring(pos) : String();
}

// Rozparsuje řetězec s piny (oddělené čárkami)
static std::vector<int> parsePinsList(String pinsRaw) {
  std::vector<int> pins;
  pinsRaw.trim();
  pinsRaw.replace(" ", "");
  if (!pinsRaw.length()) return pins;

  int start = 0;
  while (start < pinsRaw.length() && pins.size() < 4) {  // max 4 piny (více nepodporováno)
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

// Validace formátu identifikátoru typu Sxx / Axx
static bool validIdSAxx(const String& id) {
  return id.length() == 3 &&
         (id.charAt(0) == 'S' || id.charAt(0) == 'A') &&
         isDigit(id.charAt(1)) && isDigit(id.charAt(2));
}


// Vrátí uložený hlavní pin pro dané id (nebo -1 pokud není připojeno)
int VSCPDevice::getPin(const String& id) const {
  auto it = idToPin.find(id);
  return (it == idToPin.end()) ? -1 : it->second;
}

// Čte zprávu ze sériového portu bez blokování, vrací jednu kompletní linku
String VSCPDevice::readLineNonBlocking(Stream* stream) {
  static String buf_usb;
  static String buf_hw;
  String& buf = (stream == &Serial) ? buf_usb : buf_hw;
  
  while (stream->available()) {
    int b = stream->read();
    if (b < 0) break;

    // Akceptuj LF/CR/NUL jako terminátory
    if (b == 10 || b == 13 || b == 0) {
      if (buf.length()) {
        String out = buf;
        buf = "";
        out = ensureStartsWithQuestion(out);
        if (out.length()) {
          //Serial.write(out.c_str());
          return out;}
        // jinak ignoruj a pokračuj
      }
    } else {
      buf += (char)b;
    }
  }
  return String();
}

// Parsuje query string (?key=val&...) na mapu klíč→hodnota
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

// Odešle úspěšnou odpověď (status=1) s volitelnými páry k:v
void VSCPDevice::sendOK(const String& id, const std::vector<KV>& kvs) {
  if (!activeStream) return;
  if (id.length() > 0) {
    activeStream->print("?id="); activeStream->print(id);
    activeStream->print("&status=1");
  } else {
    activeStream->print("?status=1");
  }
  for (auto& p : kvs) {
    activeStream->print("&"); activeStream->print(p.k); activeStream->print("=");
    activeStream->print(p.v);
  }
  activeStream->println();
}

// Odešle chybovou odpověď se stavovým kódem a textem chyby(volitelné-třeba sjednotit error msg)
void VSCPDevice::sendERR(const String& id,const String& msg) {
  if (!activeStream) return;
  if (id.length() > 0) {
    activeStream->print("?id="); activeStream->print(id);
    activeStream->print("&status=0");
  } else {
    activeStream->print("?status=0");
  }
  activeStream->print("&error="); activeStream->print(msg);
}

// Zpracuje příchozí požadavek (line) a odkáže na konkrétní handler
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
  if (type == "RESET")       { handleRESET(kv); return; } 

  auto itId = kv.find("id");
  String id = (itId != kv.end()) ? String(itId->second.c_str()) : String(""); 
  sendERR(id,"Unknown type");
}

// Pollovací smyčka: čte obě sériové linky (USB + HW UART) non-blocking a volá handler
void VSCPDevice::poll() {
  // Zkus USB Serial
  if (Serial.available()) {
    activeStream = &Serial;
    String line = readLineNonBlocking(&Serial);
    if (line.length()) {
      handleRequest(line);
      return;
    }
  }
  
  // Zkus HW UART
  if (VirtualUART2.available()) {
    activeStream = &VirtualUART2;
    String line = readLineNonBlocking(&VirtualUART2);
    if (line.length()) {
      handleRequest(line);
      return;
    }
  }
}

// Inicializační handler: kontrola verze API a odpověď OK
void VSCPDevice::handleINIT(const std::map<String,String>& kv) {
  auto itApi = kv.find("api");
  if (itApi != kv.end()) {
    String want = itApi->second.c_str();
    if (want != String(VSCP_API_VERSION)) {
      sendERR("",String("Api mismatch have=") + VSCP_API_VERSION + " want=" + want);
      return;
    }
  }
  sendOK(""); 
  
}

// Zpracuje CONNECT požadavek, validuje id a piny, uloží mapu a volá hook
void VSCPDevice::handleCONNECT(const std::map<String,String>& kv) {
  auto itId   = kv.find("id");
  auto itPins = kv.find("pins");
  auto itPin  = kv.find("pin");

  if (itId == kv.end() || (itPins == kv.end() && itPin == kv.end())) {
    sendERR("","Missing ID or pins");
    return;
  }

  String id = itId->second.c_str();

  // validace formátu id (Sxx / Axx)
  auto validIdSAxx = [](const String& s){
    return s.length() == 3 &&
           (s.charAt(0) == 'S' || s.charAt(0) == 'A') &&
           isDigit(s.charAt(1)) && isDigit(s.charAt(2));
  };
  if (!validIdSAxx(id)) { sendERR(id,"Invalid id format"); return; }

  // Parser pinů 
  std::vector<int> pins;
  if (itPins != kv.end()) {
    pins = parsePinsList(String(itPins->second.c_str()));
  } else {
    int p = String(itPin->second.c_str()).toInt();
    if (p >= 0) pins.push_back(p);
  }

  if (pins.empty() || pins.size() > 4) { sendERR(id,"Invalid pins count"); return; }

  // validace pinů
  auto cvalidEsp32Pin = [](int p){
    if (p < 0) return false;
    return true;
  };
  
  for (int p : pins) {
    if (!cvalidEsp32Pin(p)) { sendERR(id,"Invalid pin"); return; }
  }

  // Ulož hlavní pin (první v seznamu)
  idToPin[id] = pins[0];

  // Pošli piny do senzoru/aktuátoru (umožňuje dynamickou změnu pinů za běhu)
  VSCP_OnConnect(id, pins);

  sendOK(id);
}

// Zpracuje DISCONNECT: volá hook, smaže uložený pin a odpoví OK
void VSCPDevice::handleDISCONNECT(const std::map<String,String>& kv) {
  auto itId = kv.find("id");
  if (itId == kv.end()) { sendERR("","Missing ID"); return; }
  String id = itId->second.c_str();
  auto it = idToPin.find(id);
  if (it == idToPin.end()) { sendERR(id,"Not connected"); return; }
  VSCP_OnDisconnect(id);
  idToPin.erase(it);
  sendOK(id);
}

// Zpracuje RESET: zavolá registrovaný reset handler a resetuje zařízení
void VSCPDevice::handleRESET(const std::map<String,String>& kv) {
  auto itId = kv.find("id");
  if (itId == kv.end()) { sendERR("","Missing ID"); return; }
  String id = itId->second.c_str();

  // valid id forms: "Sxx", "Axx", "S*", "A*"
  if (id.length() == 2 && id.charAt(1) == '*') {
    char g = id.charAt(0);
    if (g == 'S') {
      for (int i = 0; i < PocetSenzoru; ++i) {
        if (SeznamSenzoru[i]) SeznamSenzoru[i]->reset();
      }
      sendOK(id);
      return;
    } else if (g == 'A') {
      for (int i = 0; i < PocetAktuatoru; ++i) {
        if (SeznamAktuatoru[i]) SeznamAktuatoru[i]->reset();
      }
      sendOK(id);
      return;
    } else {
      sendERR(id,"Invalid ID format");
      return;
    }
  }

  if (id.length() != 3 || !(isDigit(id.charAt(1)) && isDigit(id.charAt(2)))) {
    sendERR(id,"Invalid ID format");
    return;
  }

  char group = id.charAt(0);
  int idx = (id.charAt(1) - '0') * 10 + (id.charAt(2) - '0');

  if (group == 'S') {
    if (idx < 0 || idx >= PocetSenzoru) { sendERR(id,"Not found"); return; }
    if (SeznamSenzoru[idx]) SeznamSenzoru[idx]->reset();
    sendOK(id);
    return;
  } else if (group == 'A') {
    if (idx < 0 || idx >= PocetAktuatoru) { sendERR(id,"Not found"); return; }
    if (SeznamAktuatoru[idx]) SeznamAktuatoru[idx]->reset();
    sendOK(id);
    return;
  } else {
    sendERR(id,"Invalid ID format");
    return;
  }
}

// Zpracuje UPDATE: zavolá registrovaný update handler a pošle data
void VSCPDevice::handleUPDATE(const std::map<String,String>& kv) {
  auto itId = kv.find("id");
  if (itId == kv.end()) { sendERR("","Missing ID"); return; }
  String id = itId->second.c_str();

  auto hit = updateHandlers.find(id);
  if (hit == updateHandlers.end()) { sendERR(id,"Unknown ID"); return; }

#if VSCP_REQUIRE_CONNECT
  if (getPin(id) < 0) { sendERR(id,"Not connected"); return; }
#endif

  int pin = getPin(id);
  std::vector<KV> data;
  try {
    data = hit->second(id, pin);
  } catch (...) {
    sendERR(id,"Update exception");
    return;
  }

  if (data.empty()) { sendERR(id,"No content"); return; }
  sendOK(id, data);
}

// Zpracuje CONFIG: vyextrahuje parametry a zavolá VSCP_OnConfig hook
void VSCPDevice::handleCONFIG(const std::map<String,String>& kv) {
  auto itId = kv.find("id");
  if (itId == kv.end()) { sendERR("","Missing ID"); return; }
  String id = itId->second.c_str();

#if VSCP_REQUIRE_CONNECT
  // Vyžaduj CONNECT před CONFIG pro Sxx i Axx
  if (getPin(id) < 0) { 
    sendERR(id,"Not connected");
    return;
  }
#endif

  // Vyrob mapu params(pro CONFIG parametry) bez protokolových klíčů
  std::map<String,String> params;
  for (auto &p : kv) {
    if (p.first == "type" || p.first == "id" || p.first == "pin" || p.first == "api")
      continue;
    params[p.first] = p.second;
  }
  if (params.empty()) { sendERR(id,"Missing params"); return; }

  bool ok = false;
  try {
    ok = VSCP_OnConfig(id, params);
  } catch (...) {
    sendERR(id,"Config exception"); 
    return;
  }

  if (!ok) { sendERR(id,"Invalid config"); return; }
  sendOK(id);
}