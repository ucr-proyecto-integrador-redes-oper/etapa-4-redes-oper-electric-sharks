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
		BCHUNK = 20,
		BHELLO,
		BEXIST,
		BCOMPLETE,
		BGET,
		BLOCALIZE,
		BDELETE
	};
	
	enum BLUE_ORANGE
	{
		ASSIGNMENT = 40,
		CONNECT,
		GO
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
	int typeNode;
} PacketEntry;

#endif // PACKET_H
