#ifndef GREEN_BLUE_H
#define GREEN_BLUE_H

#include "packet.h"

struct GreenBlue
{
	char name[NAME_SIZE];
};

typedef struct GChunk : GreenBlue
{
	unsigned int totalChunks;
	unsigned int chunkNumber;
	char data[CHUNK_SIZE];
}GChunk;

typedef struct GExist : GreenBlue
{
	bool exist;
}GExist;

typedef struct GComplete : GreenBlue
{
	bool complete;
}GComplete;

typedef struct GDelete : GreenBlue
{
	
}GDelete;

typedef struct GLocalize : GreenBlue
{
	bool question;
	unsigned int ip;
	unsigned short int port;
	unsigned int chunkNumber;
}GLocalize;

typedef struct GKill : GreenBlue
{
	unsigned short int node;
}GKill;

#endif
