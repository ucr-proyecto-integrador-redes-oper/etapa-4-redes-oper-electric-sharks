#ifndef REUDP
#define REUDP

#include <cstdint>
#include <cstdlib>
#include <ctime>

#include <unordered_map>
#include <queue>

#include <arpa/inet.h>
#include <sys/types.h>

#include "socket.h"
#include "semaphore.h"
#include "packets.h"

#define PAYLOAD_SIZE sizeof(sendChunkPckt)
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
		std::queue<struct data_frame *> processed_messages;
		Semaphore sem_recv;
		Semaphore sem_map;
		Semaphore sem_queue;
		void receiver();
		void sender();
	public:
		reUDP(uint16_t port) : sock(UDP, port), sem_map(1), sem_queue(1) {
			srand(time(NULL));
			sn = rand() % UINT16_MAX;
		}
		~reUDP();
		void run();
		void Sendto(const char *, const char *, uint16_t, size_t = PAYLOAD_SIZE);
		void Recvfrom(char *);
		void printPacket(const struct data_frame *, size_t);
};

#endif
