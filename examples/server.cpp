#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#include "socket.h"

#define MSG_LEN 1024

using namespace std;

int main(int argc, char* argv[])
{

	char message[MSG_LEN];
	
	//zero out the message buffer
	memset(message, '\0', MSG_LEN);
	
	Socket* sock = new Socket(Socket::Protocol::UDP);
	
	//Struct to save sender data
	struct sockaddr_in receiver_addr;
	
	if(!socket){
		return cout << "No se pudo crear el socket! " << endl, -1;
	}
	int read = 0;
	while(true){
		read = sock->Recvfrom(message, MSG_LEN, &receiver_addr);
		cout << "Received from " << inet_ntoa(receiver_addr.sin_addr) << " : " << receiver_addr.sin_port << " = " << string(message) << endl;
		memset(message, '\0', MSG_LEN);
	}
	return sock->Close();
}
