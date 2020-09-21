#!/usr/bin/env python

import asyncio
import websockets
from datetime import datetime
from protobufs.letters_pb2 import Letters
import string

client_num = 0 # number of clients connected to the server

# construct the main task
async def PrintText(websocket, path):

	# show the number of clients when new client is connected
	global client_num
	client_num += 1
	print("(", datetime.now(), ") established one connection to ", websocket.remote_address[0],",", client_num, "client connected")
	print()
		
	try:

		# keep receiving message from the client
		async for message in websocket:

			# receive the message
			print("(", datetime.now(), ") received message: ",message)
	
			# decode the message
			return_message = Letters()
			return_message.ParseFromString(message)
			print ("(", datetime.now(), ") decode message: ",return_message.input_letters,return_message.is_english)
		
			# check if inputs are alphabets
			# ignore spaces and punctuations when checking
			if (return_message.input_letters.replace(" ","").translate(str.maketrans('', '', string.punctuation)).isalpha()==True):
				# true: convert the message
				return_message.is_english = 1
				return_message.input_letters = return_message.input_letters.swapcase()
			else:
				# flase: enable the alert
				return_message.is_english = 0
			print ("(", datetime.now(), ") converted message: ",return_message.input_letters,return_message.is_english)	

			# encode the message
			return_message_bytes = return_message.SerializeToString()
			print ("(", datetime.now(), ") encoded converted message: ",return_message_bytes)
			print()

			# if inputs are alphabets, wait for 10 sec
			# if not, just send back and trigger the alert
			if (return_message.is_english == 1):
				await asyncio.sleep(10)

			# send back the message
			await websocket.send(return_message_bytes)
			print ("(", datetime.now(), ") send back message: ",return_message_bytes)
			print()

	# listen to connection
  # show the number of clients when a client is disconnected	
	except websockets.exceptions.ConnectionClosed:
		client_num -= 1
		print("(", datetime.now(), ") lost connection from ",websocket.remote_address[0],",", client_num, "client connected")
		print()
		
# create a event loop
loop = asyncio.get_event_loop()

# setup a task that connects to the server
start_server = websockets.serve(PrintText, "localhost", 5675)

# run the task
try:
	loop.run_until_complete(start_server)
	loop.run_forever()
# listen for ctl c to terminate the program
except KeyboardInterrupt:
	loop.stop()
	print("\n(", datetime.now(), ") server terminated")
