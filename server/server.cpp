#include <ctime>
#include <iostream>
#include <vector>
#include <cstring>
#include <cctype>
#include <csignal>

#include <uWS/uWS.h>
#include "protobufs/letters.pb.h"

#include <thread>
#include <chrono>
#include <mutex>

using namespace std;

int client_num = 0;  // num of clients
time_t rawtime;
vector<thread> threads;  // a vector of threads
mutex mu;  // for turning off multi-threading

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

// main task: converting the lettercase
void LetterCaseConverter( uWS::WebSocket<uWS::SERVER>* ws, char* raw_message, size_t length, uWS::OpCode opCode ) {

	time (&rawtime);
	cout << "( " << strtok(ctime(&rawtime), "\n") << " ) one thread created, " << threads.size() << " thread working" << endl;

	// pick the input message from the whole raw message
	char message[length];
	strncpy( message, raw_message, length );

	time (&rawtime);
	cout << "( " << strtok(ctime(&rawtime), "\n") << " ) received message: ";
	for ( int i=0; i<length; i++ ) printf( "%d ", message[i] );
	cout << endl;

	// decode the message
	Letters return_message;
	return_message.ParseFromString(message);
	time (&rawtime);
	cout << "( " << strtok(ctime(&rawtime), "\n") << " ) decode message: " << return_message.ShortDebugString() << endl;

	// convert lettercase
	string input_letters = return_message.input_letters();
	for( int i=0; i<input_letters.length(); i++ ) { 
		if      ( isupper(input_letters[i]) ) input_letters[i] = tolower(input_letters[i]);
		else if ( islower(input_letters[i]) ) input_letters[i] = toupper(input_letters[i]);
		else if ( !ispunct(input_letters[i]) & !isspace(input_letters[i]) ){
			time (&rawtime);
			cout << "( " << strtok(ctime(&rawtime), "\n") << " ) unable to identify lettercase" << endl;
			return_message.set_is_english( 0 );
			break;
		}
	}
	if ( return_message.is_english()==-1 ) return_message.set_is_english( 1 );
	return_message.set_input_letters( input_letters );
	time (&rawtime);
	cout << "( " << strtok(ctime(&rawtime), "\n") << " ) converted message: " << return_message.ShortDebugString() << endl;

	// encode the message to string
	string return_message_bytes;
	return_message.SerializeToString( &return_message_bytes );

	// change string to char form
	char *return_message_bytes_cstr;
	return_message_bytes_cstr = new char[return_message_bytes.size()+1];
	strcpy(return_message_bytes_cstr, return_message_bytes.c_str());

	// 10 sec time delay
	if ( return_message.is_english()==1 ) this_thread::sleep_for( chrono::seconds(10) );

	// send converted message
	ws->send( return_message_bytes_cstr, opCode );

	time (&rawtime);
	cout << "( " << strtok(ctime(&rawtime), "\n") << " ) send message: ";
	for ( int i=0; i<return_message_bytes.size()+1; i++ ) printf( "%d ", return_message_bytes_cstr[i] );
	cout << endl;

	// remove the thread
	mu.lock(); // turn off multi-threading to avoid race condition in modifying threads
	for ( int i=0; i<threads.size(); i++ ){
		if ( threads[i].get_id() == this_thread::get_id() ){ // search for the right thread in the vector
			threads[i].detach(); // release the thread
			threads.erase( threads.begin()+i ); // delete the thread
			break;
		}
	}
	mu.unlock();
	time (&rawtime);
	cout << "( " << strtok(ctime(&rawtime), "\n") << " ) one thread removed, " << threads.size() << " thread working" << endl;

}

// receive message from a client
void OnMessage( uWS::WebSocket<uWS::SERVER>* ws, char* raw_message, size_t length, uWS::OpCode opCode ){

	// create a thread and process the main task
	threads.push_back( thread( LetterCaseConverter, ws, raw_message, length, opCode ) );

}

// received ctrl c event
void SignalHandler( int sig ){

	// wait the threads to terminate
	//time (&rawtime);
	//cout << "( " << strtok(ctime(&rawtime), "\n") << " ) waiting all the threads to terminate" << endl;	
	//for ( int i=0; i<threads.size(); i++ ){
	//	threads[i].join();
	//}

	// exit the program
	time (&rawtime);
	cout << "( " << strtok(ctime(&rawtime), "\n") << " ) exiting the server" << endl;
	exit(EXIT_SUCCESS);

}

int main( int argc, char *argv[] )
{

	time (&rawtime);
	cout << "( " << strtok(ctime(&rawtime), "\n") << " ) server started (press Ctrl-C to exit the server)" << endl;

	// listen to ctrl c event
	signal(SIGINT, SignalHandler);

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
