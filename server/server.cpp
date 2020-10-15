#include <ctime>
#include <iostream>
#include <cstring>
#include <ctype.h>

#include <uWS/uWS.h>

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
void OnMessage( uWS::WebSocket<uWS::SERVER>* ws, char* message, size_t length, uWS::OpCode opCode ) {

	// receive message
	time (&rawtime);
	cout << "( " << strtok(ctime(&rawtime), "\n") << " ) received message: ";
	for( int i=0; i<length; i++ ) cout << message[i];
	cout << endl;

	// convert lettercase
	for( int i=0; i<length; i++ ) { 
		char c = message[i];
		if      ( isupper(c) ) c = tolower(c);
		else if ( islower(c) ) c = toupper(c);
		else cout << "unable to identify lettercase" << endl;
		message[i] = c;
	}   

	// 10 sec time delay
	this_thread::sleep_for( chrono::seconds(10) );

	// send converted message
	time (&rawtime);
	cout << "( " << strtok(ctime(&rawtime), "\n") << " ) send message: ";
	for( int i=0; i<length; i++ ) cout << message[i];
	cout << endl;
	ws->send( message, length, opCode );

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
