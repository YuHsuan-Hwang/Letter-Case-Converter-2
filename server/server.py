#!/usr/bin/env python

import asyncio
import websockets


async def PrintText(websocket, path):

	# keep receiving message from the client
	async for message in websocket:

		# wait for 10 sec and send back the converted message
		await asyncio.sleep(10)
		await websocket.send(message.swapcase())

	
# create a event loop
loop = asyncio.get_event_loop()

# setup a task
start_server = websockets.serve(PrintText, "localhost", 5675)

# run the task
loop.run_until_complete(start_server)
loop.run_forever()
