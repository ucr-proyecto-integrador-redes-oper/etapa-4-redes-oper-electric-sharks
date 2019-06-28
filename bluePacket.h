#ifndef BLUE_PACKET_H
#define BLUE_PACKET_H

#include "packet.h"

typedef char byte;

typedef struct __attribute__((__packed__)) chunkID{
	byte archID[3];
	uint32_t chunkNum;
} chunkID;

///Structure to represent a packet carriying a chunk of data between blue nodes.
typedef struct __attribute__((__packed__)) BluePacket : Packet
{
} BluePacket;

typedef struct __attribute__((__packed__)) BChunk : BluePacket
{
	chunkID cid;
	byte chunk[MAX_PAYLOAD_SIZE];
} BChunk;

///Hello packet.
typedef struct __attribute__((__packed__)) BHello : BluePacket
{
	uint16_t nodeID;
} BHello;

typedef struct __attribute__((__packed__)) BKill : BHello
{

} BKill;

typedef struct __attribute__((__packed__)) BJoin : BHello
{

} BJoin;

typedef struct __attribute__((__packed__)) BIdo : BHello
{

} BIdo;

typedef struct __attribute__((__packed__)) BDaddy : BHello
{

} BDaddy;

typedef struct __attribute__((__packed__)) BExist_R : BluePacket
{
	chunkID cid;
} BExist_R;

typedef struct __attribute__((__packed__)) BExist_A : BExist_R
{

} BExist_A;

typedef struct __attribute__((__packed__)) BComplete_R : BExist_R
{

} BComplete_R;

typedef struct __attribute__((__packed__)) BGet_R : BExist_R
{

} BGet_R;

typedef struct __attribute__((__packed__)) BGet_A : BExist_R
{

} BGet_A;

typedef struct __attribute__((__packed__)) BLocate_R : BExist_R
{

} BLocate_R;

typedef struct __attribute__((__packed__)) BComplete_A : BExist_R
{

} BComplete_A;

typedef struct __attribute__((__packed__)) BLocate_A : BluePacket
{
	uint16_t nodeID;
	byte archID[3];
} BLocate_A;

#endif
