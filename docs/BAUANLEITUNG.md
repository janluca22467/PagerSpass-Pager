# Bauanleitung

Dauer: ungefähr ein Nachmittag (+ Druckzeit).

## 1. Gehäuse drucken

Die fertigen Dateien liegen in `case/stl/`:

| Datei | Was | Ausrichtung |
|---|---|---|
| `pager_front.stl` | Vorderteil mit Display-Fenster | Vorderseite aufs Bett (liegt schon richtig) |
| `pager_back.stl` | Rückteil mit Lautsprecher-Löchern | Rückseite aufs Bett (liegt schon richtig) |
| `pager_buttons.stl` | 3 Tasten oben + 1 Seitentaste | liegt richtig |
| `pager_clip.stl` | Gürtelclip | liegt richtig, **nicht drehen** (sonst bricht er) |

Einstellungen die bei mir gut gingen:

- PLA oder PETG (Clip besser PETG)
- 0,2 mm Schichthöhe, 3 Wände, 20 % Infill
- Stützstrukturen: nur „nur vom Druckbett“ für Vorder- und Rückteil (USB-Löcher, Seitentaste)
- Schwarz oder Dunkelgrau sieht am meisten nach echtem Melder aus

Wer was ändern will (anderes Display, dickerer Akku …): `case/pager.scad` in OpenSCAD öffnen, oben stehen alle Maße. Danach z. B.

```
openscad -D 'part="front"' -o stl/pager_front.stl pager.scad
```

![offen](img/gehaeuse_offen.png)

> **Wichtig:** Die Display-Module sind nicht alle gleich groß. Miss deins nach und trag die Maße in `disp_pcb` und `disp_win` ein bevor du druckst.

## 2. Schaltplan

![Schaltplan](../hardware/schaltplan.png)

### Verdrahtung

| ESP32-C3 | geht an |
|---|---|
| 5V | VSYS (Schiebeschalter → TP4056 OUT+) |
| GND | GND von allem |
| 3V3 | Display VCC |
| GPIO0 | Spannungsteiler Akku (Mitte von R4/R5) |
| GPIO1 | R3 330 Ω → LED → GND |
| GPIO2 | Display DC |
| GPIO3 | Display RES |
| GPIO4 | Display SCL |
| GPIO5 | R1 1 kΩ → Basis Q1 (Lautsprecher) |
| GPIO6 | Display SDA |
| GPIO7 | Display CS |
| GPIO8 | Taster ZURÜCK → GND |
| GPIO9 | Taster OK (Seite) → GND |
| GPIO10 | Display BLK |
| GPIO20 | Taster HOCH → GND |
| GPIO21 | Taster RUNTER → GND |

Die Taster brauchen keine Widerstände, das macht der ESP intern (Pull-Up).

Lautsprecher: VSYS → R2 10 Ω → Lautsprecher → Kollektor Q1, Emitter an GND. Die 1N4148 parallel zum Lautsprecher, Strich (Kathode) Richtung VSYS.

## 3. Löten

Reihenfolge wie ich es gemacht habe:

1. **Tastenplatine:** Lochraster-Streifen ca. 60 x 12 mm. Drei Taster drauf, Abstand von links (von vorne gesehen): 16 mm, 38 mm, 57 mm Mitte. Das sind ZURÜCK, HOCH, RUNTER. Eine gemeinsame GND-Leitung, jeder Taster ein eigenes Kabel.
2. **Display:** 8 Kabel an das Display löten (ca. 8 cm lang). Keine Stiftleiste, die ist zu hoch!
3. **Lautsprecher-Teil:** Q1, R1, R2 und D1 fliegend oder auf einem Mini-Stück Lochraster, gut mit Schrumpfschlauch isolieren.
4. **LED:** R3 direkt an das Beinchen löten, Schrumpfschlauch drüber.
5. **Seitentaste (OK):** Einzelner Taster, zwei Kabel.
6. **Akku messen:** R4 und R5 in Reihe, Mitte an GPIO0.
7. **Strom:** Akku an B+/B- vom TP4056. OUT+ an den Schiebeschalter, vom Schalter an 5V vom ESP. OUT- an GND.

> Akku-Kabel **nie** kurzschließen. Erst alles andere fertig machen, Akku zum Schluss anlöten.

## 4. Firmware flashen

1. [VS Code](https://code.visualstudio.com/) + PlatformIO Erweiterung installieren
2. Ordner `firmware/` öffnen
4. **Schiebeschalter auf AUS**, dann den ESP per USB-C anstecken
5. Unten auf den Pfeil (Upload) klicken

Falls der Upload nicht klappt: OK-Taste (bzw. BOOT auf dem ESP) gedrückt halten, USB einstecken, loslassen, nochmal Upload.

Über die Konsole geht es auch:

```
cd firmware
pio run -t upload
pio device monitor
```

## 5. Zusammenbauen

1. Display ins Vorderteil legen, Glas Richtung Fenster. Es sitzt im Rahmen, mit ein paar Punkten Heißkleber am Rand fixieren.
2. Tastenkappen von innen in die Löcher oben stecken, dann die Tastenplatine in die zwei Halter schieben (bisschen Heißkleber).
3. LED von innen in das kleine Loch rechts oben (Walze) drücken, Kleber drauf.
4. Seitentaste: Kappe in die halbe Öffnung an der Walze legen, Taster dahinter mit Heißkleber fest.
5. Akku mit doppelseitigem Klebeband hinter das Display kleben.
6. Im Rückteil: Lautsprecher in den Ring kleben, ESP32 und TP4056 zwischen die Führungen legen, USB-Buchsen in die Öffnungen unten. Mit Heißkleber fixieren.
7. Schiebeschalter in den Schlitz an der linken Seite kleben.
8. Kabel ordentlich legen, zuklappen, 4 Schrauben M2 x 10 von hinten.
9. Gürtelclip mit 2 Senkkopfschrauben hinten dran. Durch die Löcher im Clip kommt man mit dem Schraubenzieher an die Schrauben.

## 6. Erster Test ohne Spiel

Im Ordner `tools/testserver` ist ein kleiner Server der so tut als wäre er PagerSpass:

```
cd tools/testserver
pip install -r requirements.txt
python server.py
```

Bei der Einrichtung unter „Erweitert“ als Server `http://<IP von deinem PC>:8080` eintragen, Benutzername egal, Passwort `test`.

Dann im Terminal:

```
/start Stade
B3 Wohnhausbrand | Hauptstraße 5 | Rauch aus Dachstuhl
```

→ Pager sollte piepen. Mehr Befehle stehen beim Start im Terminal.

## Probleme

| Problem | Lösung |
|---|---|
| Display bleibt weiß/schwarz | Kabel SCL/SDA/DC/CS prüfen, BLK an GPIO10? |
| Farben komisch/invertiert | Bei manchen Displays in `display.cpp` nach `tft.init` ein `tft.invertDisplay(false);` einfügen |
| Kein Ton | Q1 richtig rum? (E-B-C) R2 dran? |
| Pager startet immer neu | Akku leer oder Schalter wackelt |
| Akku zeigt „USB“ | R4/R5 nicht angeschlossen |
| WLAN „Pager-XXXX“ taucht nicht auf | Alles zurücksetzen: HOCH + RUNTER beim Einschalten 5 s halten |
