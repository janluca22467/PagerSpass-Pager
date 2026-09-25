#include "buttons.h"
#include "config.h"

struct Key {
  uint8_t pin;
  Btn shortEv, longEv;
  bool repeat;
  bool down;
  bool longSent;
  uint32_t since;
  uint32_t next;
};

static Key keys[] = {
  {PIN_BTN_OK, B_OK, B_OK_LONG, false},
  {PIN_BTN_UP, B_UP, B_NONE, true},
  {PIN_BTN_DOWN, B_DOWN, B_NONE, true},
  {PIN_BTN_BACK, B_BACK, B_BACK_LONG, false},
};

void buttonsBegin() {
  for (auto &k : keys) pinMode(k.pin, INPUT_PULLUP);
}

bool buttonHeld(int pin) {
  return digitalRead(pin) == LOW;
}

Btn buttonsRead() {
  uint32_t now = millis();
  for (auto &k : keys) {
    bool pressed = digitalRead(k.pin) == LOW;

    if (pressed && !k.down) {
      if (now - k.since < 40) continue;
      k.down = true;
      k.longSent = false;
      k.since = now;
      if (k.repeat) {
        k.next = now + 500;
        return k.shortEv;
      }
    } else if (pressed && k.down) {
      if (k.repeat && now >= k.next) {
        k.next = now + 120;
        return k.shortEv;
      }
      if (!k.repeat && !k.longSent && now - k.since > 1200) {
        k.longSent = true;
        return k.longEv;
      }
    } else if (!pressed && k.down) {
      k.down = false;
      k.since = now;
      if (!k.repeat && !k.longSent) return k.shortEv;
    }
  }
  return B_NONE;
}
