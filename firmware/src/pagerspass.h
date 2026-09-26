#pragma once
#include <Arduino.h>

enum RoundState { R_UNKNOWN, R_WAITING, R_LOBBY, R_ACTIVE };

struct Alarm {
  String id;
  String head;
  String text;
  uint8_t prio = 0;
  uint32_t ts = 0;
};

struct Login {
  bool ok = false;
  bool twoFactor = false;
  String token, user, request, target, err;
  String server;
};

struct PsCallbacks {
  void (*onAlarm)(const Alarm &a);
  void (*onRound)(RoundState st, const String &name, const String &callsign);
  void (*onUnlinked)();
};

Login psLogin(const String &server, const String &user, const String &pass);
Login psLogin2fa(const String &server, const String &request, const String &code);
void psBegin(const PsCallbacks &cb);
void psLoop();
bool psConnected();
void psAck();
