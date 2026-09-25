#include "sound.h"
#include "config.h"
#include "store.h"

struct Note { uint16_t f; uint16_t ms; };

static const Note tone0[] = {{2100, 120}, {0, 60}, {2100, 120}, {0, 60}, {2100, 120}, {0, 500}};
static const Note tone1[] = {{1600, 250}, {2400, 250}, {1600, 250}, {2400, 250}, {0, 400}};
static const Note tone2[] = {{1000, 80}, {1300, 80}, {1600, 80}, {1900, 80}, {2200, 80}, {2500, 80}, {0, 300}};
static const Note tone3[] = {{2800, 700}, {0, 300}, {2800, 700}, {0, 600}};
static const Note beep[] = {{2400, 60}, {0, 10}};
static const Note click[] = {{3000, 8}, {0, 1}};

struct Melody { const char *name; const Note *notes; uint8_t len; };

static const Melody melodies[] = {
  {"Standard", tone0, 6},
  {"Zweiton", tone1, 5},
  {"Sirene", tone2, 7},
  {"Lang", tone3, 4},
};

static const Melody mBeep = {"", beep, 2};
static const Melody mClick = {"", click, 2};

static const Melody *cur = nullptr;
static uint8_t idx;
static uint32_t noteEnd;
static bool looping;
static bool ledMode;
static uint32_t ledTimer;
static bool ledState;

static const uint16_t volDuty[] = {0, 8, 30, 90, 220, 512};

static void out(uint16_t f) {
  if (f == 0 || cfg.mute) {
    ledcWrite(0, 0);
    return;
  }
  ledcWriteTone(0, f);
  ledcWrite(0, volDuty[constrain(cfg.volume, 0, 5)]);
}

void soundBegin() {
  ledcSetup(0, 2000, 10);
  ledcAttachPin(PIN_BUZZER, 0);
  ledcWrite(0, 0);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);
}

static void start(const Melody *m, bool loop) {
  cur = m;
  idx = 0;
  looping = loop;
  out(cur->notes[0].f);
  noteEnd = millis() + cur->notes[0].ms;
}

void playAlarm(uint8_t t, bool loop) { start(&melodies[t % toneCount()], loop); }
void playBeep() { if (!cur) start(&mBeep, false); }
void playClick() { if (!cur) start(&mClick, false); }

void soundStop() {
  cur = nullptr;
  ledcWrite(0, 0);
}

bool soundPlaying() { return cur != nullptr; }

void ledBlink(bool on) {
  ledMode = on;
  if (!on) digitalWrite(PIN_LED, LOW);
}

void soundLoop() {
  uint32_t now = millis();
  if (cur && (int32_t)(now - noteEnd) >= 0) {
    if (++idx >= cur->len) {
      if (looping) idx = 0;
      else soundStop();
    }
    if (cur) {
      out(cur->notes[idx].f);
      noteEnd = now + cur->notes[idx].ms;
    }
  }
  if (ledMode && now - ledTimer > (ledState ? 80 : 400)) {
    ledState = !ledState;
    ledTimer = now;
    digitalWrite(PIN_LED, ledState);
  }
}

const char *toneName(uint8_t t) { return melodies[t % toneCount()].name; }
uint8_t toneCount() { return sizeof(melodies) / sizeof(melodies[0]); }
