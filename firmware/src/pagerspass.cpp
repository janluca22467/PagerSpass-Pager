#include "pagerspass.h"
#include "store.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

static WebSocketsClient ws;
static PsCallbacks cbs;
static bool online = false;

struct Url {
  bool tls = true;
  String host;
  uint16_t port = 443;
  String base;
};

static Url parseUrl(String s) {
  Url u;
  s.trim();
  if (s.startsWith("http://")) { u.tls = false; u.port = 80; s = s.substring(7); }
  else if (s.startsWith("https://")) s = s.substring(8);
  int slash = s.indexOf('/');
  if (slash >= 0) {
    u.base = s.substring(slash);
    s = s.substring(0, slash);
  }
  if (u.base.endsWith("/")) u.base.remove(u.base.length() - 1);
  int colon = s.indexOf(':');
  if (colon >= 0) {
    u.port = s.substring(colon + 1).toInt();
    s = s.substring(0, colon);
  }
  u.host = s;
  return u;
}

static Login post(String server, const String &path, JsonDocument &req, bool retry = true) {
  Login r;
  r.server = server;
  Url u = parseUrl(server);
  String url = String(u.tls ? "https://" : "http://") + u.host + ":" + u.port + u.base + path;

  WiFiClientSecure secure;
  WiFiClient plain;
  secure.setInsecure();
  HTTPClient http;
  http.setTimeout(10000);
  if (!http.begin(u.tls ? (WiFiClient &)secure : plain, url)) {
    r.err = "Server-Adresse ungültig";
    return r;
  }
  http.addHeader("Content-Type", "application/json");
  const char *keep[] = {"Location"};
  http.collectHeaders(keep, 1);
  String body;
  serializeJson(req, body);
  int code = http.POST(body);
  String resp = http.getString();
  String location = http.header("Location");
  http.end();

  // z.B. http:// -> https://, dann einfach nochmal mit der neuen Adresse
  if ((code == 301 || code == 302 || code == 307 || code == 308) && retry && location.startsWith("http")) {
    int cut = location.indexOf(path);
    String next = cut > 0 ? location.substring(0, cut) : location;
    Serial.println("[http] umgeleitet nach " + next);
    return post(next, path, req, false);
  }

  if (code <= 0) {
    r.err = "Server nicht erreichbar";
    return r;
  }

  JsonDocument doc;
  deserializeJson(doc, resp);
  if (code == 200 && doc["zweiFaktor"] == true) {
    r.twoFactor = true;
    r.request = doc["anfrage"] | "";
    r.target = doc["ziel"] | "";
    return r;
  }
  if (code == 200 && doc["merkmal"].is<const char *>()) {
    r.ok = true;
    r.token = doc["merkmal"].as<String>();
    r.user = doc["benutzername"] | "";
    return r;
  }
  if (doc["fehler"].is<const char *>()) r.err = doc["fehler"].as<String>();
  else if (code == 429) r.err = "Zu viele Versuche, bitte kurz warten";
  else if (code == 503) r.err = "PagerSpass ist gerade in Wartung";
  else r.err = "Fehler vom Server (" + String(code) + ")";
  return r;
}

Login psLogin(const String &server, const String &user, const String &pass) {
  JsonDocument req;
  req["benutzername"] = user;
  req["passwort"] = pass;
  return post(server, "/api/konto/anmelden", req);
}

Login psLogin2fa(const String &server, const String &request, const String &code) {
  JsonDocument req;
  req["anfrage"] = request;
  req["code"] = code;
  return post(server, "/api/konto/anmelden/zweifaktor", req);
}

// "2026-09-25T18:30:00.123+02:00" -> Unix-Zeit
static uint32_t parseIso(const char *s) {
  int y, mo, d, h, mi, sec;
  if (!s || sscanf(s, "%d-%d-%dT%d:%d:%d", &y, &mo, &d, &h, &mi, &sec) != 6) return 0;
  y -= mo <= 2;
  int era = y / 400;
  int yoe = y - era * 400;
  int doy = (153 * (mo + (mo > 2 ? -3 : 9)) + 2) / 5 + d - 1;
  int doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
  long days = era * 146097L + doe - 719468L;
  long t = days * 86400L + h * 3600L + mi * 60L + sec;

  const char *tz = s + 19;
  while (*tz && *tz != 'Z' && *tz != '+' && *tz != '-') tz++;
  if (*tz == '+' || *tz == '-') {
    int oh = 0, om = 0;
    sscanf(tz + 1, "%d:%d", &oh, &om);
    t -= (*tz == '+' ? 1 : -1) * (oh * 3600L + om * 60L);
  }
  return t;
}

// gleiche Zeilen wie der Melder im Spiel (web/src/composables/melderanzeige.ts)
static Alarm buildAlarm(JsonObjectConst a) {
  Alarm al;
  al.id = a["incidentId"] | "";
  al.head = a["schleife"] | "";
  al.prio = a["prioritaet"] | 0;
  al.ts = parseIso(a["zeit"]);

  auto line = [&](const String &s) {
    if (!s.length()) return;
    if (al.text.length()) al.text += "\n";
    al.text += s;
  };
  line(String(a["stichwort"] | "") + " " + (a["stichwortText"] | ""));
  line(a["adresse"] | "");
  if (a["ortsteil"].is<const char *>()) line(String("OT ") + a["ortsteil"].as<const char *>());
  line(a["meldebild"] | "");
  if (a["funkgruppe"].is<const char *>()) line(String("GRUPPE ") + a["funkgruppe"].as<const char *>());
  String units;
  for (JsonVariantConst e : a["einheiten"].as<JsonArrayConst>()) {
    if (units.length()) units += ", ";
    units += e.as<const char *>();
  }
  if (units.length()) line("EINH: " + units);
  if (a["zusatztext"].is<const char *>()) line(String("LST: ") + a["zusatztext"].as<const char *>());
  return al;
}

static void handleMessage(const char *data, size_t len) {
  JsonDocument doc;
  if (deserializeJson(doc, data, len)) return;
  String t = doc["t"] | "";

  if (t == "alarm") {
    if (cbs.onAlarm) cbs.onAlarm(buildAlarm(doc["alarm"]));
  } else if (t == "round") {
    String st = doc["state"] | "";
    RoundState r = st == "active" ? R_ACTIVE : st == "lobby" ? R_LOBBY : R_WAITING;
    if (cbs.onRound) cbs.onRound(r, doc["name"] | "", doc["funkrufname"] | "");
  } else if (t == "unlinked") {
    if (cbs.onUnlinked) cbs.onUnlinked();
  }
}

static void onEvent(WStype_t type, uint8_t *payload, size_t len) {
  switch (type) {
    case WStype_CONNECTED:
      online = true;
      Serial.println("[ps] verbunden");
      break;
    case WStype_DISCONNECTED:
      if (online) Serial.println("[ps] getrennt");
      online = false;
      break;
    case WStype_TEXT:
      handleMessage((const char *)payload, len);
      break;
    default:
      break;
  }
}

void psBegin(const PsCallbacks &cb) {
  cbs = cb;
  Url u = parseUrl(cfg.server);
  String path = u.base + "/hub/pager";
  if (u.tls) ws.beginSSL(u.host.c_str(), u.port, path.c_str());
  else ws.begin(u.host, u.port, path);
  String auth = "Authorization: Bearer " + cfg.token;
  ws.setExtraHeaders(auth.c_str());
  ws.onEvent(onEvent);
  ws.setReconnectInterval(5000);
  ws.enableHeartbeat(20000, 5000, 2);
}

void psLoop() {
  ws.loop();
}

bool psConnected() {
  return online;
}

void psAck() {
  if (online) ws.sendTXT("{\"t\":\"ack\"}");
}
