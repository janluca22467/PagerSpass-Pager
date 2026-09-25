# Protokoll

So redet der Pager mit PagerSpass. Standard-Server ist `https://pagerspass.de`, kann bei der Einrichtung unter „Erweitert“ geändert werden (z. B. `https://beta.pagerspass.de`).

## 1. Anmelden

Ganz normal mit dem PagerSpass Konto, derselbe Weg wie im Spiel:

```
POST /api/konto/anmelden
{ "benutzername": "jan", "passwort": "geheim" }
```

Antwort: das Konto mit `merkmal` (das ist der Token). Den speichert der Pager, das Passwort nicht.

Bei Zwei-Faktor kommt stattdessen `{"zweiFaktor":true,"anfrage":"…","ziel":"j***@…"}`. Die Einrichtungsseite fragt dann den Code ab:

```
POST /api/konto/anmelden/zweifaktor
{ "anfrage": "…", "code": "123456" }
```

Fehler kommen als `{"fehler":"…"}` und werden so auf der Einrichtungsseite angezeigt.

## 2. Verbindung

WebSocket auf

```
/hub/pager
Authorization: Bearer <merkmal>
```

Kein Premium, kein QR-Code nötig. Der Pager bekommt die Alarme vom eigenen Platz in der Runde, der PC spielt ganz normal weiter.

## Server → Pager

Runde (bei jeder Änderung, alle ~10 s geprüft):

```json
{ "t": "round", "state": "waiting" }
{ "t": "round", "state": "lobby",  "name": "Stade", "code": "ABC123", "funkrufname": null }
{ "t": "round", "state": "active", "name": "Stade", "code": "ABC123", "funkrufname": "Florian Stade 1/46-1" }
```

Alarm – genau der Alarm vom Melder im Spiel:

```json
{
  "t": "alarm",
  "alarm": {
    "incidentId": "e4c82be9",
    "einsatznummer": "E-0002",
    "schleife": "FW Stade",
    "stichwort": "B3",
    "stichwortText": "Wohnhausbrand",
    "meldebild": "Rauch aus Dachstuhl",
    "adresse": "Hauptstraße 5",
    "ortsteil": null,
    "prioritaet": 3,
    "zeit": "2026-09-25T18:30:00+00:00",
    "einheiten": ["HLF 1/46-1", "DLK 1/33-1"],
    "zusatztext": null,
    "funkgruppe": "3301 Kreis West"
  }
}
```

Der Pager baut daraus dieselben Zeilen wie der DME im Spiel:

```
B3 Wohnhausbrand
Hauptstraße 5
Rauch aus Dachstuhl
GRUPPE 3301 Kreis West
EINH: HLF 1/46-1, DLK 1/33-1
```

Oben im Balken stehen Zeit und Schleife.

Merkmal ungültig (z. B. Passwort geändert):

```json
{ "t": "unlinked" }
```

→ Pager startet die Anmeldung neu.

## Pager → Server

Quittieren:

```json
{ "t": "ack" }
```

Das ist dasselbe wie „Quittieren“ im Spiel.

## Testserver

`tools/testserver/server.py` macht dasselbe ohne Spiel, zum Ausprobieren.
