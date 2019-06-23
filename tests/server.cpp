#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#include "../secudp.h"

#define MSG_LEN 1024
#define MY_PORT 2909

using namespace std;

struct package{
	unsigned int ln;
	char message[MSG_LEN];
};

int main(int argc, char* argv[])
{

	char message[MSG_LEN];
	
	//zero out the message buffer
	memset(message, '\0', MSG_LEN);
	
	reUDP* sock = new reUDP(MY_PORT);
	sock->run();

	struct package mypkg;
	while(true){
		sock->Recvfrom((char *) &mypkg);
		std::cout << mypkg.ln << ": " << mypkg.message << std::endl;
		memset(mypkg.message, '\0', MSG_LEN);
	}

	return 0;
}
