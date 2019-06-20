#ifndef PACKET_H
#define PACKET_H

typedef struct ID
{
	//ID space for orange packets: [0, 9]
	enum ORANGE
	{
		TOKEN_INITIAL,
		TOKEN_FULL_AND_REQUEST,
		TOKEN_FULL_AND_COMPLETE,
		TOKEN_EMPTY
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
char* make_ip(unsigned int ip, char* str)
{
    unsigned char bytes[4];
    bytes[0] = ip & 0xFF;
    bytes[1] = (ip >> 8) & 0xFF;
    bytes[2] = (ip >> 16) & 0xFF;
    bytes[3] = (ip >> 24) & 0xFF;   
    sprintf(str, "%d.%d.%d.%d", bytes[3], bytes[2], bytes[1], bytes[0]);        
    return str;
}

///Base struct Packet. All packets share the id field.
struct Packet
{
	///Packet identifier.
	unsigned short int id;
};

#endif // PACKET_H
