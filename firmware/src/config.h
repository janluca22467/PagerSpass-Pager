#pragma once

#ifdef ESP8266
// WeMos D1 Mini (ESP8266). Jeder Pin ist belegt, auch RX/TX (darum kein Serial).
#define PIN_SCK      14  // D5
#define PIN_MOSI     13  // D7
#define PIN_TFT_CS   15  // D8
#define PIN_TFT_DC   16  // D0
#define PIN_TFT_RST  -1  // RES haengt am RST vom D1 Mini
#define PIN_TFT_BL   5   // D1
#define PIN_BUZZER   4   // D2
#define PIN_BTN_OK   0   // D3
#define PIN_LED      2   // D4, LED gegen 3V3 -> LOW = an
#define PIN_BTN_UP   12  // D6
#define PIN_BTN_DOWN 3   // RX
#define PIN_BTN_BACK 1   // TX
#define PIN_BAT      A0
#define LED_ON LOW
#else
// ESP32 fuer den Simulator
#define PIN_SCK      18
#define PIN_MOSI     23
#define PIN_TFT_CS   5
#define PIN_TFT_DC   16
#define PIN_TFT_RST  17
#define PIN_TFT_BL   4
#define PIN_BTN_OK   27
#define PIN_BTN_UP   25
#define PIN_BTN_DOWN 26
#define PIN_BTN_BACK 32
#define PIN_BUZZER   13
#define PIN_LED      22
#define PIN_BAT      35
#define LED_ON HIGH
#endif

// 1.9" ST7789, 170x320 quer
#define SCREEN_W 320
#define SCREEN_H 170

#ifdef SIM
#define DEFAULT_SERVER "http://host.wokwi.internal:8080"
#else
#define DEFAULT_SERVER "https://pagerspass.de"
#endif

#define TZ_INFO "CET-1CEST,M3.5.0,M10.5.0/3"
#define MAX_MSGS 10
#define MSG_LEN 280
#define ALARM_SECONDS 30
#define LIGHT_SECONDS 15
