#pragma once
#include <Arduino.h>
#include <HardwareSerial.h>

// přepínač Sériové linky
#ifndef USE_HW_UART
#define USE_HW_UART 0 // 0 = USB Serial, 1 = Serial1 (HW)
#endif

#ifndef VSCP_BAUD
#define VSCP_BAUD 115200
#endif

extern HardwareSerial VirtualUART2;


// definice HW pinů (nevyužito)
#if USE_HW_UART
  #ifndef VSCP_RX_PIN
  #define VSCP_RX_PIN 18  //16
  #endif
  #ifndef VSCP_TX_PIN
  #define VSCP_TX_PIN 17   //17
  #endif

  #define VSCP_STREAM VirtualUART2
#else
  #define VSCP_STREAM Serial
#endif
