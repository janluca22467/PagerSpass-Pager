#pragma once
#include <Arduino.h>

enum Btn { B_NONE, B_OK, B_UP, B_DOWN, B_BACK, B_OK_LONG, B_BACK_LONG };

void buttonsBegin();
Btn buttonsRead();
bool buttonHeld(int pin);
