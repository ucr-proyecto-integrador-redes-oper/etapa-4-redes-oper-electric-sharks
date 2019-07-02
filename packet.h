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
#define NODE_BLUE 300
#define NODE_ORANGE 400
#define NODE_GREEN 500
#define PACKET_ORANGE 6
#define PACKET_BLUE 7
#define PACKET_GREEN 8
#define MAX_PAYLOAD_SIZE 1024

/// Struct with enumerations that identify the packets by their ID field.
typedef struct ID
{
	enum ORANGE
	{
		INITIAL_TOKEN,
		TOKEN_FULL_AND_REQUEST,
		TOKEN_FULL_AND_COMPLETE,
		TOKEN_EMPTY
	};
	
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
		BDELETE,
		BJOIN_TREE,
		BIDO,
		BIDONOT = 18,
		BDADDY = 13
	};
	
	enum BLUE_ORANGE
	{
		BOJOIN_GRAPH = 14,
		BOGRAPH_POSITION_E,
		BOGRAPH_POSITION_N,
		BOGRAPH_COMPLETE
	};
	
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
	bool receivedFromLeft;
	int sendTo;
	int typeNode;
	unsigned int senderIP;
	unsigned short int senderPort;
	unsigned int sendToIP;
	unsigned short int sendToPort;
} PacketEntry;

typedef struct BlueRequest
{
	unsigned int blueIP;
	unsigned short bluePort;
} BlueRequest;

#endif // PACKET_H
