#include "ui.h"
#include "display.h"
#include "store.h"
#include "config.h"
#include "portal.h"
#include <WiFi.h>
#include <time.h>

Status st;

#define F_SMALL u8g2_font_helvR10_tf
#define F_MID u8g2_font_helvR14_tf
#define F_MIDB u8g2_font_helvB14_tf
#define F_BIG u8g2_font_helvR18_tf
#define F_BIGB u8g2_font_helvB24_tf
#define F_CLOCK u8g2_font_logisoso54_tn

static const char *WEEKDAYS[] = {"Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag"};

String fmtTime(uint32_t ts, bool withYear) {
  if (ts < 1600000000) return withYear ? "--.--.---- --:--" : "--.-- --:--";
  time_t t = ts;
  struct tm tm;
  localtime_r(&t, &tm);
  char buf[24];
  if (withYear) strftime(buf, sizeof(buf), "%d.%m.%Y %H:%M", &tm);
  else strftime(buf, sizeof(buf), "%d.%m. %H:%M", &tm);
  return buf;
}

static void drawBattery(int x, int y, int pct) {
  cv.drawRect(x, y, 22, 12, 0);
  cv.fillRect(x + 22, y + 3, 2, 6, 0);
  if (pct >= 0) cv.fillRect(x + 2, y + 2, 18 * pct / 100, 8, 0);
}

static void drawWifi(int x, int y) {
  int bars = 0;
  if (st.wifi) bars = st.rssi > -60 ? 4 : st.rssi > -70 ? 3 : st.rssi > -80 ? 2 : 1;
  for (int i = 0; i < 4; i++) {
    int h = 3 + i * 3;
    if (i < bars) cv.fillRect(x + i * 5, y + 12 - h, 3, h, 0);
    else cv.drawFastHLine(x + i * 5, y + 11, 3, 0);
  }
}

void uiWelcome() {
  clearScreen();
  textCenter(80, "Willkommen zu", F_MID);
  textCenter(125, "PagerSpass", F_BIGB);
  textCenter(155, "Pager", F_BIG);
  cv.drawFastHLine(90, 175, 140, 1);
  textCenter(225, String("v" FW_VERSION "  ·  ") + apName(), F_SMALL);
  displayPush();
}

void uiSetup(const String &title, const String &l1, const String &big, const String &l2, const String &small) {
  clearScreen();
  headerBar(title, "");
  String lines[4];
  int n = wrapText(l1, F_MID, SCREEN_W - 20, lines, 4);
  int y = 60;
  for (int i = 0; i < min(n, 4); i++, y += 22) textCenter(y, lines[i], F_MID);
  if (big.length()) {
    y += 16;
    textCenter(y, big, F_BIGB);
    y += 14;
  }
  n = wrapText(l2, F_MID, SCREEN_W - 20, lines, 4);
  y += 16;
  for (int i = 0; i < min(n, 4); i++, y += 22) textCenter(y, lines[i], F_MID);
  if (small.length()) textCenter(230, small, F_SMALL);
  displayPush();
}

void uiHome() {
  clearScreen();
  headerBar("PagerSpass", "");
  drawWifi(SCREEN_W - 62, 7);
  drawBattery(SCREEN_W - 32, 7, st.battery);
  if (st.battery >= 0) textRight(SCREEN_W - 68, 19, String(st.battery) + "%", F_SMALL, true);

  time_t now = time(nullptr);
  if (now > 1600000000) {
    struct tm tm;
    localtime_r(&now, &tm);
    char buf[16];
    strftime(buf, sizeof(buf), "%H:%M", &tm);
    textCenter(110, buf, F_CLOCK);
    strftime(buf, sizeof(buf), "%d.%m.%Y", &tm);
    textCenter(142, String(WEEKDAYS[tm.tm_wday]) + ", " + buf, F_MID);
  } else {
    textCenter(110, "--:--", F_CLOCK);
  }

  cv.drawFastHLine(20, 160, SCREEN_W - 40, 1);

  String top, bottom;
  if (!st.wifi) {
    top = "Kein WLAN";
    bottom = "Suche Netzwerk...";
  } else if (!st.server) {
    top = "Keine Verbindung";
    bottom = "Verbinde mit PagerSpass...";
  } else if (st.round == R_ACTIVE) {
    top = st.callsign.length() ? st.callsign : String("Runde: ") + st.roundName;
    bottom = "BETRIEBSBEREIT";
  } else if (st.round == R_LOBBY) {
    top = String("Lobby: ") + st.roundName;
    bottom = "Warte auf Start...";
  } else {
    top = "Warte auf Runde...";
    bottom = cfg.user.length() ? String("Angemeldet als ") + cfg.user : String("Bereit");
  }
  textCenter(190, top, F_MIDB);
  int unread = unreadCount();
  if (unread) bottom = String(unread) + (unread == 1 ? " neue Nachricht" : " neue Nachrichten");
  textCenter(220, bottom, F_MID);
  displayPush();
}

void uiMenu(const String &title, const String *items, const String *values, int n, int sel) {
  clearScreen();
  headerBar(title, String(sel + 1) + "/" + n);
  const int rows = 7, h = 30;
  int first = constrain(sel - rows / 2, 0, max(0, n - rows));
  for (int i = first; i < min(n, first + rows); i++) {
    int y = 28 + (i - first) * h;
    bool s = i == sel;
    if (s) cv.fillRect(0, y, SCREEN_W, h - 2, 1);
    textAt(10, y + 21, items[i], F_MID, s);
    if (values && values[i].length()) textRight(SCREEN_W - 10, y + 21, values[i], F_MID, s);
  }
  displayPush();
}

void uiMsgList(int sel) {
  clearScreen();
  headerBar("Nachrichten", msgCount ? String(sel + 1) + "/" + msgCount : "");
  if (!msgCount) {
    textCenter(130, "Keine Nachrichten", F_MID);
    displayPush();
    return;
  }
  const int rows = 5, h = 42;
  int first = constrain(sel - rows / 2, 0, max(0, msgCount - rows));
  for (int i = first; i < min(msgCount, first + rows); i++) {
    int y = 28 + (i - first) * h;
    bool s = i == sel;
    if (s) cv.fillRect(0, y, SCREEN_W, h - 2, 1);
    if (!msgs[i].read) cv.fillCircle(10, y + 12, 4, s ? 0 : 1);
    textAt(20, y + 16, fmtTime(msgs[i].ts, false) + "   " + msgs[i].head, F_SMALL, s);
    String line[1];
    String t = msgs[i].text;
    t.replace("\n", " ");
    wrapText(t, F_MID, SCREEN_W - 30, line, 1);
    textAt(20, y + 35, line[0], F_MID, s);
  }
  displayPush();
}

int uiMsg(int i, int scroll) {
  clearScreen();
  const Msg &m = msgs[i];
  String right = m.head;
  while (right.length() > 1 && textWidth(right, u8g2_font_helvR14_tf) > 130) right.remove(right.length() - 1);
  if (m.prio >= 3) right = "! " + right;
  headerBar(fmtTime(m.ts, true), right);

  const int maxLines = 40, visible = 7;
  static String lines[maxLines];
  int n = min(wrapText(m.text, F_BIG, SCREEN_W - 20, lines, maxLines), maxLines);
  scroll = constrain(scroll, 0, max(0, n - visible));
  for (int l = 0; l < visible && scroll + l < n; l++) textAt(6, 54 + l * 28, lines[scroll + l], F_BIG);

  if (n > visible) {
    int barH = 206 * visible / n;
    int barY = 30 + (206 - barH) * scroll / (n - visible);
    cv.drawFastVLine(SCREEN_W - 3, 30, 206, 1);
    cv.fillRect(SCREEN_W - 5, barY, 5, barH, 1);
  }
  displayPush();
  return n;
}

void uiValue(const String &title, const String &value, const String &hint) {
  clearScreen();
  headerBar(title, "");
  textCenter(100, value, F_BIGB);
  textCenter(150, "Hoch / Runter zum Ändern", F_MID);
  textCenter(215, hint.length() ? hint : "OK speichern · Zurück abbrechen", F_SMALL);
  displayPush();
}

void uiConfirm(const String &question) {
  clearScreen();
  headerBar("Bestätigen", "");
  String lines[4];
  int n = wrapText(question, F_BIG, SCREEN_W - 20, lines, 4);
  for (int i = 0; i < min(n, 4); i++) textCenter(80 + i * 28, lines[i], F_BIG);
  textCenter(215, "OK = Ja    Zurück = Nein", F_MID);
  displayPush();
}

void uiInfo(const String *lines, int n) {
  clearScreen();
  headerBar("Info", "");
  for (int i = 0; i < n && i < 10; i++) textAt(8, 46 + i * 21, lines[i], F_SMALL);
  displayPush();
}
