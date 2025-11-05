#pragma once
#include <vector>
#include <WString.h> 

// PŮVODNÍ kompatibilní C-overload s jedním pinem
extern "C" void VSCP_OnConnect(const String& id, int pin);
// C++ overload pro multipiny (bez extern "C")- aktuální
void VSCP_OnConnect(const String& id, const std::vector<int>& pins);
void VSCP_OnDisconnect(const String& id);