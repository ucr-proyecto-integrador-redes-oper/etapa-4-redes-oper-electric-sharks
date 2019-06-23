#ifndef REUDP
#define REUDP

#include <cstdint>
#include <cstdlib>

#include <unordered_map>
#include <queue>

#include <arpa/inet.h>

#include "socket.h"
#include "semaphore.h"
//#include "Semaphore.h"

#define PAYLOAD_SIZE 1024
#define WAIT_TIME 50

struct data_frame{
	uint8_t type;
	uint16_t sn;
	char payload[PAYLOAD_SIZE];
};

struct direction{
	struct in_addr addr;
	uint16_t port;
};

struct map_entry{
	struct direction * direc;
	struct data_frame * frame;
	bool received;
};

class reUDP{
	private:
		uint16_t sn;
		Socket sock;
		std::unordered_map<uint16_t, struct map_entry *> sent; // Map that uses the sn as a key
		std::queue<struct data_frame *> processed_messages;
		Semaphore sem_recv;
		Semaphore sem_map;
		Semaphore sem_queue;
		void receiver();
		void sender();
	public:
		#ifdef SEMAPHORE_H
		reUDP(uint16_t port) : sock(UDP, port), sn(rand() % UINT16_MAX), sem_recv(KEY_CRIS, 0), sem_map(KEY_ASCH, 1), sem_queue(KEY_ROY, 1) {}
		#else
		reUDP(uint16_t port) : sock(UDP, port), sn(rand() % UINT16_MAX), sem_map(1), sem_queue(1) {}
		#endif
		~reUDP();
		void run();
		void Sendto(const char *, const char *, uint16_t);
		void Recvfrom(char *);
};

#endif
