#include <ctime>
#include <iostream>
#include <cstring>
#include <ctype.h>

#include <uWS/uWS.h>

#include <thread>
#include <chrono>

using namespace std;

int main( int argc, char *argv[] )
{

	// construct a hub
	uWS::Hub h;

	// task when receive a message from the client
	h.onMessage( []( uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode ) {

		time_t rawtime;

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

	} );

	// listen to connection from the port
	if ( h.listen(5675) ) {
		h.run();
	}

}
