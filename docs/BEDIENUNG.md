# Bedienung

## Tasten

```
   [ZURÜCK]  [ HOCH ]  [RUNTER]
 ┌──────────────────────────┬───┐
 │                          │ ● │  ← LED
 │        Display           │   │
 │                          │ ◉ │  ← OK (Seite)
 │                          │   │
 │ PagerSpass        PS-1   │   │
 └──────────────────────────┴───┘
```

| Taste | kurz | lang |
|---|---|---|
| OK (Seite) | Menü / auswählen / Alarm quittieren | in der Nachrichtenliste: alle löschen |
| HOCH / RUNTER | blättern | schnell blättern |
| ZURÜCK | zurück | – |

Wenn das Display aus ist, macht der erste Tastendruck nur das Licht an. Beim Alarm quittiert jede Taste sofort.

## Erster Start

1. **Willkommen** – Einschalten, das Logo kommt.
2. **WLAN** – Auf dem Display steht `Pager-XXXX`. Mit dem Handy in dieses WLAN gehen. Die Einrichtungsseite geht normalerweise von selbst auf, sonst im Browser `http://192.168.4.1` öffnen. WLAN auswählen, Passwort rein, „Verbinden“.
3. **PagerSpass verknüpfen** – Die Seite wechselt automatisch. Benutzername und Passwort von deinem PagerSpass Konto eingeben, „Verknüpfen“.
4. **Fertig** – Der Pager zeigt die Uhr und „Warte auf Runde…“.

Das Passwort wird nicht im Pager gespeichert, nur ein Token vom Server.

## Startbildschirm

- oben: WLAN-Empfang und Akku
- Mitte: Uhrzeit und Datum
- unten: Status
  - „Warte auf Runde…“ – verbunden, keine Runde aktiv
  - „Runde: Name“ – du bist in einer Runde, Alarme kommen an
  - „Keine Verbindung“ – Server nicht erreichbar, er versucht es alle paar Sekunden
  - „x neue Nachrichten“

## Alarm

Kommt ein Alarm:

- Alarmton (30 Sekunden lang), rote LED blinkt
- Display: Datum, Uhrzeit und `Adr.1` oben im Balken, darunter der Text – wie im Spiel
- Eine Taste drücken = **quittiert**. Ton und LED gehen aus, PagerSpass bekommt die Quittung.

Bei langen Texten mit HOCH/RUNTER scrollen.

## Menü

OK auf dem Startbildschirm:

| Punkt | Was |
|---|---|
| Nachrichten | Die letzten 20 Alarme. OK öffnet, in der Nachricht OK = löschen |
| Lautstärke | Aus, 1–5 |
| Alarmton | Standard, Zweiton, Sirene, Lang (spielt beim Auswählen an) |
| Helligkeit | 1–5 |
| Stumm | An/Aus – nur LED, kein Ton |
| Info | Gerät, Firmware, IP, WLAN, Akku … |
| WLAN ändern | WLAN löschen, startet wieder mit Schritt 2 |
| Konto trennen | Pager vom PagerSpass Konto lösen, startet mit Schritt 3 |
| Werkseinstellungen | alles löschen |

## Zurücksetzen ohne Menü

HOCH + RUNTER gedrückt halten, einschalten, 5 Sekunden halten.

## Laden

USB-C Buchse unten in der Mitte (die linke ist nur zum Flashen). Rote LED am Modul = lädt, blau/grün = voll. Laden geht auch wenn der Pager aus ist.
