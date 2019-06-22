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
	
	while(fs){
		fs.getline(message, MSG_LEN);
		sock->Sendto(message, argv[1], port);
	}
	while(true){
		std::this_thread::sleep_for(std::chrono::seconds(100));
	}
	return 0;
}
