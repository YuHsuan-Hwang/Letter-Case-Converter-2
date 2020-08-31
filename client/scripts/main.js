'use strict';

// define the enter button
let buttonEnter = document.getElementById("button_enter");

// event of clicking the enter button
buttonEnter.onclick = function(){

	// setup connection with the server
	let ws = new WebSocket("ws://localhost:5675/");

	// receive message from the server and print out
	ws.onmessage = function (event) {
		document.getElementById('output_text').value = event.data;
	};

};

