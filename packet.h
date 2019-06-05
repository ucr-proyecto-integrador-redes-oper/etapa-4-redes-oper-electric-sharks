#ifndef PACKET_H
#define PACKET_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define CHUNK_SIZE 10240 //10KB

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

///Abstract class Packet. All packets share the id field.
struct Packet
{
		unsigned short int id;
};

#endif // PACKET_H
