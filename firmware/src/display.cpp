#include "display.h"
#include "config.h"
#include <SPI.h>

// im Simulator (Wokwi) gibt es kein ST7789, der ILI9341 hat aber auch 320x240
#ifdef SIM
#include <Adafruit_ILI9341.h>
static Adafruit_ILI9341 tft(&SPI, PIN_TFT_DC, PIN_TFT_CS, PIN_TFT_RST);
#else
#include <Adafruit_ST7789.h>
static Adafruit_ST7789 tft(&SPI, PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST);
#endif
GFXcanvas1 cv(SCREEN_W, SCREEN_H);
U8G2_FOR_ADAFRUIT_GFX u8;

// LCD-Optik wie beim echten Melder: grau-blauer Hintergrund, dunkelblaue Schrift
static const uint16_t COL_BG = ((178 & 0xF8) << 8) | ((192 & 0xFC) << 3) | (204 >> 3);
static const uint16_t COL_FG = ((34 & 0xF8) << 8) | ((44 & 0xFC) << 3) | (110 >> 3);

void displayBegin() {
  ledcSetup(1, 5000, 8);
  ledcAttachPin(PIN_TFT_BL, 1);
  backlight(0);

  SPI.begin(PIN_SCK, -1, PIN_MOSI, PIN_TFT_CS);
#ifdef SIM
  tft.begin(40000000);
#else
  tft.init(240, 320);
  tft.setSPISpeed(40000000);
#endif
  tft.setRotation(1);
  tft.fillScreen(COL_BG);

  u8.begin(cv);
  u8.setFontMode(1);
  u8.setFontDirection(0);
}

void displayPush() {
  static uint16_t line[SCREEN_W];
  const uint8_t *buf = cv.getBuffer();
  const int stride = (SCREEN_W + 7) / 8;
  tft.startWrite();
  tft.setAddrWindow(0, 0, SCREEN_W, SCREEN_H);
  for (int y = 0; y < SCREEN_H; y++) {
    const uint8_t *row = buf + y * stride;
    for (int x = 0; x < SCREEN_W; x++)
      line[x] = (row[x >> 3] & (0x80 >> (x & 7))) ? COL_FG : COL_BG;
    tft.writePixels(line, SCREEN_W);
  }
  tft.endWrite();
}

void backlight(uint8_t level) {
  static const uint8_t duty[] = {0, 10, 40, 90, 160, 255};
  ledcWrite(1, duty[min<uint8_t>(level, 5)]);
}

void clearScreen() {
  cv.fillScreen(0);
}

void textAt(int x, int y, const String &s, const uint8_t *font, bool inv) {
  u8.setFont(font);
  u8.setForegroundColor(inv ? 0 : 1);
  u8.drawUTF8(x, y, s.c_str());
}

int textWidth(const String &s, const uint8_t *font) {
  u8.setFont(font);
  return u8.getUTF8Width(s.c_str());
}

void textCenter(int y, const String &s, const uint8_t *font) {
  textAt((SCREEN_W - textWidth(s, font)) / 2, y, s, font);
}

void textRight(int x, int y, const String &s, const uint8_t *font, bool inv) {
  textAt(x - textWidth(s, font), y, s, font, inv);
}

void headerBar(const String &left, const String &right) {
  cv.fillRect(0, 0, SCREEN_W, 26, 1);
  textAt(6, 20, left, u8g2_font_helvR14_tf, true);
  textRight(SCREEN_W - 6, 20, right, u8g2_font_helvR14_tf, true);
}

static int utf8Len(uint8_t c) {
  if (c < 0x80) return 1;
  if ((c & 0xE0) == 0xC0) return 2;
  if ((c & 0xF0) == 0xE0) return 3;
  return 4;
}

int wrapText(const String &s, const uint8_t *font, int w, String *lines, int maxLines) {
  u8.setFont(font);
  int n = 0;
  String cur;
  String word;
  auto fits = [&](const String &t) { return u8.getUTF8Width(t.c_str()) <= w; };
  auto push = [&](const String &t) {
    if (n < maxLines) lines[n] = t;
    n++;
  };
  auto flushWord = [&]() {
    if (!word.length()) return;
    String test = cur.length() ? cur + " " + word : word;
    if (fits(test)) {
      cur = test;
    } else {
      if (cur.length()) push(cur);
      cur = "";
      while (!fits(word)) {
        int i = 0, last = 0;
        while (i < (int)word.length()) {
          int l = utf8Len(word[i]);
          if (!fits(word.substring(0, i + l))) break;
          i += l;
          last = i;
        }
        if (last == 0) last = utf8Len(word[0]);
        push(word.substring(0, last));
        word = word.substring(last);
      }
      cur = word;
    }
    word = "";
  };

  for (unsigned i = 0; i < s.length(); i++) {
    char c = s[i];
    if (c == ' ') {
      flushWord();
    } else if (c == '\n') {
      flushWord();
      push(cur);
      cur = "";
    } else {
      word += c;
    }
  }
  flushWord();
  if (cur.length()) push(cur);
  return n;
}
