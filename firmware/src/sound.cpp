#include "sound.h"
#include "config.h"
#include "store.h"
#include "toene_spiel.h"

static const Seg beep[] PROGMEM = {{2400, 2400, 60}, {0, 0, 10}};
static const Seg click[] PROGMEM = {{3000, 3000, 8}, {0, 0, 1}};

static const Seg *cur = nullptr;
static uint8_t curLen;
static uint8_t idx;
static uint32_t segStart;
static bool looping;
static bool ledMode;
static uint32_t ledTimer;
static bool ledState;

static const uint16_t volDuty[] = {0, 8, 30, 90, 220, 512};
static Seg seg;

static void load(uint8_t i) {
  memcpy_P(&seg, &cur[i], sizeof(Seg));
}

static void silent() {
#ifdef ESP8266
  analogWrite(PIN_BUZZER, 0);
#else
  ledcWrite(0, 0);
#endif
}

static void out(uint16_t f) {
  if (f == 0 || cfg.mute || cfg.volume == 0) {
    silent();
    return;
  }
  uint16_t duty = volDuty[constrain(cfg.volume, 0, 5)];
#ifdef ESP8266
  // die PWM-Frequenz gilt beim ESP8266 fuer alle Pins, das Display-Licht aendert sich dadurch aber nicht
  analogWriteFreq(f);
  analogWrite(PIN_BUZZER, duty);
#else
  ledcWriteTone(0, f);
  ledcWrite(0, duty);
#endif
}

void soundBegin() {
#ifdef ESP8266
  analogWriteRange(1023);
  pinMode(PIN_BUZZER, OUTPUT);
#else
  ledcSetup(0, 2000, 10);
  ledcAttachPin(PIN_BUZZER, 0);
#endif
  silent();
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, !LED_ON);
}

static void start(const Seg *s, uint8_t len, bool loop) {
  cur = s;
  curLen = len;
  idx = 0;
  looping = loop;
  segStart = millis();
  load(0);
  out(seg.f0);
}

void playAlarm(uint8_t t, uint8_t prio, bool loop) {
  const SpielTon &ton = SPIEL_TOENE[t % SPIEL_TOENE_ANZAHL];
  uint8_t p = constrain(prio, 1, 3) - 1;
  start(ton.seg[p], ton.len[p], loop);
}

void playBeep() { if (!cur) start(beep, 2, false); }
void playClick() { if (!cur) start(click, 2, false); }

void soundStop() {
  cur = nullptr;
  silent();
}

bool soundPlaying() { return cur != nullptr; }

void ledBlink(bool on) {
  ledMode = on;
  if (!on) digitalWrite(PIN_LED, !LED_ON);
}

void soundLoop() {
  uint32_t now = millis();
  if (cur) {
    const Seg &s = seg;
    uint32_t el = now - segStart;
    if (el >= s.ms) {
      segStart += s.ms;
      if (++idx >= curLen) {
        if (looping) idx = 0;
        else soundStop();
      }
      if (cur) {
        load(idx);
        out(seg.f0);
      }
    } else if (s.f0 != s.f1 && s.f0) {
      static uint32_t lastGlide;
      if (now - lastGlide >= 10) {
        lastGlide = now;
        out(s.f0 + (int32_t)(s.f1 - s.f0) * (int32_t)el / s.ms);
      }
    }
  }
  if (ledMode && now - ledTimer > (ledState ? 80 : 400)) {
    ledState = !ledState;
    ledTimer = now;
    digitalWrite(PIN_LED, ledState ? LED_ON : !LED_ON);
  }
}

const char *toneName(uint8_t t) { return SPIEL_TOENE[t % SPIEL_TOENE_ANZAHL].name; }
uint8_t toneCount() { return SPIEL_TOENE_ANZAHL; }
