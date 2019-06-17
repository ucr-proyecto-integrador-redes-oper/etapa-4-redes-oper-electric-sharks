#ifndef REUDP
#define REUDP

#include <cstdint>
#include <cstdlib>
#include <map>
#include <string>
#include <utility>

#include <arpa/inet.h>

#include "socket.h"

struct data_frame{
	uint8_t type;
	uint16_t sn;
	char payload[1024];
};

struct ack_frame{
	uint8_t type;
	uint16_t rn;
};
class reUDP{
	private:
		uint16_t sn;
		Socket sock;
		std::map<std::pair<std::string, uint16_t>, struct frame *> sent;
	public:
		reUDP(uint16_t port) : sock(UDP, port), sn(rand() % UINT16_MAX);
		~reUDP();
		void Sendto(const char *, int, const char *, int);
		void Recvfrom(char *, int, struct sockaddr_in* = nullptr);
};

#endif
