#include "store.h"
#include <Preferences.h>

Settings cfg;
Msg msgs[MAX_MSGS];
int msgCount = 0;

static Preferences prefs;

void storeBegin() {
  prefs.begin("pager", false);
  cfg.ssid = prefs.getString("ssid", "");
  cfg.pass = prefs.getString("pass", "");
  cfg.server = prefs.getString("server", DEFAULT_SERVER);
  cfg.token = prefs.getString("token", "");
  cfg.user = prefs.getString("user", "");
  cfg.volume = prefs.getUChar("vol", 3);
  cfg.tone = prefs.getUChar("tone", 0);
  cfg.light = prefs.getUChar("light", 4);
  cfg.mute = prefs.getBool("mute", false);

  msgCount = prefs.getInt("mcount", 0);
  if (msgCount < 0 || msgCount > MAX_MSGS) msgCount = 0;
  if (prefs.getBytesLength("msgs") != sizeof(Msg) * msgCount) msgCount = 0;
  if (msgCount) prefs.getBytes("msgs", msgs, sizeof(Msg) * msgCount);
}

void saveSettings() {
  prefs.putString("ssid", cfg.ssid);
  prefs.putString("pass", cfg.pass);
  prefs.putString("server", cfg.server);
  prefs.putString("token", cfg.token);
  prefs.putString("user", cfg.user);
  prefs.putUChar("vol", cfg.volume);
  prefs.putUChar("tone", cfg.tone);
  prefs.putUChar("light", cfg.light);
  prefs.putBool("mute", cfg.mute);
}

void saveMsgs() {
  prefs.putInt("mcount", msgCount);
  if (msgCount) prefs.putBytes("msgs", msgs, sizeof(Msg) * msgCount);
  else prefs.remove("msgs");
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
  prefs.clear();
  delay(100);
  ESP.restart();
}

String deviceId() {
  uint64_t mac = ESP.getEfuseMac();
  char buf[8];
  snprintf(buf, sizeof(buf), "%04X", (uint16_t)((mac >> 32) ^ (mac >> 16)));
  return String(buf);
}
