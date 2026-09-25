#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include "config.h"
#include "store.h"
#include "display.h"
#include "buttons.h"
#include "sound.h"
#include "portal.h"
#include "pagerspass.h"
#include "ui.h"

enum Mode { M_SETUP_WIFI, M_SETUP_ACCOUNT, M_RUN };
enum Screen { S_HOME, S_MENU, S_LIST, S_MSG, S_ALARM, S_VALUE, S_CONFIRM, S_INFO };
enum Menu { MN_MSGS, MN_VOL, MN_TONE, MN_LIGHT, MN_MUTE, MN_INFO, MN_WIFI, MN_UNLINK, MN_RESET, MN_COUNT };
enum Confirm { C_DELETE, C_DELETE_ALL, C_WIFI, C_UNLINK, C_RESET };

static Mode mode;
static Screen screen = S_HOME;
static bool dirty = true;

static int menuSel = 0, listSel = 0, msgIdx = 0, msgScroll = 0, msgLines = 0;
static int editVal = 0;
static Menu editing;
static Confirm confirming;
static Screen confirmBack;

static String alarmId;
static uint32_t alarmStart = 0;
static uint32_t lastInput = 0;
static bool lightOn = true;
static uint32_t lastTick = 0, lastStatus = 0;
static uint32_t wifiTryStart = 0, doneAt = 0;
static String pendingSsid, pendingPass;

static int readBattery() {
  uint32_t mv = 0;
  for (int i = 0; i < 8; i++) mv += analogReadMilliVolts(PIN_BAT);
  mv = mv / 8 * 2;
  if (mv < 2500) return -1;
  return constrain(map(mv, 3300, 4150, 0, 100), 0, 100);
}

static void wake() {
  lastInput = millis();
  if (!lightOn) {
    lightOn = true;
    backlight(cfg.light);
  }
}

static void go(Screen s) {
  screen = s;
  dirty = true;
}

static void showSetupWifi() {
  if (setupState == SS_WIFI_FAIL)
    uiSetup("Einrichtung 1/2", "Verbindung fehlgeschlagen. Verbinde dich mit dem WLAN", apName(), "und versuche es nochmal.", setupError);
  else
    uiSetup("Einrichtung 1/2", "Willkommen! Verbinde dich mit dem WLAN", apName(), "Die Einrichtung öffnet sich dann von selbst.", "Falls nicht: http://192.168.4.1 öffnen");
}

static void showSetupAccount() {
  String ip = WiFi.localIP().toString();
  switch (setupState) {
    case SS_LINKING:
      uiSetup("Einrichtung 2/2", "Verknüpfe mit PagerSpass...", "", "", "");
      break;
    case SS_LINK_FAIL:
      uiSetup("Einrichtung 2/2", "Verknüpfen fehlgeschlagen:", "", setupError, "http://" + ip);
      break;
    case SS_DONE:
      uiSetup("Fertig", "Dein Pager ist verknüpft.", "Hallo " + cfg.user + "!", "", "");
      break;
    default:
      uiSetup("Einrichtung 2/2", "PagerSpass verknüpfen", "", "Gib jetzt im Browser deine PagerSpass Daten ein.", "Im Heimnetz: http://" + ip);
  }
}

static void onAlarm(const Alarm &a) {
  Msg m = {};
  m.ts = a.ts ? a.ts : (uint32_t)time(nullptr);
  m.adr = a.adr;
  m.prio = a.prio;
  m.read = 0;
  strlcpy(m.text, a.text.c_str(), sizeof(m.text));
  addMsg(m);

  alarmId = a.id;
  alarmStart = millis();
  msgIdx = 0;
  msgScroll = 0;
  playAlarm(cfg.tone, true);
  ledBlink(true);
  lightOn = true;
  lastInput = millis();
  backlight(5);
  go(S_ALARM);
}

static void onRound(RoundState r, const String &name) {
  bool started = r == R_ACTIVE && st.round != R_ACTIVE;
  st.round = r;
  st.roundName = name;
  if (started && screen != S_ALARM) playBeep();
  if (screen == S_HOME) dirty = true;
}

static void onUnlinked() {
  cfg.token = "";
  saveSettings();
  ESP.restart();
}

static void startRun() {
  mode = M_RUN;
  WiFi.setAutoReconnect(true);
  configTzTime(TZ_INFO, "pool.ntp.org", "time.google.com");
  psBegin({onAlarm, onRound, onUnlinked});
  backlight(cfg.light);
  lastInput = millis();
  go(S_HOME);
}

static void startAccountSetup(bool withAP) {
  mode = M_SETUP_ACCOUNT;
  setupState = SS_ACCOUNT;
  portalBegin(withAP);
  dirty = true;
}

static bool connectSaved() {
  WiFi.mode(WIFI_STA);
  WiFi.setHostname(apName().c_str());
  WiFi.begin(cfg.ssid.c_str(), cfg.pass.c_str());
  uiSetup("PagerSpass", "Verbinde mit WLAN", cfg.ssid, "", "");
  uint32_t t = millis();
  while (millis() - t < 20000) {
    if (WiFi.status() == WL_CONNECTED) return true;
    delay(100);
  }
  return false;
}

// ---------- Einrichtung ----------

static void setupLoop() {
  portalLoop();

  String a, b, c;
  if (takeWifiRequest(a, b)) {
    pendingSsid = a;
    pendingPass = b;
    WiFi.disconnect();
    WiFi.begin(a.c_str(), b.c_str());
    wifiTryStart = millis();
    uiSetup("Einrichtung 1/2", "Verbinde mit WLAN", a, "", "");
  }

  if (setupState == SS_WIFI_TRY) {
    if (WiFi.status() == WL_CONNECTED) {
      cfg.ssid = pendingSsid;
      cfg.pass = pendingPass;
      saveSettings();
      setupError = "";
      if (cfg.token.length()) {
        setupState = SS_DONE;
        doneAt = millis();
        mode = M_SETUP_ACCOUNT;
      } else {
        startAccountSetup(false);
      }
      dirty = true;
    } else if (millis() - wifiTryStart > 15000) {
      WiFi.disconnect();
      setupError = WiFi.status() == WL_NO_SSID_AVAIL ? "WLAN nicht gefunden" : "Passwort falsch?";
      setupState = SS_WIFI_FAIL;
      WiFi.scanNetworks(true);
      dirty = true;
    }
  }

  if (takeAccountRequest(a, b, c)) {
    dirty = true;
    showSetupAccount();
    String token, err;
    if (psLink(a, b, c, token, err)) {
      cfg.server = a;
      cfg.user = b;
      cfg.token = token;
      saveSettings();
      setupState = SS_DONE;
      doneAt = millis();
      playBeep();
    } else {
      setupError = err;
      setupState = SS_LINK_FAIL;
    }
    dirty = true;
  }

  if (setupState == SS_DONE && millis() - doneAt > 6000) {
    portalStop();
    startRun();
    return;
  }

  if (dirty) {
    dirty = false;
    if (mode == M_SETUP_WIFI) showSetupWifi();
    else showSetupAccount();
  }

  if (buttonsRead() == B_BACK_LONG) ESP.restart();
}

// ---------- Menue ----------

static String menuValue(int i) {
  switch (i) {
    case MN_MSGS: return String(msgCount);
    case MN_VOL: return String(cfg.volume);
    case MN_TONE: return toneName(cfg.tone);
    case MN_LIGHT: return String(cfg.light);
    case MN_MUTE: return cfg.mute ? "An" : "Aus";
    default: return "";
  }
}

static void drawMenu() {
  static const char *names[MN_COUNT] = {"Nachrichten", "Lautstärke", "Alarmton", "Helligkeit", "Stumm", "Info", "WLAN ändern", "Konto trennen", "Werkseinstellungen"};
  String items[MN_COUNT], values[MN_COUNT];
  for (int i = 0; i < MN_COUNT; i++) {
    items[i] = names[i];
    values[i] = menuValue(i);
  }
  uiMenu("Menü", items, values, MN_COUNT, menuSel);
}

static void drawValue() {
  switch (editing) {
    case MN_VOL: uiValue("Lautstärke", editVal == 0 ? "Aus" : String(editVal) + " / 5", ""); break;
    case MN_TONE: uiValue("Alarmton", toneName(editVal), ""); break;
    case MN_LIGHT: uiValue("Helligkeit", String(editVal) + " / 5", ""); break;
    default: break;
  }
}

static void drawInfo() {
  String l[10];
  int bat = st.battery;
  l[0] = "Gerät: " + apName();
  l[1] = "Firmware: " FW_VERSION;
  l[2] = "WLAN: " + cfg.ssid + (st.wifi ? String(String(" (") + st.rssi + " dBm)") : String(" (getrennt)"));
  l[3] = "IP: " + WiFi.localIP().toString();
  l[4] = "Server: " + cfg.server;
  l[5] = "Konto: " + cfg.user;
  l[6] = String("PagerSpass: ") + (st.server ? "verbunden" : "nicht verbunden");
  l[7] = "Akku: " + (bat >= 0 ? String(String(bat) + " %") : String("USB"));
  l[8] = "Nachrichten: " + String(msgCount) + " (" + unreadCount() + " neu)";
  l[9] = "Laufzeit: " + String(millis() / 60000) + " min";
  uiInfo(l, 10);
}

static void askConfirm(Confirm c) {
  confirming = c;
  confirmBack = screen;
  go(S_CONFIRM);
}

static void doConfirm() {
  switch (confirming) {
    case C_DELETE:
      deleteMsg(msgIdx);
      listSel = min(listSel, max(0, msgCount - 1));
      go(S_LIST);
      break;
    case C_DELETE_ALL:
      clearMsgs();
      listSel = 0;
      go(S_LIST);
      break;
    case C_WIFI:
      cfg.ssid = "";
      cfg.pass = "";
      saveSettings();
      ESP.restart();
      break;
    case C_UNLINK:
      cfg.token = "";
      saveSettings();
      ESP.restart();
      break;
    case C_RESET:
      factoryReset();
      break;
  }
}

static void openMsg(int i) {
  msgIdx = i;
  msgScroll = 0;
  if (!msgs[i].read) {
    msgs[i].read = 1;
    saveMsgs();
  }
  go(S_MSG);
}

static void ackAlarm() {
  soundStop();
  ledBlink(false);
  psAck(alarmId);
  alarmId = "";
  openMsg(0);
}

static void handleButton(Btn b) {
  switch (screen) {
    case S_ALARM:
      ackAlarm();
      break;

    case S_HOME:
      if (b == B_OK) { menuSel = 0; go(S_MENU); }
      else if (b == B_UP || b == B_DOWN) { listSel = 0; go(S_LIST); }
      break;

    case S_MENU:
      if (b == B_UP) menuSel = (menuSel + MN_COUNT - 1) % MN_COUNT;
      else if (b == B_DOWN) menuSel = (menuSel + 1) % MN_COUNT;
      else if (b == B_BACK) { go(S_HOME); break; }
      else if (b == B_OK) {
        switch (menuSel) {
          case MN_MSGS: listSel = 0; go(S_LIST); break;
          case MN_VOL: editing = MN_VOL; editVal = cfg.volume; go(S_VALUE); break;
          case MN_TONE: editing = MN_TONE; editVal = cfg.tone; go(S_VALUE); break;
          case MN_LIGHT: editing = MN_LIGHT; editVal = cfg.light; go(S_VALUE); break;
          case MN_MUTE: cfg.mute = !cfg.mute; saveSettings(); break;
          case MN_INFO: go(S_INFO); break;
          case MN_WIFI: askConfirm(C_WIFI); break;
          case MN_UNLINK: askConfirm(C_UNLINK); break;
          case MN_RESET: askConfirm(C_RESET); break;
        }
      }
      dirty = true;
      break;

    case S_LIST:
      if (b == B_BACK) go(S_HOME);
      else if (!msgCount) break;
      else if (b == B_UP) listSel = (listSel + msgCount - 1) % msgCount;
      else if (b == B_DOWN) listSel = (listSel + 1) % msgCount;
      else if (b == B_OK) openMsg(listSel);
      else if (b == B_OK_LONG) askConfirm(C_DELETE_ALL);
      dirty = true;
      break;

    case S_MSG:
      if (b == B_BACK) { listSel = msgIdx; go(S_LIST); }
      else if (b == B_OK || b == B_OK_LONG) askConfirm(C_DELETE);
      else if (b == B_DOWN) {
        if (msgScroll + 7 < msgLines) msgScroll++;
        else if (msgIdx + 1 < msgCount) openMsg(msgIdx + 1);
      } else if (b == B_UP) {
        if (msgScroll > 0) msgScroll--;
        else if (msgIdx > 0) openMsg(msgIdx - 1);
      }
      dirty = true;
      break;

    case S_VALUE: {
      int maxV = editing == MN_TONE ? toneCount() - 1 : 5;
      int minV = editing == MN_LIGHT ? 1 : 0;
      if (b == B_UP || b == B_DOWN) {
        editVal += b == B_UP ? 1 : -1;
        if (editing == MN_TONE) editVal = (editVal + toneCount()) % toneCount();
        else editVal = constrain(editVal, minV, maxV);
        if (editing == MN_VOL) { uint8_t old = cfg.volume; cfg.volume = editVal; soundStop(); playBeep(); cfg.volume = old; }
        if (editing == MN_TONE) { soundStop(); playAlarm(editVal, false); }
        if (editing == MN_LIGHT) backlight(editVal);
      } else if (b == B_OK) {
        if (editing == MN_VOL) cfg.volume = editVal;
        if (editing == MN_TONE) cfg.tone = editVal;
        if (editing == MN_LIGHT) cfg.light = editVal;
        saveSettings();
        soundStop();
        go(S_MENU);
      } else if (b == B_BACK) {
        soundStop();
        backlight(cfg.light);
        go(S_MENU);
      }
      dirty = true;
      break;
    }

    case S_CONFIRM:
      if (b == B_OK) doConfirm();
      else if (b == B_BACK) go(confirmBack);
      break;

    case S_INFO:
      if (b == B_BACK || b == B_OK) go(S_MENU);
      break;
  }
}

static void draw() {
  switch (screen) {
    case S_HOME: uiHome(); break;
    case S_MENU: drawMenu(); break;
    case S_LIST: uiMsgList(listSel); break;
    case S_MSG:
    case S_ALARM: msgLines = uiMsg(msgIdx, msgScroll); break;
    case S_VALUE: drawValue(); break;
    case S_CONFIRM:
      uiConfirm(confirming == C_DELETE ? "Nachricht löschen?" :
                confirming == C_DELETE_ALL ? "Alle Nachrichten löschen?" :
                confirming == C_WIFI ? "WLAN-Daten löschen und neu einrichten?" :
                confirming == C_UNLINK ? "Pager von deinem PagerSpass Konto trennen?" :
                "Alles auf Werkseinstellungen zurücksetzen?");
      break;
    case S_INFO: drawInfo(); break;
  }
}

static void runLoop() {
  psLoop();
  uint32_t now = millis();

  Btn b = buttonsRead();
  if (b != B_NONE) {
    bool wasDark = !lightOn;
    wake();
    if (!wasDark || screen == S_ALARM) {
      playClick();
      handleButton(b);
    }
  }

  if (screen == S_ALARM && alarmStart && now - alarmStart > ALARM_SECONDS * 1000UL) {
    soundStop();
    alarmStart = 0;
  }

  bool ringing = screen == S_ALARM && alarmStart;
  if (lightOn && !ringing && now - lastInput > LIGHT_SECONDS * 1000UL) {
    lightOn = false;
    backlight(0);
    if (screen != S_HOME && screen != S_ALARM) go(S_HOME);
  }

  if (now - lastTick > 1000) {
    lastTick = now;
    bool w = WiFi.status() == WL_CONNECTED;
    bool s = psConnected();
    int bat = readBattery();
    if (w != st.wifi || s != st.server || abs(bat - st.battery) > 1) dirty |= screen == S_HOME;
    st.wifi = w;
    st.server = s;
    st.rssi = WiFi.RSSI();
    st.battery = bat;
    if (screen == S_HOME) {
      static int lastMin = -1;
      int m = (time(nullptr) / 60) % 60;
      if (m != lastMin) { lastMin = m; dirty = true; }
    }
  }

  if (now - lastStatus > 60000) {
    lastStatus = now;
    psSendStatus(st.battery, st.rssi);
  }

  if (dirty) {
    dirty = false;
    draw();
  }
}

void setup() {
  Serial.begin(115200);
  storeBegin();
  buttonsBegin();
  soundBegin();
  displayBegin();
  uiWelcome();
  backlight(4);
  playBeep();

  uint32_t t = millis();
  while (millis() - t < 2500) {
    soundLoop();
    delay(5);
  }

  if (buttonHeld(PIN_BTN_UP) && buttonHeld(PIN_BTN_DOWN)) {
    uiSetup("Zurücksetzen", "Beide Tasten 5 Sekunden halten um alles zu löschen.", "", "", "");
    t = millis();
    while (buttonHeld(PIN_BTN_UP) && buttonHeld(PIN_BTN_DOWN)) {
      if (millis() - t > 5000) {
        uiSetup("Zurücksetzen", "Werkseinstellungen...", "", "", "");
        factoryReset();
      }
      delay(20);
    }
  }

  if (cfg.ssid.length() && connectSaved()) {
    if (cfg.token.length()) startRun();
    else startAccountSetup(false);
    return;
  }

  mode = M_SETUP_WIFI;
  setupState = cfg.ssid.length() ? SS_WIFI_FAIL : SS_WIFI;
  if (cfg.ssid.length()) setupError = "Gespeichertes WLAN \"" + cfg.ssid + "\" nicht erreichbar";
  portalBegin(true);
  dirty = true;
}

void loop() {
  soundLoop();
  if (mode == M_RUN) runLoop();
  else setupLoop();
  delay(2);
}
