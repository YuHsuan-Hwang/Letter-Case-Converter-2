#include <ctime>
#include <iostream>
#include <cstring>
#include <ctype.h>

#include <uWS/uWS.h>

#include "protobufs/letters.pb.h"

#include <thread>
#include <chrono>

using namespace std;

int client_num = 0;
time_t rawtime;

// connect to a client
void OnConnection( uWS::WebSocket<uWS::SERVER>* ws, uWS::HttpRequest http_request ) {

	client_num += 1;
	time (&rawtime);
	cout << "( " << strtok(ctime(&rawtime), "\n") << " ) established one connection to server, " << client_num << " client connected" << endl;

}

// disconnect to a client
void OnDisconnection( uWS::WebSocket<uWS::SERVER>* ws, int code, char* message, size_t length ) {

	client_num -= 1;
	time (&rawtime);
	cout << "( " << strtok(ctime(&rawtime), "\n") << " ) lost one connection to server, " << client_num << " client connected" << endl;

}

// receive message from a client
void OnMessage( uWS::WebSocket<uWS::SERVER>* ws, char* raw_message, size_t length, uWS::OpCode opCode ) {

	// receive message
	time (&rawtime);
	cout << "( " << strtok(ctime(&rawtime), "\n") << " ) receive message" << endl;

	// pick the input message from the whole raw message
	char message[length];
	strncpy( message, raw_message, length );

	// decode the message
	Letters return_message;
	return_message.ParseFromString(message);
	time (&rawtime);
	cout << "( " << strtok(ctime(&rawtime), "\n") << " ) decode message: " << return_message.input_letters() << ", " << return_message.is_english() << endl;



	// convert lettercase
	string input_letters = return_message.input_letters();
	for( int i=0; i<input_letters.length(); i++ ) { 
		if      ( isupper(input_letters[i]) ) input_letters[i] = tolower(input_letters[i]);
		else if ( islower(input_letters[i]) ) input_letters[i] = toupper(input_letters[i]);
		else if ( !ispunct(input_letters[i]) & !isspace(input_letters[i]) ){
			time (&rawtime);
			cout << "( " << strtok(ctime(&rawtime), "\n") << " ) unable to identify lettercase" << endl;
			return_message.set_is_english( 0 );
		}
	}
	if ( return_message.is_english()==-1 ) return_message.set_is_english( 1 );
	return_message.set_input_letters( input_letters );
	time (&rawtime);
	cout << "( " << strtok(ctime(&rawtime), "\n") << " ) converted message: " << return_message.input_letters() << ", " << return_message.is_english() << endl;



	// encode the message
	string return_message_bytes;
	return_message.SerializeToString( &return_message_bytes );

	// 10 sec time delay
	if ( return_message.is_english()==1 ) this_thread::sleep_for( chrono::seconds(10) );

	// send converted message
	time (&rawtime);
	cout << "( " << strtok(ctime(&rawtime), "\n") << " ) send message" << endl;
	char *return_message_bytes_cstr;
	return_message_bytes_cstr = new char[return_message_bytes.size()+1];
	strcpy(return_message_bytes_cstr, return_message_bytes.c_str());
	ws->send( return_message_bytes_cstr, opCode );
}


int main( int argc, char *argv[] )
{

	time (&rawtime);
	cout << "( " << strtok(ctime(&rawtime), "\n") << " ) server started (press Ctrl-C to exit the server)" << endl;
	// construct a hub
	uWS::Hub websocket_hub;

	// actions in the hub
	websocket_hub.onMessage(&OnMessage);
	websocket_hub.onConnection(&OnConnection);
	websocket_hub.onDisconnection(&OnDisconnection);

	// listen to connection from the port
	if ( websocket_hub.listen(5675) ) {
		websocket_hub.run();
	}

}
