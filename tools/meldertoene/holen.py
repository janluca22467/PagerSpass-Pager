#!/usr/bin/env python3
"""
Holt die Meldertoene aus dem PagerSpass Repo und macht daraus firmware/src/toene_spiel.h

    python3 holen.py /pfad/zu/pagerspass

Braucht node >= 22. Die Toene werden direkt aus web/src/audio/sounds.ts berechnet,
also genau wie im Spiel. Der Pager hat nur einen Piezo, bei Akkorden gewinnt der lauteste Ton.
"""
import json
import os
import subprocess
import sys
import tempfile

spiel = sys.argv[1] if len(sys.argv) > 1 else "../../../pagerspass"
ziel = os.path.join(os.path.dirname(__file__), "../../firmware/src/toene_spiel.h")

src = open(os.path.join(spiel, "web/src/audio/sounds.ts")).read().split("\n")
types = open(os.path.join(spiel, "web/src/types.ts")).read()


def block(start_text):
    start = next(i for i, l in enumerate(src) if l.startswith(start_text))
    end = start
    while src[end] != "}":
        end += 1
    return "\n".join(src[start:end + 1])


start = next(i for i, l in enumerate(src) if l.startswith("const PIEZO"))
ende = next(i for i, l in enumerate(src) if l.startswith("let alarmTimer"))
a = types.index("export const MELDER_TOENE")
b = types.index("\n]", a)

js = "type Tonereignis = any;\n" + block("function folge(") + """
const klangStuecke = new Map();
function tonbauZyklus() { return { dauer: 1, ereignisse: [] }; }
function eigenerZyklus() { return { dauer: 1, ereignisse: [] }; }
""" + "\n".join(src[start:ende]) + "\n" + types[a:b + 2].replace(
    "export const MELDER_TOENE: SchmuckInfo[]", "const MELDER_TOENE") + """
console.log(JSON.stringify(MELDER_TOENE.map(t => ({ id: t.id, name: t.name, z: [1, 2, 3].map(p => melderzyklus(t.id, p)) }))));
"""

with tempfile.NamedTemporaryFile("w", suffix=".ts", delete=False) as f:
    f.write(js)
daten = json.loads(subprocess.check_output(["node", "--experimental-strip-types", f.name], stderr=subprocess.DEVNULL))
os.unlink(f.name)


def einstimmig(zyklus):
    ev = zyklus["ereignisse"]
    punkte = sorted({0.0, zyklus["dauer"]} | {e["versatz"] for e in ev} | {e["versatz"] + e["dauer"] for e in ev})
    teile = []
    for t0, t1 in zip(punkte, punkte[1:]):
        if t1 - t0 < 0.0005:
            continue
        mitte = (t0 + t1) / 2
        an = [e for e in ev if e["versatz"] <= mitte < e["versatz"] + e["dauer"]]
        if not an:
            f0 = f1 = 0
        else:
            e = max(an, key=lambda e: e["pegel"])
            bis = e.get("bis", e["frequenz"])
            anteil = lambda t: (t - e["versatz"]) / e["dauer"]
            f0 = e["frequenz"] + (bis - e["frequenz"]) * anteil(t0)
            f1 = e["frequenz"] + (bis - e["frequenz"]) * anteil(t1)
        ms = round((t1 - t0) * 1000)
        if teile and teile[-1][0] == 0 and f0 == 0:
            teile[-1][2] += ms
            continue
        teile.append([round(f0), round(f1), ms])
    return teile


zeilen = ["// automatisch erzeugt von tools/meldertoene/holen.py, nicht von Hand aendern", "#pragma once", "#include <Arduino.h>", "",
          "struct Seg { uint16_t f0, f1, ms; };", "struct SpielTon { const char *id; const char *name; const Seg *seg[3]; uint8_t len[3]; };", ""]
namen = []
for t in daten:
    var = "t_" + t["id"]
    for p, z in enumerate(t["z"]):
        teile = einstimmig(z)
        zeilen.append(f"static const Seg {var}_{p}[] PROGMEM = {{" + ", ".join(f"{{{a},{b},{c}}}" for a, b, c in teile) + "};")
    namen.append(t)
zeilen += ["", "static const SpielTon SPIEL_TOENE[] = {"]
for t in namen:
    var = "t_" + t["id"]
    lens = ", ".join(f"sizeof({var}_{p}) / sizeof(Seg)" for p in range(3))
    zeilen.append(f'  {{"{t["id"]}", "{t["name"]}", {{{var}_0, {var}_1, {var}_2}}, {{{lens}}}}},')
zeilen += ["};", "static const uint8_t SPIEL_TOENE_ANZAHL = sizeof(SPIEL_TOENE) / sizeof(SPIEL_TOENE[0]);", ""]
open(ziel, "w").write("\n".join(zeilen))
print(f"{len(namen)} Toene nach {os.path.normpath(ziel)}")
