'use strict';

// define the elements in the webpage
let txtStatus = document.getElementById("txt-status"); 
let inpInput  = document.getElementById("inp-input" );
let btnEnter  = document.getElementById("btn-enter" );
let txtOutput = document.getElementById("txt-output");

// setup connection with the server
let ws = new WebSocket("ws://localhost:5675/");

// update the status if the connection is opened
ws.onopen = function(event) {
  txtStatus.value = "connection established";
};

// event of clicking the enter button
btnEnter.onclick = function(){

	// read text in the input column and send to the server
	ws.send(inpInput.value);

};

// print out the result if receive message from the server
ws.onmessage = function (event) {
	txtOutput.value = event.data;
};

// update the status if the connection is closed
ws.onclose = function(event) {
  if (event.wasClean) {
    txtStatus.value = "connection closed cleanly";
  } else {
    txtStatus.value = "connection lost";
  }
};
