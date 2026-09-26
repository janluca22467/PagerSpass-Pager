# Teileliste

| Nr | Teil | Anzahl | ca. Preis | Hinweis |
|---|---|---|---|---|
| U1 | **WeMos D1 Mini V4.0.0 Type-C** (ESP8266, ESP-12F, CH340G) | 1 | 3–4 € | das „Gehirn“ |
| U2 | **1.9" TFT IPS, ST7789, 170x320, SPI** | 1 | 4–5 € | 8 Pins: GND VCC SCL SDA RES DC CS BLK |
| U3 | TP4056 Lademodul USB-C **mit Schutzschaltung** | 1 | 0,50 € | die Version mit OUT+/OUT- (6 Lötpunkte) |
| BT1 | LiPo Akku 503450, 3.7 V, 1000 mAh | 1 | 4 € | 50 x 34 x 5 mm, mit Schutzplatine und Kabel |
| LS1 | Mini Lautsprecher 8 Ω, 0,5 W, Ø 20 mm | 1 | 1 € | im Gehäuse ist Platz für Ø 20 mm |
| Q1 | NPN Transistor S8050 (oder 2N2222 / BC337) | 1 | – | |
| D1 | Diode 1N4148 | 1 | – | |
| D2 | LED rot 3 mm | 1 | – | |
| R1 | Widerstand 1 kΩ | 1 | – | |
| R2 | Widerstand 10 Ω | 1 | – | |
| R3 | Widerstand 330 Ω | 1 | – | |
| R4 | Widerstand 100 kΩ | 1 | – | Akku messen |
| S1–S4 | **Taster 6x6x4.3 mm** (4 Beinchen) | 4 | – | |
| SW1 | Schiebeschalter SS12D00 | 1 | – | |
| – | Lochrasterplatine 5 x 7 cm | 1 | – | für die Tasten oben, siehe Bauanleitung |
| – | Schrauben M2 x 10 (selbstschneidend) | 4 | – | Gehäuse |
| – | Schrauben M2 x 6 Senkkopf | 2 | – | Gürtelclip |
| – | Litze 0,14 mm², Schrumpfschlauch, Heißkleber, doppelseitiges Klebeband | | | |

Widerstände, Diode, Transistor und LED gibt es am günstigsten als Sortiment.

**Summe:** ca. 15–20 € (ohne Filament)

## Worauf man beim Kaufen achten sollte

- **D1 Mini:** genau der ESP8266 (ESP-12F). Nicht „D1 Mini ESP32“, das ist ein anderes Board mit anderen Pins.
- **Display:** 1.9 Zoll mit **170x320** und **ST7789**. Es gibt auch 1.9" mit anderer Auflösung, die gehen nicht ohne Änderung.
- **TP4056:** die Version **mit** Schutz (hat OUT+ und OUT-). Die ohne Schutz hat nur B+/B-.
- **Akku:** mit Schutzplatine, Kabel dran. Maße ungefähr 50 x 34 x 5 mm, sonst passt er nicht ins Gehäuse.
- **Taster:** 6x6 mm, **4,3 mm hoch**. Die Tastenkappen im Gehäuse sind genau darauf abgestimmt.

## Werkzeug

- Lötkolben
- 3D-Drucker (PLA oder PETG)
- Heißklebepistole
- Cutter oder feine Säge (für die Lochrasterplatine)
- kleiner Kreuzschlitz
- Mac/PC mit VS Code + PlatformIO zum Flashen
