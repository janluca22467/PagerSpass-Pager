#!/usr/bin/env python3
"""
Kleiner Testserver fuer den PagerSpass Pager.
Macht das gleiche wie der echte PagerSpass Server, nur ohne Spiel drumrum.

    pip install -r requirements.txt
    python server.py

Im Pager als Server dann http://<ip-vom-pc>:8080 eintragen.
"""
import asyncio
import json
import secrets
import sys
import time

from aiohttp import web

PORT = 8080
PASSWORD = "test"

tokens = {}
pagers = {}
round_state = {"state": "waiting", "name": ""}


async def link(request):
    data = await request.json()
    user = data.get("user", "")
    if not user or data.get("password") != PASSWORD:
        return web.json_response({"error": "Benutzername oder Passwort falsch"}, status=401)
    token = secrets.token_hex(16)
    tokens[token] = user
    print(f"[link] {data.get('device')} -> {user}")
    return web.json_response({"token": token, "name": user})


async def ws_handler(request):
    token = request.query.get("token", "")
    if token not in tokens:
        ws = web.WebSocketResponse()
        await ws.prepare(request)
        await ws.send_json({"t": "unlinked"})
        await ws.close()
        return ws

    ws = web.WebSocketResponse(heartbeat=30)
    await ws.prepare(request)
    device = request.query.get("device", "?")
    pagers[device] = ws
    print(f"[ws] {device} ({tokens[token]}) verbunden")
    await ws.send_json({"t": "round", **round_state})

    async for msg in ws:
        if msg.type != web.WSMsgType.TEXT:
            continue
        data = json.loads(msg.data)
        if data.get("t") == "ack":
            print(f"[ack] {device} hat {data.get('id')} quittiert")
        elif data.get("t") == "status":
            print(f"[status] {device} akku={data.get('bat')}% rssi={data.get('rssi')}")

    pagers.pop(device, None)
    print(f"[ws] {device} getrennt")
    return ws


async def broadcast(obj):
    for ws in list(pagers.values()):
        try:
            await ws.send_json(obj)
        except ConnectionResetError:
            pass


async def console():
    loop = asyncio.get_running_loop()
    print("Befehle: /start <name>, /ende, /prio <text>, /adr <1-4> <text>, /quit")
    print("Alles andere wird als Alarm geschickt.")
    while True:
        line = (await loop.run_in_executor(None, sys.stdin.readline)).strip()
        if not line:
            continue
        adr, prio = 1, 0
        if line == "/quit":
            break
        if line.startswith("/start"):
            round_state.update(state="active", name=line[6:].strip())
            await broadcast({"t": "round", **round_state})
            continue
        if line == "/ende":
            round_state.update(state="waiting", name="")
            await broadcast({"t": "round", **round_state})
            continue
        if line.startswith("/prio "):
            prio, line = 1, line[6:]
        elif line.startswith("/adr "):
            parts = line.split(" ", 2)
            adr, line = int(parts[1]), parts[2] if len(parts) > 2 else ""
        alarm = {"t": "alarm", "id": secrets.token_hex(4), "text": line.replace("\\n", "\n"),
                 "adr": adr, "prio": prio, "ts": int(time.time())}
        await broadcast(alarm)
        print(f"[alarm] an {len(pagers)} Pager gesendet")


async def main():
    app = web.Application()
    app.router.add_post("/api/pager/link", link)
    app.router.add_get("/api/pager/ws", ws_handler)
    runner = web.AppRunner(app)
    await runner.setup()
    await web.TCPSite(runner, "0.0.0.0", PORT).start()
    print(f"Testserver laeuft auf Port {PORT} (Passwort: {PASSWORD})")
    await console()
    await runner.cleanup()


if __name__ == "__main__":
    asyncio.run(main())
