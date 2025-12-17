#pragma once
#define DEBUG

#ifdef DEBUG
  #define DBG_BEGIN(baud) Serial.begin(baud)
  #define DBG_PRINT(x)    Serial.print(x)
  #define DBG_PRINTLN(x)  Serial.println(x)
  #define DBG_PRINTF(...) Serial.printf(__VA_ARGS__)
#else
  #define DBG_BEGIN(baud) ((void)0)
  #define DBG_PRINT(x)    ((void)0)
  #define DBG_PRINTLN(x)  ((void)0)
  #define DBG_PRINTF(...) ((void)0)
#endif