#!/usr/bin/env python

import asyncio
import websockets

async def PrintText(websocket, path):
	await websocket.send("hello from the server")

# create a event loop
loop = asyncio.get_event_loop()

# setup a task
start_server = websockets.serve(PrintText, "localhost", 5675)

# run the task
loop.run_until_complete(start_server)
loop.run_forever()
