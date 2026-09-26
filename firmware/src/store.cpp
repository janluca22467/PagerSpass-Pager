#include "store.h"
#include <LittleFS.h>
#include <ArduinoJson.h>

Settings cfg;
Msg msgs[MAX_MSGS];
int msgCount = 0;

static void mount() {
#ifdef ESP8266
  if (!LittleFS.begin()) {
    LittleFS.format();
    LittleFS.begin();
  }
#else
  LittleFS.begin(true);
#endif
}

void storeBegin() {
  mount();
  JsonDocument doc;
  File f = LittleFS.open("/cfg.json", "r");
  if (f) {
    deserializeJson(doc, f);
    f.close();
  }
#ifdef SIM
  cfg.ssid = doc["ssid"] | "Wokwi-GUEST";
#else
  cfg.ssid = doc["ssid"] | "";
#endif
  cfg.pass = doc["pass"] | "";
  cfg.server = doc["server"] | DEFAULT_SERVER;
  cfg.token = doc["token"] | "";
  cfg.user = doc["user"] | "";
  cfg.volume = doc["vol"] | 3;
  cfg.tone = doc["tone"] | 0;
  cfg.light = doc["light"] | 4;
  cfg.mute = doc["mute"] | false;

  f = LittleFS.open("/msgs.bin", "r");
  if (f) {
    msgCount = f.size() / sizeof(Msg);
    if (msgCount > MAX_MSGS || f.size() % sizeof(Msg)) msgCount = 0;
    f.read((uint8_t *)msgs, sizeof(Msg) * msgCount);
    f.close();
  }
}

void saveSettings() {
  JsonDocument doc;
  doc["ssid"] = cfg.ssid;
  doc["pass"] = cfg.pass;
  doc["server"] = cfg.server;
  doc["token"] = cfg.token;
  doc["user"] = cfg.user;
  doc["vol"] = cfg.volume;
  doc["tone"] = cfg.tone;
  doc["light"] = cfg.light;
  doc["mute"] = cfg.mute;
  File f = LittleFS.open("/cfg.json", "w");
  if (!f) return;
  serializeJson(doc, f);
  f.close();
}

void saveMsgs() {
  File f = LittleFS.open("/msgs.bin", "w");
  if (!f) return;
  f.write((const uint8_t *)msgs, sizeof(Msg) * msgCount);
  f.close();
}

void addMsg(const Msg &m) {
  if (msgCount == MAX_MSGS) msgCount--;
  memmove(&msgs[1], &msgs[0], sizeof(Msg) * msgCount);
  msgs[0] = m;
  msgCount++;
  saveMsgs();
}

void deleteMsg(int i) {
  if (i < 0 || i >= msgCount) return;
  memmove(&msgs[i], &msgs[i + 1], sizeof(Msg) * (msgCount - i - 1));
  msgCount--;
  saveMsgs();
}

void clearMsgs() {
  msgCount = 0;
  saveMsgs();
}

int unreadCount() {
  int n = 0;
  for (int i = 0; i < msgCount; i++) if (!msgs[i].read) n++;
  return n;
}

void factoryReset() {
  LittleFS.remove("/cfg.json");
  LittleFS.remove("/msgs.bin");
  delay(100);
  ESP.restart();
}

String deviceId() {
#ifdef ESP8266
  uint32_t id = ESP.getChipId();
#else
  uint64_t mac = ESP.getEfuseMac();
  uint32_t id = (uint32_t)((mac >> 32) ^ (mac >> 16));
#endif
  char buf[8];
  snprintf(buf, sizeof(buf), "%04X", (uint16_t)id);
  return String(buf);
}
