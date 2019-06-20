#ifndef REUDP
#define REUDP

#include <cstdint>
#include <cstdlib>

#include <unordered_map>
#include <queue>

#include <arpa/inet.h>

#include "socket.h"

#define PAYLOAD_SIZE 1024
#define WAIT_TIME 5

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

// Necesito un hilo que este escuchando mensajes de entrada y que envie un ack o ponga el mensaje en una cola
// Necesito un hilo que este manejando el timeout y que cada cierto tiempo envie los mensajes en el mapa

class reUDP{
	private:
		uint16_t sn;
		Socket sock;
		std::unordered_map<uint16_t, struct map_entry *> sent; // Map that uses the IP and port as a key
		std::queue<struct data_frame> inbox;
		std::queue<char *> processed_messages;
		void receiver();
		void sender();
	public:
		reUDP(uint16_t port) : sock(UDP, port), sn(rand() % UINT16_MAX) {}
		void run();
		void Sendto(const char *, const char *, uint16_t);
		void Recvfrom(char *);
};

#endif
