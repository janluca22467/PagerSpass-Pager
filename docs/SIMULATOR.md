# Pager am Mac testen (ohne Hardware)

Mit **Wokwi** läuft die echte Firmware im Simulator: Display, Tasten, LED und Piepser sind auf dem Bildschirm, WLAN geht auch.

Einziger Unterschied zum echten Gerät: Wokwi hat kein ST7789-Display, darum nimmt der Simulator-Build ein ILI9341 (auch 320x240). Sonst ist alles gleich.

## Einmal einrichten

1. **VS Code** installieren: https://code.visualstudio.com
2. In VS Code bei Erweiterungen installieren:
   - **PlatformIO IDE**
   - **Wokwi Simulator**
3. `F1` → „Wokwi: Request a new License“ → im Browser bestätigen (für Hobby kostenlos)
4. **Python 3** ist auf dem Mac meistens schon da (`python3 --version` im Terminal). Für den Testserver:
   ```
   cd tools/testserver
   pip3 install -r requirements.txt
   ```

## Starten

1. In VS Code **nur den Ordner `firmware/`** öffnen (Datei → Ordner öffnen)
2. Unten in der blauen Leiste bei PlatformIO die Umgebung **`env:sim`** wählen und auf ✓ (Build) klicken
   – oder im Terminal: `pio run -e sim`
3. `diagram.json` öffnen → der Pager wird angezeigt
4. `F1` → **„Wokwi: Start Simulator“**

Der Pager startet, zeigt „Willkommen“ und verbindet sich automatisch mit dem Simulator-WLAN (`Wokwi-GUEST`). Die WLAN-Einrichtung wird im Simulator übersprungen.

## Testen mit dem Testserver (ohne Spiel)

1. Terminal auf dem Mac:
   ```
   cd tools/testserver
   python3 server.py
   ```
2. Im Browser **http://localhost:8180** öffnen → das ist die Einrichtungsseite vom simulierten Pager
3. Benutzername egal, Passwort `test` → „Anmelden“
   (Server ist im Simulator schon auf `http://host.wokwi.internal:8080` gestellt = dein Mac)
4. Im Terminal vom Testserver:
   ```
   /start Stade
   B3 Wohnhausbrand | Hauptstraße 5 | Rauch aus Dachstuhl
   ```
   → Pager piept, LED blinkt, Meldung erscheint

## Testen mit dem echten Spiel

Geht genauso, nur auf der Einrichtungsseite unter „Erweitert“ als Server `https://pagerspass.de` eintragen und mit dem normalen Konto anmelden. Dann im Spiel einer Runde als Fahrzeug beitreten und alarmieren lassen.

(Der Server muss dafür den Pager-Anschluss schon haben, also nach dem Deploy.)

## Tasten

Man kann die Knöpfe anklicken oder die Tastatur nehmen (vorher einmal ins Simulator-Fenster klicken):

| Taste | Knopf |
|---|---|
| Enter | OK |
| Pfeil hoch | HOCH |
| Pfeil runter | RUNTER |
| Esc | ZURÜCK |

## Log

Unten in VS Code im Terminal-Bereich zeigt Wokwi die Ausgabe vom Pager, z. B.:

```
[boot] PagerSpass Pager 1.0.0
[boot] Display gestartet
[wifi] verbinde mit Wokwi-GUEST
[wifi] verbunden, IP 10.13.37.2
[setup] warte auf Anmeldung, Einrichtungsseite ist offen
```

Wenn was nicht geht: diese Zeilen angucken, da steht wo es hängt.

## Probleme

| Problem | Lösung |
|---|---|
| „firmware.bin not found“ | Erst bauen mit `env:sim`, nicht `env:pager` |
| localhost:8180 geht nicht | Simulator läuft? Pager zeigt „PagerSpass verknüpfen“? Ein paar Sekunden warten |
| „Server nicht erreichbar“ beim Testserver | Läuft `python3 server.py`? Server muss `http://host.wokwi.internal:8080` sein |
| Display bleibt schwarz | Nach dem Ändern immer neu bauen (`env:sim`) und den Simulator neu starten. Im Log muss „Display gestartet“ stehen |
| Alles zurücksetzen | Menü → Werkseinstellungen, oder Simulator neu starten und HOCH+RUNTER halten |
