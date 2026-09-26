#pragma once
#include <Arduino.h>

void soundBegin();
void soundLoop();
void playAlarm(uint8_t tone, uint8_t prio, bool loop);
void playBeep();
void playClick();
void soundStop();
bool soundPlaying();
void ledBlink(bool on);
const char *toneName(uint8_t t);
uint8_t toneCount();
