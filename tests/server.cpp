#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#include "../secudp.h"

#define MSG_LEN 1024
#define MY_PORT 2909

using namespace std;

int main(int argc, char* argv[])
{

	char message[MSG_LEN];
	
	//zero out the message buffer
	memset(message, '\0', MSG_LEN);
	
	reUDP* sock = new reUDP(MY_PORT);
	sock->run();

	int run = 0;
	while(true){
		sock->Recvfrom(message);
		std::cout << run++ << ": " << message << std::endl;
		memset(message, '\0', MSG_LEN);
	}

	return 0;
}
