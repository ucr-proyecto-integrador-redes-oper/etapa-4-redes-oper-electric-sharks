#ifndef BLUE_ORANGE_H
#define BLUE_ORANGE_H

#include "packet.h"

struct BlueOrange : Packet
{
	
};

typedef struct Assignment : BlueOrange
{
	char nodeID;
	unsigned short int totalNeighbors;
	unsigned int ip;
	unsigned short int port;
}Assignment;

typedef struct Connect : BlueOrange
{
	
}Connect;

typedef struct Go : BlueOrange
{
	
}Go;

#endif
