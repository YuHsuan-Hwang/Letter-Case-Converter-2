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

// construct event of clicking the enter button
function EnterClick(event){

  // read text in the input column
  let message = new proto.Letters();
  message.setInputLetters(inpInput.value);
  message.setIsEnglish(-1);
  console.log(new Date(),"input message: ",message.getInputLetters(),message.getIsEnglish());

	ws.send(inpInput.value);
	console.log(new Date(),"message sent");

	/*
	// encode the message
  let message_bytes = message.serializeBinary();
  console.log(new Date(),"serialized: ",message_bytes);

	// send the encoded message to the server  
	ws.send(message_bytes);
  console.log(new Date(),"message sent");
	*/

}

// response when clicking the enter button
btnEnter.onclick = EnterClick;

// also listen to enter key event
inpInput.addEventListener("keyup", function(event) {
	if (event.keyCode === 13) {
		EnterClick();
	}
} );

// set the type of receiving message
ws.binaryType = "arraybuffer";

// print out the result if receive message from the server
ws.onmessage = function (event) {

	/*
	// receive the message
	let return_message_bytes = new Uint8Array(event.data);
	console.log(new Date(),"received message: ",return_message_bytes);
	*/

	txtOutput.value = event.data;
	console.log(new Date(),event.data,"result displayed");

	/*
	// decode the message
	let return_message = proto.Letters.deserializeBinary(return_message_bytes);
	console.log(new Date(),"decode message: ",return_message.getInputLetters(),return_message.getIsEnglish());

	// if input is alphabet, show the message
	// if not, show alert
	if (return_message.getIsEnglish() === 1){
		txtOutput.value = return_message.getInputLetters();
		console.log(new Date(),"result displayed");
	}
	else if (return_message.getIsEnglish() === 0){
		alert("Alphabets only!");
	}
	else {
		alert("error");
	}
	*/
};

// update the status if the connection is closed
ws.onclose = function(event) {
  if (event.wasClean) {
    txtStatus.value = "connection closed cleanly";
  } else {
    txtStatus.value = "connection lost";
  }
};
