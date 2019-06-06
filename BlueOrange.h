#ifndef BLUE_ORANGE_H
#define BLUE_ORANGE_H

#include "packet.h"

///Common structure for blue-orange packets.
struct BlueOrange : Packet
{
	
};

///Structure of an assignment packet, to assign graph nodes to blue nodes.
typedef struct Assignment : BlueOrange
{
	///Graph node.
	unsigned short int nodeID;
	///Number of neighbors of the node in the graph.
	unsigned short int totalNeighbors;
	///IP address of a neighbor.
	unsigned int ip;
	///Port number of a neighbor.
	unsigned short int port;
}Assignment;

///Structure of a connect packet, nothing special.
typedef struct Connect : BlueOrange
{
	
}Connect;

///Structure of a go packet, nothing special.
typedef struct Go : BlueOrange
{
	
}Go;

#endif
