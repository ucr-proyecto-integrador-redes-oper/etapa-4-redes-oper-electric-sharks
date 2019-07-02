#ifndef GREEN_NODE
#define GREEN_NODE
#define DEFAULT_PORT 2909
#define SIZE 1024

#include "bluePacket.h"
#include <cstdint>

class GreenNode{
	private:
		reUDP socket;
	public:
		GreenNode(uint16_t = DEFAULT_PORT);
		void send(const char *);
		void assembleFile(BChunk *);
		void receive();
};

#endif
