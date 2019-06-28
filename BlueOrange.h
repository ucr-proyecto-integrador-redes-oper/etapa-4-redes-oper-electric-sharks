#ifndef BLUE_ORANGE_H
#define BLUE_ORANGE_H

#include "packet.h"
#include <cstdint>

typedef struct __attribute__((__packed__)) BlueOrange : Packet
{
	
} BlueOrange;

typedef struct __attribute__((__packed__)) BOJoinGraph : BlueOrange
{

} BOJoinGraph;

typedef struct __attribute__((__packed__)) BOGraphPosition_E : BlueOrange
{
	uint16_t nodeID;
	uint16_t neighborID;
} BOGraphPosition_E;

typedef struct __attribute__((__packed__)) BOGraphPosition_N : BOGraphPosition_E
{
	uint32_t neighborIP;
	uint16_t neighborPort;
	
} BOGraphPosition_N;

typedef struct __attribute__((__packed__)) BOGraphComplete : BlueOrange
{

} BOGraphComplete;
#endif
