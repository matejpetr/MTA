#pragma once
#include <vector>
#include <WString.h> // nebo kde bereš Arduino String

// NOVÝ C++ overload pro multipiny (bez extern "C")
void VSCP_OnConnect(const String& id, const std::vector<int>& pins);

// PŮVODNÍ kompatibilní C-overload s jedním pinem
extern "C" void VSCP_OnConnect(const String& id, int pin);

// NOVÉ:
void VSCP_OnDisconnect(const String& id);