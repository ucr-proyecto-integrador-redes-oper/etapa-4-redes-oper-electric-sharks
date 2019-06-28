#ifndef PACKET_H
#define PACKET_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>

#define CHUNK_SIZE 10240 //10KB
#define NAME_SIZE 10
#define IP_LEN 16
#define SEND_TO_RIGHT 0
#define SEND_TO_LEFT 1
#define SEND_TO_BOTH 2
#define NODE_BLUE 3
#define NODE_ORANGE 4
#define NODE_GREEN 5

#define MAX_PAYLOAD_SIZE 1024

/// Struct with enumerations that identify the packets by their ID field.
typedef struct ID
{
	//ID space for orange packets: [0, 19]
	enum ORANGE
	{
		INITIAL_TOKEN,
		TOKEN_FULL_AND_REQUEST,
		TOKEN_FULL_AND_COMPLETE,
		TOKEN_EMPTY
	};
	
	//ID space for blue packets: [20, 39]
	enum BLUE
	{
		BCHUNK,
		BHELLO,
		BEXISTS_R,
		BEXISTS_A,
		BCOMPLETE_R,
		BCOMPLETE_A,
		BGET_R,
		BGET_A,
		BLOCATE_R,
		BLOCATE_A,
		BKILL,
		BDELETE,
		BJOIN_TREE,
		BIDO,
		BDADDY
	};
	
	//ID space for blue-orange packets: [40, 50]
	enum BLUE_ORANGE
	{
		ASSIGNMENT = 40,
		CONNECT,
		GO
	};
	
	//ID space for green-blue packets: [50, ...]
	enum GREEN_BLUE
	{
		GCHUNK = 50,
		GEXIST,
		GCOMPLETE,
		GDELETE,
		GLOCALIZE,
		GKILL
	};
} ID;



///Base struct Packet. All packets share the id field.
struct Packet
{
	///Packet identifier.
	unsigned char id;
};

typedef struct PacketEntry
{
	Packet* packet;
	int typeNode;
	int sendTo;
} PacketEntry;

#endif // PACKET_H
