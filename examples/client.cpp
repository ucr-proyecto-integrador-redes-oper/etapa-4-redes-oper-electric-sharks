#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#include "socket.h"

#define MSG_LEN 1024

using namespace std;

int main(int argc, char* argv[])
{
	if(argc < 3)
		return cout << "Usage: " << argv[0] << " ip_addr port" << endl, -1;
		
	char* ip_addr = argv[1];
	int port = atoi(argv[2]);
	
	cout << "Escriba mensajes" << endl;
	cout << "> ";
	char buffer[MSG_LEN];
	memset(buffer, '\0', MSG_LEN);
	
	Socket* sock = new Socket(Socket::Protocol::UDP);
	if(!socket){
		return cout << "No se pudo crear el socket! " << endl, -1;
	}
	
	string message;
	
	while(cin.getline(buffer, MSG_LEN, '\n')){
		cout << "> ";
		string message(buffer);
		sock->Sendto(message.data(), sizeof(string(message)), ip_addr, port);
	}
	
	return sock->Close();
}
