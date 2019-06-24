#ifndef PACKETS_H
#define PACKETS_H

#include <cstdint>

#define MAX_PAYLOAD_SIZE 1024
typedef char byte;

typedef enum packetID{
	CHUNK,
	HELLO,
	EXISTS_R,
	EXISTS_A,
	COMPLETE_R,
	COMPLETE_A,
	GET_R,
	GET_A,
	LOCATE_R,
	LOCATE_A,
	KILL,
	DELETE,
	JOIN_TREE,
	IDO,
	DADDY
} packetID;

typedef struct chunkID{
	byte archID[3];
	uint32_t chunkNum;
} chunkID;

typedef struct packet{
	packetID id;
} packet;

typedef struct sendChunkPckt : packet{
	chunkID cid;
	byte chunk[MAX_PAYLOAD_SIZE];
} sendChunkPckt;

typedef struct helloPacket : packet{
	uint16_t nodeID;
} helloPacket;

typedef helloPacket killPacket;
typedef helloPacket joinTree;
typedef helloPacket idoPacket;
typedef helloPacket daddyPacket;

typedef struct existsPacketR : packet{
	chunkID cid;
} existsPacketR;

typedef existsPacketR existsPacketA;
typedef existsPacketR completePacketR;
typedef existsPacketR getPacketR;
typedef existsPacketR locatePacketR;
typedef existsPacketR completePacketA;
typedef existsPacketR getPacketA;

typedef struct locatePacketA{
	uint16_t nodeID;
	byte archID[3];
} locatePacketA;


typedef struct deletePacket{
	byte archID[3];
} deletePacket;


#endif
