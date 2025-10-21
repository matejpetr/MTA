#pragma once
#include <Arduino.h>

//přepínač Sériové linky
#ifndef USE_HW_UART
#define USE_HW_UART 0 // 0 = USB Serial, 1 = Serial1 (HW)
#endif

#ifndef VSCP_BAUD
#define VSCP_BAUD 115200
#endif



#if USE_HW_UART
  #ifndef VSCP_RX_PIN
  #define VSCP_RX_PIN 44
  #endif
  #ifndef VSCP_TX_PIN
  #define VSCP_TX_PIN 43
  #endif

  #define VSCP_STREAM Serial1
#else
  #define VSCP_STREAM Serial
#endif
