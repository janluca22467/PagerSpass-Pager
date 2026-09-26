#include "ui.h"
#include "display.h"
#include "store.h"
#include "plattform.h"
#include "config.h"
#include "portal.h"
#include <time.h>

Status st;

// Das 1.9" Display ist nur 43x23 mm gross, darum alles fett und nicht zu klein
#define F_SMALL u8g2_font_helvB12_tf
#define F_MID u8g2_font_helvB14_tf
#define F_MENU u8g2_font_helvB18_tf
#define F_MSG u8g2_font_helvB18_tf
#define F_BIGB u8g2_font_helvB24_tf
#define F_TINY u8g2_font_helvR10_tf
#define F_CLOCK u8g2_font_logisoso50_tn

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
  textCenter(42, "Willkommen zu", F_MID);
  textCenter(82, "PagerSpass", F_BIGB);
  textCenter(110, "Pager", F_MENU);
  cv.drawFastHLine(90, 124, 140, 1);
  textCenter(160, String("v" FW_VERSION "  ·  ") + apName(), F_TINY);
  displayPush();
}

void uiSetup(const String &title, const String &l1, const String &big, const String &l2, const String &small) {
  clearScreen();
  headerBar(title, "");
  String lines[3];
  int n = wrapText(l1, F_MID, SCREEN_W - 16, lines, 3);
  int y = 48;
  for (int i = 0; i < min(n, 2); i++, y += 20) textCenter(y, lines[i], F_MID);
  if (big.length()) {
    y += 12;
    textCenter(y, big, F_BIGB);
    y += 6;
  }
  n = wrapText(l2, F_MID, SCREEN_W - 16, lines, 3);
  y += 18;
  for (int i = 0; i < min(n, 2); i++, y += 20) textCenter(y, lines[i], F_MID);
  if (small.length()) textCenter(165, small, F_TINY);
  displayPush();
}

void uiHome() {
  clearScreen();
  headerBar("PagerSpass", "");
  drawWifi(SCREEN_W - 62, 6);
  drawBattery(SCREEN_W - 32, 6, st.battery);
  if (st.battery >= 0) textRight(SCREEN_W - 68, 18, String(st.battery) + "%", F_TINY, true);

  time_t now = time(nullptr);
  if (now > 1600000000) {
    struct tm tm;
    localtime_r(&now, &tm);
    char buf[16];
    strftime(buf, sizeof(buf), "%H:%M", &tm);
    textCenter(84, buf, F_CLOCK);
    strftime(buf, sizeof(buf), "%d.%m.%Y", &tm);
    textCenter(106, String(WEEKDAYS[tm.tm_wday]) + ", " + buf, F_MID);
  } else {
    textCenter(84, "--:--", F_CLOCK);
  }

  cv.drawFastHLine(10, 114, SCREEN_W - 20, 1);

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
    bottom = cfg.user.length() ? cfg.user : String("Bereit");
  }
  int unread = unreadCount();
  if (unread) bottom = String(unread) + (unread == 1 ? " neue Nachricht" : " neue Nachrichten");

  String l[1];
  wrapText(top, F_MENU, SCREEN_W - 10, l, 1);
  textCenter(139, l[0], F_MENU);
  textCenter(163, bottom, F_MID);
  displayPush();
}

void uiMenu(const String &title, const String *items, const String *values, int n, int sel) {
  clearScreen();
  headerBar(title, String(sel + 1) + "/" + n);
  const int rows = 5, h = 29;
  int first = constrain(sel - rows / 2, 0, max(0, n - rows));
  for (int i = first; i < min(n, first + rows); i++) {
    int y = HEADER_H + 1 + (i - first) * h;
    bool s = i == sel;
    if (s) cv.fillRect(0, y, SCREEN_W, h, 1);
    textAt(8, y + 22, items[i], F_MENU, s);
    if (values && values[i].length()) textRight(SCREEN_W - 8, y + 22, values[i], F_MENU, s);
  }
  displayPush();
}

void uiMsgList(int sel) {
  clearScreen();
  headerBar("Nachrichten", msgCount ? String(sel + 1) + "/" + msgCount : "");
  if (!msgCount) {
    textCenter(100, "Keine Nachrichten", F_MENU);
    displayPush();
    return;
  }
  const int rows = 3, h = 48;
  int first = constrain(sel - rows / 2, 0, max(0, msgCount - rows));
  for (int i = first; i < min(msgCount, first + rows); i++) {
    int y = HEADER_H + 1 + (i - first) * h;
    bool s = i == sel;
    if (s) cv.fillRect(0, y, SCREEN_W, h - 1, 1);
    int x = 8;
    if (!msgs[i].read) {
      cv.fillCircle(12, y + 11, 5, s ? 0 : 1);
      x = 24;
    }
    textAt(x, y + 16, fmtTime(msgs[i].ts, false) + "   " + msgs[i].head, F_SMALL, s);
    String line[1];
    String t = msgs[i].text;
    t.replace("\n", " ");
    wrapText(t, F_MENU, SCREEN_W - 16, line, 1);
    textAt(8, y + 40, line[0], F_MENU, s);
  }
  displayPush();
}

int uiMsg(int i, int scroll) {
  clearScreen();
  const Msg &m = msgs[i];
  String right = m.head;
  while (right.length() > 1 && textWidth(right, F_MID) > 120) right.remove(right.length() - 1);
  if (m.prio >= 3) right = "! " + right;
  headerBar(fmtTime(m.ts, true), right);

  const int maxLines = 30, visible = 5;
  static String lines[maxLines];
  int n = min(wrapText(m.text, F_MSG, SCREEN_W - 14, lines, maxLines), maxLines);
  scroll = constrain(scroll, 0, max(0, n - visible));
  for (int l = 0; l < visible && scroll + l < n; l++) textAt(4, 47 + l * 28, lines[scroll + l], F_MSG);

  if (n > visible) {
    const int top = HEADER_H + 2, len = SCREEN_H - top;
    int barH = len * visible / n;
    int barY = top + (len - barH) * scroll / (n - visible);
    cv.drawFastVLine(SCREEN_W - 3, top, len, 1);
    cv.fillRect(SCREEN_W - 6, barY, 6, barH, 1);
  }
  displayPush();
  return n;
}

void uiValue(const String &title, const String &value, const String &hint) {
  clearScreen();
  headerBar(title, "");
  textCenter(80, value, F_BIGB);
  textCenter(116, "Hoch / Runter", F_MID);
  textCenter(160, hint.length() ? hint : "OK = Speichern   Zurück = Abbruch", F_SMALL);
  displayPush();
}

void uiConfirm(const String &question) {
  clearScreen();
  headerBar("Bestätigen", "");
  String lines[3];
  int n = wrapText(question, F_MENU, SCREEN_W - 16, lines, 3);
  for (int i = 0; i < min(n, 3); i++) textCenter(56 + i * 27, lines[i], F_MENU);
  textCenter(162, "OK = Ja    Zurück = Nein", F_MID);
  displayPush();
}

int uiInfo(const String *lines, int n, int scroll) {
  clearScreen();
  const int visible = 7;
  scroll = constrain(scroll, 0, max(0, n - visible));
  headerBar("Info", String(scroll + 1) + "-" + min(n, scroll + visible) + "/" + n);
  for (int i = 0; i < visible && scroll + i < n; i++) textAt(6, 43 + i * 20, lines[scroll + i], F_SMALL);
  displayPush();
  return scroll;
}
