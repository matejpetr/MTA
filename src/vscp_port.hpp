#pragma once
#include <Arduino.h>
#include <HardwareSerial.h>

#ifndef VSCP_BAUD
#define VSCP_BAUD 115200
#endif

extern HardwareSerial VirtualUART2;

#ifndef VSCP_RX_PIN
#define VSCP_RX_PIN 18
#endif
#ifndef VSCP_TX_PIN
#define VSCP_TX_PIN 17
#endif
