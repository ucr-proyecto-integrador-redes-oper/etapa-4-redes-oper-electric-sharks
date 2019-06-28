#include <cstdlib>
#include <cstring>
#include <thread>
#include <cstdio>

#include "../secudp.h"

#define MY_PORT 2909
#define MSG_LEN 1024

using namespace std;

void sender(reUDP * sock){
	char buffer[MSG_LEN];
	memset(buffer, '\0', MSG_LEN);
	char ip[20];
	int myport;
	int c;
	while(true){
		printf("Escriba la IP del destinatario\n");
		scanf("%s", ip);
		printf("Escriba el puerto del destinatario\n");
		scanf("%d", &myport);

		while ( (c = getchar()) != EOF && c != '\n') { }
		printf("Escriba su mensaje\n");
		fgets(buffer, MSG_LEN, stdin);

		sock->Sendto(buffer, ip, myport, strlen(buffer));
	}
}

void receive(reUDP * sock){
	char buffer[MSG_LEN];
	while(true){
		sock->Recvfrom(buffer);
		printf("%s\n", buffer);
	}
}

int main(int argc, char* argv[])
{
	
	reUDP * sock = new reUDP(atoi(argv[1]));
	sock->run();
	std::thread first(sender, sock);
	std::thread second(receive, sock);

	first.join();
	second.join();

	return 0;
}

