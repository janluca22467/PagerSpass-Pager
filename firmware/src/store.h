#pragma once
#include <Arduino.h>
#include "config.h"

struct Msg {
  uint32_t ts;
  uint8_t read;
  uint8_t prio;
  char head[18];
  char text[MSG_LEN];
};

struct Settings {
  String ssid, pass;
  String server, token, user;
  uint8_t volume = 3;
  uint8_t tone = 0;
  uint8_t light = 4;
  bool mute = false;
};

extern Settings cfg;
extern Msg msgs[MAX_MSGS];
extern int msgCount;

void storeBegin();
void saveSettings();
void saveMsgs();
void addMsg(const Msg &m);
void deleteMsg(int i);
void clearMsgs();
int unreadCount();
void factoryReset();
String deviceId();
