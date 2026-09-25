#!/usr/bin/env python3
"""
Kleiner Testserver fuer den PagerSpass Pager.
Spricht dasselbe wie PagerSpass (/api/konto/anmelden + /hub/pager), nur ohne Spiel drumrum.

    pip install -r requirements.txt
    python server.py

Im Pager als Server dann http://<ip-vom-pc>:8080 eintragen.
"""
import asyncio
import json
import secrets
import sys
from datetime import datetime, timezone

from aiohttp import web

PORT = 8080
PASSWORD = "test"

tokens = {}
pagers = {}
round_state = {"t": "round", "state": "waiting"}


async def login(request):
    data = await request.json()
    user = data.get("benutzername", "")
    if not user or data.get("passwort") != PASSWORD:
        return web.json_response({"fehler": "Benutzername oder Passwort falsch."}, status=400)
    token = secrets.token_urlsafe(32)
    tokens[token] = user
    print(f"[login] {user}")
    return web.json_response({"kennung": user, "benutzername": user, "anzeigename": user, "merkmal": token})


async def ws_handler(request):
    auth = request.headers.get("Authorization", "")
    token = auth[7:] if auth.startswith("Bearer ") else request.query.get("access_token", "")
    ws = web.WebSocketResponse(heartbeat=20)
    await ws.prepare(request)
    if token not in tokens:
        await ws.send_json({"t": "unlinked"})
        await ws.close()
        return ws

    user = tokens[token]
    pagers[token] = ws
    print(f"[ws] Pager von {user} verbunden")
    await ws.send_json(round_state)

    async for msg in ws:
        if msg.type == web.WSMsgType.TEXT and json.loads(msg.data).get("t") == "ack":
            print(f"[ack] {user} hat quittiert")

    pagers.pop(token, None)
    print(f"[ws] Pager von {user} getrennt")
    return ws


async def broadcast(obj):
    for ws in list(pagers.values()):
        try:
            await ws.send_json(obj)
        except ConnectionResetError:
            pass


async def console():
    loop = asyncio.get_running_loop()
    print("Befehle: /start <ort>, /lobby <ort>, /ende, /prio <text>, /quit")
    print("Alles andere wird als Alarm geschickt (Format: STICHWORT Text | Adresse | Meldebild).")
    while True:
        line = (await loop.run_in_executor(None, sys.stdin.readline)).strip()
        if not line:
            continue
        prio = 1
        if line == "/quit":
            break
        if line.startswith("/start") or line.startswith("/lobby"):
            state = "active" if line.startswith("/start") else "lobby"
            round_state.clear()
            round_state.update(t="round", state=state, name=line[6:].strip() or "Teststadt",
                               code="TEST01", funkrufname="Florian Test 1/46-1")
            await broadcast(round_state)
            continue
        if line == "/ende":
            round_state.clear()
            round_state.update(t="round", state="waiting")
            await broadcast(round_state)
            continue
        if line.startswith("/prio "):
            prio, line = 3, line[6:]
        teile = [t.strip() for t in line.split("|")]
        kopf = teile[0].split(" ", 1)
        alarm = {
            "incidentId": secrets.token_hex(4),
            "einsatznummer": "E-0001",
            "schleife": "FW Test",
            "stichwort": kopf[0],
            "stichwortText": kopf[1] if len(kopf) > 1 else "",
            "adresse": teile[1] if len(teile) > 1 else "Musterstraße 1",
            "ortsteil": None,
            "meldebild": teile[2] if len(teile) > 2 else "",
            "prioritaet": prio,
            "zeit": datetime.now(timezone.utc).isoformat(),
            "einheiten": ["Florian Test 1/46-1"],
            "zusatztext": None,
            "funkgruppe": None,
        }
        await broadcast({"t": "alarm", "alarm": alarm})
        print(f"[alarm] an {len(pagers)} Pager gesendet")


async def main():
    app = web.Application()
    app.router.add_post("/api/konto/anmelden", login)
    app.router.add_get("/hub/pager", ws_handler)
    runner = web.AppRunner(app)
    await runner.setup()
    await web.TCPSite(runner, "0.0.0.0", PORT).start()
    print(f"Testserver laeuft auf Port {PORT} (Passwort: {PASSWORD})")
    await console()
    await runner.cleanup()


if __name__ == "__main__":
    asyncio.run(main())
