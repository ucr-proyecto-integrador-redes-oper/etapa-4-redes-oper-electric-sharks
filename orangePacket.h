#ifndef ORANGE_PACKET_H
#define ORANGE_PACKET_H

#include "packet.h"

///Abstract class to define a common packet between orange nodes.
struct OrangePacket : Packet
{
		unsigned short int id;
		///Orange token always carries the source node ip.
		unsigned int ip;
		///Accepted/rejected.
		bool boolean;
};

///Concrete classs to define the token, has nothing more than its abstract base class.
typedef struct InitialToken : OrangePacket
{
		///Allows this class to be concrete.
}InitialToken;

typedef struct Token : OrangePacket
{
	
		///Field to represent a graph node.
		unsigned short int node;
		///IP address of the blue node to be assigned.
		unsigned assignedIp;
		///Port number of the blue node to be assigned.
		unsigned short int assignedPort;
}Token;

#endif
