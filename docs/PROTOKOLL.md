# Protokoll

Damit PagerSpass (oder ein eigener Server) mit dem Pager redet. Alles JSON, Server-Adresse ist im Pager einstellbar (`https://...` oder `http://...:port`).

## 1. Verknüpfen

```
POST /api/pager/link
Content-Type: application/json

{
  "user": "jan",
  "password": "geheim",
  "device": "Pager-A1B2",
  "fw": "1.0.0"
}
```

Antwort OK (`200`):

```json
{ "token": "abc123...", "name": "Jan" }
```

Fehler (`401` o.ä.):

```json
{ "error": "Benutzername oder Passwort falsch" }
```

Der Text in `error` wird so auf der Einrichtungsseite angezeigt.

## 2. Verbindung

WebSocket:

```
/api/pager/ws?token=<token>&device=Pager-A1B2
```

Der Pager hält die Verbindung offen und verbindet sich alle 5 s neu, wenn sie abbricht. Ping/Pong vom WebSocket selbst wird unterstützt.

## Server → Pager

Runde:

```json
{ "t": "round", "state": "active", "name": "Wache 1" }
{ "t": "round", "state": "waiting" }
```

Am besten direkt nach dem Verbinden einmal den aktuellen Stand schicken.

Alarm:

```json
{
  "t": "alarm",
  "id": "a1b2c3",
  "text": "Nachrichtentechnik\nEmmerl",
  "adr": 1,
  "prio": 0,
  "ts": 1790374214
}
```

| Feld | |
|---|---|
| `id` | beliebig, kommt bei der Quittung zurück |
| `text` | max. ~190 Zeichen, `\n` = neue Zeile, Umlaute gehen |
| `adr` | Unteradresse 1–4, wird als `Adr.1` angezeigt |
| `prio` | 1 = hohe Priorität (zeigt `!` im Balken) |
| `ts` | Unix-Zeit, optional (sonst Uhrzeit vom Pager) |

Token ungültig / Pager im Konto gelöscht:

```json
{ "t": "unlinked" }
```

→ Pager löscht den Token und startet die Verknüpfung neu.

## Pager → Server

Quittung:

```json
{ "t": "ack", "id": "a1b2c3" }
```

Status (jede Minute):

```json
{ "t": "status", "bat": 87, "rssi": -61, "fw": "1.0.0" }
```

`bat` ist -1 wenn kein Akku gemessen wird.

## Beispiel

`tools/testserver/server.py` macht genau das, gut 100 Zeilen Python. Kann man gut als Vorlage nehmen.
