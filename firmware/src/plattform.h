#pragma once
// Das echte Geraet ist ein ESP8266 (WeMos D1 Mini), der Simulator (Wokwi) kann nur ESP32.
// Hier stehen die Unterschiede, damit der Rest vom Code gleich bleibt.

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
using WebServerT = ESP8266WebServer;
using SecureClient = BearSSL::WiFiClientSecure;
#define LOG(x)
#else
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
using WebServerT = WebServer;
using SecureClient = WiFiClientSecure;
#define LOG(x) Serial.println(x)
#endif
