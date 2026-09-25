#pragma once
#include <Arduino.h>

enum RoundState { R_UNKNOWN, R_WAITING, R_ACTIVE };

struct Alarm {
  String id;
  String text;
  uint8_t adr = 1;
  uint8_t prio = 0;
  uint32_t ts = 0;
};

struct PsCallbacks {
  void (*onAlarm)(const Alarm &a);
  void (*onRound)(RoundState st, const String &name);
  void (*onUnlinked)();
};

bool psLink(const String &server, const String &user, const String &pass, String &token, String &err);
void psBegin(const PsCallbacks &cb);
void psLoop();
bool psConnected();
void psAck(const String &id);
void psSendStatus(int battery, int rssi);
