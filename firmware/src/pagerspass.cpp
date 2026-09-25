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

bool psLink(const String &server, const String &user, const String &pass, String &token, String &err) {
  Url u = parseUrl(server);
  String url = String(u.tls ? "https://" : "http://") + u.host + ":" + u.port + u.base + "/api/pager/link";

  WiFiClientSecure secure;
  WiFiClient plain;
  secure.setInsecure();
  HTTPClient http;
  http.setTimeout(8000);
  if (!http.begin(u.tls ? (WiFiClient &)secure : plain, url)) {
    err = "Server-Adresse ungültig";
    return false;
  }
  http.addHeader("Content-Type", "application/json");

  JsonDocument req;
  req["user"] = user;
  req["password"] = pass;
  req["device"] = "Pager-" + deviceId();
  req["fw"] = FW_VERSION;
  String body;
  serializeJson(req, body);

  int code = http.POST(body);
  String resp = http.getString();
  http.end();

  if (code <= 0) {
    err = "Server nicht erreichbar";
    return false;
  }

  JsonDocument doc;
  deserializeJson(doc, resp);
  if (code != 200 || !doc["token"].is<const char *>()) {
    if (doc["error"].is<const char *>()) err = doc["error"].as<String>();
    else if (code == 401 || code == 403) err = "Benutzername oder Passwort falsch";
    else err = "Fehler vom Server (" + String(code) + ")";
    return false;
  }
  token = doc["token"].as<String>();
  return true;
}

static void handleMessage(const char *data, size_t len) {
  JsonDocument doc;
  if (deserializeJson(doc, data, len)) return;
  String t = doc["t"] | "";

  if (t == "alarm") {
    Alarm a;
    a.id = doc["id"] | "";
    a.text = doc["text"] | "";
    a.adr = doc["adr"] | 1;
    a.prio = doc["prio"] | 0;
    a.ts = doc["ts"] | 0;
    if (cbs.onAlarm) cbs.onAlarm(a);
  } else if (t == "round") {
    String st = doc["state"] | "";
    RoundState r = st == "active" ? R_ACTIVE : R_WAITING;
    if (cbs.onRound) cbs.onRound(r, doc["name"] | "");
  } else if (t == "unlinked") {
    if (cbs.onUnlinked) cbs.onUnlinked();
  } else if (t == "ping") {
    ws.sendTXT("{\"t\":\"pong\"}");
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
  String path = u.base + "/api/pager/ws?token=" + cfg.token + "&device=Pager-" + deviceId();
  if (u.tls) ws.beginSSL(u.host.c_str(), u.port, path.c_str());
  else ws.begin(u.host, u.port, path);
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

void psAck(const String &id) {
  if (!online || !id.length()) return;
  JsonDocument doc;
  doc["t"] = "ack";
  doc["id"] = id;
  String s;
  serializeJson(doc, s);
  ws.sendTXT(s);
}

void psSendStatus(int battery, int rssi) {
  if (!online) return;
  JsonDocument doc;
  doc["t"] = "status";
  doc["bat"] = battery;
  doc["rssi"] = rssi;
  doc["fw"] = FW_VERSION;
  String s;
  serializeJson(doc, s);
  ws.sendTXT(s);
}
