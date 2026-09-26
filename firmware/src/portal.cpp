#include "portal.h"
#include "store.h"
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <ESPmDNS.h>
#include <ArduinoJson.h>

SetupState setupState = SS_WIFI;
String setupError;
String setupHint;

static WebServer server(80);
static DNSServer dns;
static bool running = false;
static bool apOn = false;

static bool wifiReq = false, accReq = false, codeReq = false;
static String reqSsid, reqPass, reqServer, reqUser, reqUserPass, reqCode;

static const char PAGE_HEAD[] PROGMEM = R"(<!DOCTYPE html><html lang="de"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1"><title>PagerSpass Pager</title>
<style>
body{font-family:sans-serif;background:#1d2a5a;color:#222;margin:0;padding:16px}
.box{background:#e8edf1;max-width:380px;margin:20px auto;padding:20px;border-radius:10px}
h1{font-size:20px;margin:0 0 4px;color:#1d2a5a}p{font-size:14px;color:#555}
label{display:block;margin-top:12px;font-size:13px;font-weight:bold}
input{width:100%;box-sizing:border-box;padding:10px;margin-top:4px;border:1px solid #aab;border-radius:6px;font-size:16px}
button{width:100%;margin-top:18px;padding:12px;background:#c0262d;color:#fff;border:0;border-radius:6px;font-size:16px}
.err{color:#c0262d;font-weight:bold}.step{font-size:12px;color:#888}
</style></head><body><div class="box">)";

static const char PAGE_WIFI[] PROGMEM = R"(<div class="step">Schritt 1 von 2</div><h1>WLAN verbinden</h1>
<p>Mit welchem WLAN soll sich dein Pager verbinden?</p>%ERR%
<form method="post" action="/wifi">
<label>WLAN-Name</label><input name="ssid" list="nets" required autocomplete="off">
<datalist id="nets">%NETS%</datalist>
<label>Passwort</label><input name="pass" type="password">
<button>Verbinden</button></form>)";

static const char PAGE_ACCOUNT[] PROGMEM = R"(<div class="step">Schritt 2 von 2</div><h1>PagerSpass verknüpfen</h1>
<p>Melde dich mit deinem PagerSpass Konto an, genau wie im Spiel.</p>%ERR%
<form method="post" action="/konto">
<label>Benutzername</label><input name="user" required autocapitalize="none" autocomplete="username">
<label>Passwort</label><input name="pass" type="password" required>
<details><summary style="font-size:13px;margin-top:12px">Erweitert</summary>
<label>Server</label><input name="server" value="%SERVER%"></details>
<button>Anmelden</button></form>)";

static const char PAGE_CODE[] PROGMEM = R"(<div class="step">Schritt 2 von 2</div><h1>Bestätigungscode</h1>
<p>Wir haben dir einen Code geschickt%HINT%.</p>%ERR%
<form method="post" action="/code">
<label>Code</label><input name="code" required inputmode="numeric" autocomplete="one-time-code">
<button>Bestätigen</button></form>)";

static const char PAGE_WAIT[] PROGMEM = R"(<h1 id="t">%TITLE%</h1><p id="m">Bitte kurz warten…</p>
<script>
setInterval(()=>fetch('/state').then(r=>r.json()).then(s=>{
if(['account','wifi_fail','link_fail','wifi','code'].includes(s.state))location='/';
if(s.state=='done'){document.getElementById('t').innerText='Fertig!';
document.getElementById('m').innerText='Dein Pager ist jetzt mit PagerSpass verbunden. Du kannst dieses Fenster schließen.';}
}).catch(()=>{}),1500);
</script>)";

static const char PAGE_DONE[] PROGMEM = R"(<h1>Fertig!</h1><p>Dein Pager ist mit PagerSpass verbunden. Du kannst dieses Fenster schließen.</p>)";

static String htmlEscape(const String &s) {
  String o;
  for (char c : s) {
    if (c == '<') o += "&lt;";
    else if (c == '>') o += "&gt;";
    else if (c == '&') o += "&amp;";
    else if (c == '"') o += "&quot;";
    else o += c;
  }
  return o;
}

static void send(const String &body) {
  server.send(200, "text/html; charset=utf-8", String(FPSTR(PAGE_HEAD)) + body + "</div></body></html>");
}

static String errBlock() {
  return setupError.length() ? "<p class=\"err\">" + htmlEscape(setupError) + "</p>" : "";
}

static void pageWait(const char *title) {
  String p = FPSTR(PAGE_WAIT);
  p.replace("%TITLE%", title);
  send(p);
}

static void handleRoot() {
  switch (setupState) {
    case SS_WIFI:
    case SS_WIFI_FAIL: {
      String p = FPSTR(PAGE_WIFI);
      String nets;
      int n = WiFi.scanComplete();
      for (int i = 0; i < n; i++) nets += "<option value=\"" + htmlEscape(WiFi.SSID(i)) + "\">";
      p.replace("%NETS%", nets);
      p.replace("%ERR%", setupState == SS_WIFI_FAIL ? errBlock() : "");
      send(p);
      break;
    }
    case SS_WIFI_TRY:
      pageWait("Verbinde mit WLAN…");
      break;
    case SS_ACCOUNT:
    case SS_LINK_FAIL: {
      String p = FPSTR(PAGE_ACCOUNT);
      p.replace("%SERVER%", htmlEscape(cfg.server));
      p.replace("%ERR%", setupState == SS_LINK_FAIL ? errBlock() : "");
      send(p);
      break;
    }
    case SS_CODE: {
      String p = FPSTR(PAGE_CODE);
      p.replace("%HINT%", setupHint.length() ? " an " + htmlEscape(setupHint) : "");
      p.replace("%ERR%", errBlock());
      send(p);
      break;
    }
    case SS_LINKING:
      pageWait("Melde an…");
      break;
    case SS_DONE:
      send(FPSTR(PAGE_DONE));
      break;
  }
}

static void handleWifi() {
  if (setupState != SS_WIFI && setupState != SS_WIFI_FAIL) return handleRoot();
  reqSsid = server.arg("ssid");
  reqPass = server.arg("pass");
  reqSsid.trim();
  if (!reqSsid.length()) return handleRoot();
  wifiReq = true;
  setupState = SS_WIFI_TRY;
  pageWait("Verbinde mit WLAN…");
}

static void handleAccount() {
  if (setupState != SS_ACCOUNT && setupState != SS_LINK_FAIL) return handleRoot();
  reqUser = server.arg("user");
  reqUserPass = server.arg("pass");
  reqServer = server.arg("server");
  reqUser.trim();
  reqServer.trim();
  if (!reqServer.length()) reqServer = cfg.server;
  if (!reqServer.startsWith("http://") && !reqServer.startsWith("https://")) reqServer = "https://" + reqServer;
  while (reqServer.endsWith("/")) reqServer.remove(reqServer.length() - 1);
  accReq = true;
  setupState = SS_LINKING;
  pageWait("Melde an…");
}

static void handleCode() {
  if (setupState != SS_CODE) return handleRoot();
  reqCode = server.arg("code");
  reqCode.trim();
  codeReq = true;
  setupState = SS_LINKING;
  pageWait("Melde an…");
}

static void handleState() {
  static const char *names[] = {"wifi", "trying", "wifi_fail", "account", "linking", "link_fail", "code", "done"};
  JsonDocument doc;
  doc["state"] = names[setupState];
  doc["error"] = setupError;
  String s;
  serializeJson(doc, s);
  server.send(200, "application/json", s);
}

static void handleCaptive() {
  if (apOn && server.hostHeader() != WiFi.softAPIP().toString()) {
    server.sendHeader("Location", "http://" + WiFi.softAPIP().toString() + "/", true);
    server.send(302, "text/plain", "");
    return;
  }
  handleRoot();
}

String apName() {
  return "Pager-" + deviceId();
}

void portalBegin(bool withAP) {
  if (withAP) {
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(apName().c_str());
    delay(100);
    dns.setErrorReplyCode(DNSReplyCode::NoError);
    dns.start(53, "*", WiFi.softAPIP());
    WiFi.scanNetworks(true);
    apOn = true;
  }
  MDNS.begin(apName().c_str());

  static bool routes = false;
  if (!routes) {
    server.on("/", HTTP_GET, handleRoot);
    server.on("/wifi", HTTP_POST, handleWifi);
    server.on("/konto", HTTP_POST, handleAccount);
    server.on("/code", HTTP_POST, handleCode);
    server.on("/state", HTTP_GET, handleState);
    server.onNotFound(handleCaptive);
    routes = true;
  }
  if (!running) {
    server.begin();
    running = true;
  }
}

void portalLoop() {
  if (!running) return;
  if (apOn) dns.processNextRequest();
  server.handleClient();
}

void portalStop() {
  if (apOn) {
    dns.stop();
    WiFi.softAPdisconnect(true);
    WiFi.mode(WIFI_STA);
    apOn = false;
  }
  if (running) {
    server.stop();
    running = false;
  }
}

bool portalRunning() {
  return running;
}

bool takeWifiRequest(String &ssid, String &pass) {
  if (!wifiReq) return false;
  wifiReq = false;
  ssid = reqSsid;
  pass = reqPass;
  return true;
}

bool takeAccountRequest(String &srv, String &user, String &pass) {
  if (!accReq) return false;
  accReq = false;
  srv = reqServer;
  user = reqUser;
  pass = reqUserPass;
  reqUserPass = "";
  return true;
}

bool takeCodeRequest(String &code) {
  if (!codeReq) return false;
  codeReq = false;
  code = reqCode;
  return true;
}
