#pragma once
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <U8g2_for_Adafruit_GFX.h>

extern GFXcanvas1 cv;
extern U8G2_FOR_ADAFRUIT_GFX u8;

void displayBegin();
void displayPush();
void backlight(uint8_t level);

void clearScreen();
void headerBar(const String &left, const String &right);
void textAt(int x, int y, const String &s, const uint8_t *font, bool inv = false);
void textCenter(int y, const String &s, const uint8_t *font);
void textRight(int x, int y, const String &s, const uint8_t *font, bool inv = false);
int textWidth(const String &s, const uint8_t *font);
int wrapText(const String &s, const uint8_t *font, int w, String *lines, int maxLines);
