#ifndef REUDP
#define REUDP

#include <cstdint>
#include <cstdlib>
#include <ctime>

#include <unordered_map>
#include <queue>
#include <utility>

#include <arpa/inet.h>
#include <sys/types.h>

#include "socket.h"
#include "semaphore.h"
#include "bluePacket.h"

#define PAYLOAD_SIZE sizeof(BChunk)
#define WAIT_TIME 50

struct __attribute__((__packed__)) data_frame{
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
	size_t len;
	bool received;
};

class reUDP{
	private:
		uint16_t sn;
		Socket sock;
		std::unordered_map<uint16_t, struct map_entry *> sent; // Map that uses the sn as a key
		std::queue<std::pair<struct data_frame *, struct sockaddr_in *>> processed_messages;
		Semaphore sem_recv;
		Semaphore sem_map;
		Semaphore sem_queue;
		void receiver();
		void sender();
	public:
		reUDP(uint16_t);
		~reUDP();
		void run();
		
		
		void Sendto(const char * message, const char * destination, uint16_t port, size_t len = PAYLOAD_SIZE);
		void Recvfrom(char *, struct sockaddr_in * = nullptr);
		void printPacket(const struct data_frame *, size_t);
};

#endif
