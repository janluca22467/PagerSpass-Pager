#pragma once
#include <Arduino.h>

enum SetupState { SS_WIFI, SS_WIFI_TRY, SS_WIFI_FAIL, SS_ACCOUNT, SS_LINKING, SS_LINK_FAIL, SS_CODE, SS_DONE };

extern SetupState setupState;
extern String setupError;
extern String setupHint;

void portalBegin(bool withAP);
void portalLoop();
void portalStop();
bool portalRunning();
String apName();

bool takeWifiRequest(String &ssid, String &pass);
bool takeAccountRequest(String &server, String &user, String &pass);
bool takeCodeRequest(String &code);
