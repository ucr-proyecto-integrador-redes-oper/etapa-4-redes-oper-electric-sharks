#include "packet.h"
#include "bluePacket.h"
#include "secudp.h"
#include "greenNode.h"
#include "error_handler.h"
#include "semaphore.h"

#include <cstring>
#include <cstdio>
#include <cstdint>

GreenNode::GreenNode(uint16_t port) : socket(port){
	socket.run();
}

void GreenNode::send(const char * filename){
	FILE * source = fopen(filename, "rb");
	if(source == NULL){
		error_exit(errno, "Error opening file\n");
	}
	
	BChunk * chunk = new BChunk();
	chunk->id = ID::BCHUNK;
	for(int i = 0; i < 3; ++i){ // TEMPORAL
		chunk->archID[i] = 'a';
	}
	chunk->archID[2] = '\0';
	chunk->chunkNum = 0;

	int read_result;
	int full_groups;
	int remainder_bytes;
	char aux[SIZE];
	do{
		read_result = fread((void *) chunk->chunk, 1, SIZE, source);
		if(read_result > 0){
			socket.Sendto((const char *) chunk, "127.0.0.1", 9999, sizeof(BChunk));
			printf("%d\n", chunk->chunkNum);
			chunk->chunkNum += 1;
		}
	} while(read_result);
	fclose(source);
	delete chunk;
}

void GreenNode::receive(){
	FILE * source;
	long expected = 0;
	BChunk * chunk = new BChunk();
	while(true){
		socket.Recvfrom((char *) chunk);
		source = fopen(chunk->archID, "w+b");
		printf("%d\n", chunk->chunkNum);
		fseek(source, chunk->chunkNum * SIZE, SEEK_SET);
		fwrite((const void *) chunk->chunk, SIZE, 1, source);
		fclose(source);
	}
}

int main(int argc, char * argv[]){
	Semaphore sem;
	if(argc > 1){
		GreenNode gn(2909);
		gn.send(argv[1]);
		sem.wait();
	} else {
		GreenNode gn(9999);
		gn.receive();
	}
	return 0;
}
