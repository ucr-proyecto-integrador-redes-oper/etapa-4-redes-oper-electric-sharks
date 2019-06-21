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

///Function that takes an ipv4 address in binary notation and returns it in dots notation.
/*char* decode_ip(unsigned int ip, char* str)
{
    unsigned char bytes[4];
    bytes[0] = ip & 0xFF;
    bytes[1] = (ip >> 8) & 0xFF;
    bytes[2] = (ip >> 16) & 0xFF;
    bytes[3] = (ip >> 24) & 0xFF;   
    sprintf(str, "%d.%d.%d.%d", bytes[0], bytes[1], bytes[2], bytes[3]);        
    return str;
}*/

/*unsigned int encode_ip(char* ip)
{
	struct in_addr ip_addr;
	inet_aton(ip, &ip_addr);
	return (int) ip_addr.s_addr;
}*/

///Base struct Packet. All packets share the id field.
struct Packet
{
	///Packet identifier.
	unsigned short int id;
};

#endif // PACKET_H
