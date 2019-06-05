#ifndef BLUE_PACKET_H
#define BLUE_PACKET_H

#include "packet.h"

struct BluePacket : Packet
{
	///Name of the assigned node.
	unsigned short name;
};

typedef struct Chunk : BluePacket
{
	unsigned int totalChunks;
	unsigned int chunkNumber;
	char data[CHUNK_SIZE];
	unsigned short int hops;
}Chunk;

typedef struct Hello : BluePacket
{
	unsigned short int node;
}Hello;

typedef struct Exist : BluePacket
{
	bool question;
	bool exist;
}Exist;

typedef struct Complete : BluePacket
{
	unsigned int totalChunks;
	bool question;
	unsigned int chunkNumber;
}Complete;

typedef struct Get : Packet
{
	unsigned int totalChunks;
	bool question;
	unsigned int chunkNumber;
	char chunk[CHUNK_SIZE];
}Get;

typedef struct Localize : BluePacket
{
	bool question;
	unsigned int ip;
	unsigned short int port;
	unsigned int chunkNumber;
	
}Localize;

typedef struct Delete : BluePacket
{
	
}Delete;

#endif
