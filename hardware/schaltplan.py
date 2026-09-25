# erzeugt schaltplan.svg / schaltplan.png
# pip install schemdraw matplotlib
import schemdraw
import schemdraw.elements as elm

schemdraw.use("matplotlib")
schemdraw.config(fontsize=11, lw=1.2)


def net(d, name, at, direction="right", length=1.0):
    line = d.add(getattr(elm.Line(), direction)().at(at).length(length))
    d.add(elm.Dot(open=True).at(line.end).label(name, loc={"right": "right", "left": "left", "up": "top", "down": "bottom"}[direction]))


with schemdraw.Drawing(file="schaltplan.svg", show=False) as d:
    d.config(unit=2)

    esp = d.add(elm.Ic(
        pins=[
            *[elm.IcPin(n, side="left", slot=f"{8 - i}/8") for i, n in
              enumerate(["5V", "GND", "3V3", "GPIO4", "GPIO3", "GPIO2", "GPIO1", "GPIO0"])],
            *[elm.IcPin(n, side="right", slot=f"{8 - i}/8") for i, n in
              enumerate(["GPIO5", "GPIO6", "GPIO7", "GPIO8", "GPIO9", "GPIO10", "GPIO20", "GPIO21"])],
        ],
        edgepadW=2.2, pinspacing=0.9, leadlen=0.6,
        label="ESP32-C3\nSuperMini",
    ).at((0, 0)))

    for pin, name in [("5V", "VSYS"), ("GND", "GND"), ("3V3", "3V3"), ("GPIO4", "TFT_SCK"),
                      ("GPIO3", "TFT_RST"), ("GPIO2", "TFT_DC"), ("GPIO1", "LED"), ("GPIO0", "BAT_ADC")]:
        net(d, name, getattr(esp, pin), "left")
    for pin, name in [("GPIO5", "SPK"), ("GPIO6", "TFT_SDA"), ("GPIO7", "TFT_CS"), ("GPIO8", "BTN_ZURUECK"),
                      ("GPIO9", "BTN_OK"), ("GPIO10", "TFT_BLK"), ("GPIO20", "BTN_HOCH"), ("GPIO21", "BTN_RUNTER")]:
        net(d, name, getattr(esp, pin), "right")

    tft = d.add(elm.Ic(
        pins=[elm.IcPin(n, side="left", slot=f"{8 - i}/8") for i, n in
              enumerate(["GND", "VCC", "SCL", "SDA", "RES", "DC", "CS", "BLK"])],
        edgepadW=1.8, pinspacing=0.9, leadlen=0.6,
        label="TFT 2.0\"\nST7789\n240x320",
    ).at((14, 0)))
    for pin, name in zip(["GND", "VCC", "SCL", "SDA", "RES", "DC", "CS", "BLK"],
                         ["GND", "3V3", "TFT_SCK", "TFT_SDA", "TFT_RST", "TFT_DC", "TFT_CS", "TFT_BLK"]):
        net(d, name, getattr(tft, pin), "left")

    # Stromversorgung
    x0, y0 = -16, -8
    d.add(elm.Label().at((x0 - 1, y0 + 2.2)).label("Stromversorgung", loc="right", fontsize=13))
    tp = d.add(elm.Ic(
        pins=[elm.IcPin("B+", side="left", slot="2/2"), elm.IcPin("B-", side="left", slot="1/2"),
              elm.IcPin("OUT+", side="right", slot="2/2"), elm.IcPin("OUT-", side="right", slot="1/2")],
        edgepadW=1.8, pinspacing=2, leadlen=0.5, label="TP4056\nUSB-C\n(mit Schutz)",
    ).theta(0).at((x0 + 3, y0 - 1)))
    bx = x0
    d.add(elm.Line().at(tp["B+"]).to((bx, tp["B+"][1])))
    d.add(elm.Line().at(tp["B-"]).to((bx, tp["B-"][1])))
    d.add(elm.BatteryCell().at((bx, tp["B-"][1])).to((bx, tp["B+"][1])).label("LiPo 3.7V\n1000mAh", loc="bottom"))
    d.add(elm.Line().right().at(tp["OUT+"]).length(0.8))
    sw = d.add(elm.Switch().right().label("SW1\nEin/Aus", loc="top"))
    d.add(elm.Line().right().length(0.5))
    d.add(elm.Dot(open=True).label("VSYS", loc="right"))
    d.add(elm.Line().right().at(tp["OUT-"]).length(3.3))
    d.add(elm.Ground())

    # Akkuspannung
    xa = x0 + 13
    d.add(elm.Label().at((xa - 1, y0 + 2.2)).label("Akku messen", loc="right", fontsize=13))
    d.add(elm.Line().up().at((xa, y0)).length(0.5))
    d.add(elm.Dot(open=True).label("VSYS", loc="top"))
    r4 = d.add(elm.Resistor().down().at((xa, y0)).label("R4\n100k", loc="bottom"))
    d.add(elm.Dot())
    d.add(elm.Line().right().length(0.8))
    d.add(elm.Dot(open=True).label("BAT_ADC", loc="right"))
    d.add(elm.Resistor().down().at(r4.end).label("R5\n100k", loc="bottom"))
    d.add(elm.Ground())

    # Lautsprecher
    xs = x0 + 19
    d.add(elm.Label().at((xs - 1, y0 + 2.2)).label("Lautsprecher", loc="right", fontsize=13))
    xk = xs + 3
    d.add(elm.Line().up().at((xk, y0)).length(0.5))
    d.add(elm.Dot(open=True).label("VSYS", loc="top"))
    d.add(elm.Resistor().down().at((xk, y0)).to((xk, y0 - 2.3)).label("R2 10Ω", loc="bottom"))
    d.add(elm.Dot())
    d.add(elm.Line().down().to((xk, y0 - 3)))
    d.add(elm.Line().right().length(0.6))
    spk = d.add(elm.Speaker().theta(0).anchor("in1").label("LS1 8Ω 0.5W", loc="right", ofst=(0.9, -0.3)))
    d.add(elm.Line().at(spk.in2).to((xk, spk.in2[1])))
    d.add(elm.Line().down().to((xk, y0 - 4.2)))
    d.add(elm.Dot())
    d.add(elm.Line().down().to((xk, y0 - 5.4)))
    q = d.add(elm.BjtNpn(circle=True).theta(0).anchor("collector").label("Q1\nS8050", loc="right"))
    d.add(elm.Ground().at(q.emitter))
    d.add(elm.Resistor().left().at(q.base).length(2).label("R1 1k", loc="bottom"))
    d.add(elm.Dot(open=True).label("SPK", loc="left"))
    d.add(elm.Line().at((xk, y0 - 2.3)).to((xk - 2.4, y0 - 2.3)))
    d.add(elm.Diode().at((xk - 2.4, y0 - 4.2)).to((xk - 2.4, y0 - 2.3)).label("D1\n1N4148", loc="top"))
    d.add(elm.Line().at((xk - 2.4, y0 - 4.2)).to((xk, y0 - 4.2)))

    # LED
    xl = x0 + 30
    d.add(elm.Label().at((xl - 1, y0 + 2.2)).label("Alarm-LED", loc="right", fontsize=13))
    d.add(elm.Line().up().at((xl, y0)).length(0.5))
    d.add(elm.Dot(open=True).label("LED", loc="top"))
    d.add(elm.Resistor().down().at((xl, y0)).label("R3\n330Ω", loc="bottom"))
    d.add(elm.LED().down().label("D2 rot\n3mm", loc="bottom"))
    d.add(elm.Ground())

    # Tasten
    xt = x0 + 35
    d.add(elm.Label().at((xt - 1, y0 + 2.2)).label("Tasten (interner Pull-Up)", loc="right", fontsize=13))
    for i, name in enumerate(["BTN_OK", "BTN_HOCH", "BTN_RUNTER", "BTN_ZURUECK"]):
        x = xt + i * 3
        d.add(elm.Line().up().at((x, y0)).length(0.5))
        d.add(elm.Dot(open=True).label(name, loc="top", fontsize=9))
        d.add(elm.Button().down().at((x, y0)).label(f"S{i + 1}", loc="bottom"))
        d.add(elm.Ground())

    d.save("schaltplan.png", dpi=160)
