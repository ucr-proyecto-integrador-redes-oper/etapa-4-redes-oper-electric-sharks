#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "../secudp.h"

#define MY_PORT 2909
#define MSG_LEN 1024

using namespace std;

struct package{
	unsigned int ln;
	char message[MSG_LEN];
};

int main(int argc, char* argv[])
{
	if(argc < 3)
		return cout << "Usage: " << argv[0] << " ip_addr port" << endl, -1;
		
	int port = atoi(argv[2]);
	
	char buffer[MSG_LEN];
	memset(buffer, '\0', MSG_LEN);
	
	reUDP * sock = new reUDP(5909);
	sock->run();
	
	char message[MSG_LEN];

	std::ifstream fs(argv[3]);
	struct package mypkg;
	mypkg.ln = 0;
	while(fs){
		fs.getline(mypkg.message, MSG_LEN);
		sock->Sendto((const char *) &mypkg, argv[1], port);
		mypkg.ln += 1;
	}
	while(true){
		std::this_thread::sleep_for(std::chrono::seconds(100));
	}
	return 0;
}
