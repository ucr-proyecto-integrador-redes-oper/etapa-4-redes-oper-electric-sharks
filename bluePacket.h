#ifndef BLUE_PACKET_H
#define BLUE_PACKET_H

#include "packet.h"

///Common structure of packets in blue-blue communications.
struct BluePacket : Packet
{
	///Name of the file, when applies.
	char name[NAME_SIZE];
};

///Structure to represent a packet carriying a chunk of data between blue nodes.
typedef struct BChunk : BluePacket
{
	///Total number of chunks belonging to a file.
	unsigned int totalChunks;
	///Number of this chunk.
	unsigned int chunkNumber;
	///The chunk of data itself.
	char data[CHUNK_SIZE];
	/**Number of hops allowed to this packet. If it reaches 0, it is either saved on that
	blue node's disk or discarded, but it can't be forwarded anymore.*/
	unsigned short int hops;
}BChunk;

///Hello packet.
typedef struct BHello : BluePacket
{
	unsigned short int node;
}BHello;

///Exist packet. Could be either a question or an answer.
typedef struct BExist : BluePacket
{
	///True if the sending node is asking, false if it's answering.
	bool question;
	///True if a chunk of data of the file exists, false otherwise.
	bool exist;
}BExist;

///Complete packet. Could be either a question or an answer.
typedef struct BComplete : BluePacket
{
	///Total number of chunks that make up a file.
	unsigned int totalChunks;
	///True if the sending node is asking, false if it's answering.
	bool question;
	///Number of the chunk confirmed to exist with this packet.
	unsigned int chunkNumber;
}BComplete;

///Get packet. Could be either a question or an answer.
typedef struct BGet : Packet
{
	///Total number of chunks that make up a file.
	unsigned int totalChunks;
	/**If true, triggers a get action of that file's packets among all its neighbors.
	If false, is just a green node asking to get a file to a blue node.*/
	bool question;
	///Number of this chunk.
	unsigned int chunkNumber;
	///The chunk of data itself.
	char chunk[CHUNK_SIZE];
}BGet;

///Localize packet. Could be either a question or an answer.
typedef struct BLocalize : BluePacket
{
	///True if a green node is asking to localize a packet, false if it's the answer.
	bool question;
	///IP of the node that has a chunk of the asked file.
	unsigned int ip;
	///Port number of the node that has a chunk of the asked file.
	unsigned short int port;
	///Number of the chunk localized in that node.
	unsigned int chunkNumber;
	
}BLocalize;

/**Delete packet. It doesn't need more than the file name to delete all the chunks
associated with it.*/
typedef struct BDelete : BluePacket
{
	
}BDelete;

#endif
