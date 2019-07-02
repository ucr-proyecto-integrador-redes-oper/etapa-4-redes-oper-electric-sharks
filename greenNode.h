#ifndef GREEN_NODE
#define GREEN_NODE
#define DEFAULT_PORT 2909
#define FIRST_GREEN 0
#define SIZE 1024

#include "bluePacket.h"
#include <cstdint>

struct myFileID{
	uint8_t prefix;
	uint16_t localID;
};


class GreenNode{
	private:
		reUDP socket;
		struct myFileID nextFileID;
	public:
		GreenNode(uint16_t = DEFAULT_PORT, uint8_t = 0);
		void sendFile(const char *);
		void assembleFile(BChunk *);
		void receive();
};

#endif
