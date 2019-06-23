#ifndef PACKET_H
#define PACKET_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>

#define CHUNK_SIZE 10240 //10KB
#define NAME_SIZE 10

/// Struct with enumerations that identify the packets by their ID field.
typedef struct ID
{
	//ID space for orange packets: [0, 19]
	enum ORANGE
	{
		INITIAL_TOKEN,
		TOKEN
	};
	
	//ID space for blue packets: [20, 39]
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
	unsigned short int id;
};

#endif // PACKET_H
