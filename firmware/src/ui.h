#pragma once
#include <Arduino.h>
#include "pagerspass.h"

struct Status {
  bool wifi = false;
  int rssi = 0;
  int battery = -1;
  bool server = false;
  RoundState round = R_UNKNOWN;
  String roundName;
};

extern Status st;

void uiWelcome();
void uiSetup(const String &title, const String &l1, const String &big, const String &l2, const String &small);
void uiHome();
void uiMenu(const String &title, const String *items, const String *values, int n, int sel);
void uiMsgList(int sel);
int uiMsg(int i, int scroll);
void uiValue(const String &title, const String &value, const String &hint);
void uiConfirm(const String &question);
void uiInfo(const String *lines, int n);

String fmtTime(uint32_t ts, bool withYear);
