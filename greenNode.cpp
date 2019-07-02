#include "packet.h"
#include "bluePacket.h"
#include "secudp.h"
#include "greenNode.h"
#include "error_handler.h"
#include "semaphore.h"

#include <cstring>
#include <cstdio>
#include <cstdint>

GreenNode::GreenNode(uint16_t port, uint8_t greenID) : socket(port){
	nextFileID.prefix = 000000101;
	nextFileID.prefix << 5;
	nextFileID.prefix ||= greenID;
	nextFileID.localID = 1;
	socket.run();
}

void GreenNode::sendFile(const char * filename){
	FILE * source = fopen(filename, "rb");
	if(source == NULL){
		error_exit(errno, "Error opening file\n");
	}
	
	BChunk * chunk = new BChunk();
	chunk->id = ID::BCHUNK;
	for(int i = 0; i < 3; ++i){ // TEMPORAL
		chunk->archID[i] = 'a';
	}
	chunk->chunkNum = 0;

	int read_result;
	do{
		for(int i = 0; i < 3; ++i){
			//chunk->archID[i] = ((uint8_t *) nextFileID)[i];
		}
		nextFileID.localID += 1;
		read_result = fread((void *) chunk->chunk, 1, SIZE, source);
		if(read_result > 0){
			socket.Sendto((const char *) chunk, "127.0.0.1", 9999, sizeof(BChunk));
			chunk->chunkNum += 1;
		}
		memset((void *) chunk->chunk, 0, SIZE);
	} while(read_result);
	printf("still going on\n");
	fclose(source);
	delete chunk;
}

void GreenNode::assembleFile(BChunk * chunk){
	//Agregar codigo para que si no existe, se cree el archivo primero
	source = fopen(chunk->archID, "r+b");
	fseek(source, chunk->chunkNum * SIZE, SEEK_SET);
	fwrite((const void *) chunk->chunk, SIZE, 1, source);
	fclose(source);
	printf("still going on\n");
}

void GreenNode::receive(){
	FILE * source;
	char recvPacket[sizeof(BChunk)];
	source = fopen("aa", "w");
	fclose(source);
	while(true){
		socket.Recvfrom((char *) recvPacket);
		#ifdef DEBUG
			printf("Se recibio un paquete con ID: %c\n", ((Packet *) recvPacket)->id);
		#endif
		switch(((Packet *) recvPacket)->id){
			case 3:
				printf("yep, %s exists\n", ((BExists_A *) recvPacket)->archID);
				break;
			case 5:
				//printf("yep, complete\n");
				//El nodo verde deberia de guardar cuantos chunks envio para ver cuantos recibe
				break;
			case 7:
				assembleFile((BChunk *) recvPacket);
				break;
			case 9:
				printf("%s is in %hhd\n", ((BLocate_A *) recvPacket)->archID, ((BLocate_A *) recvPacket)->nodeID);
				break;
			#ifdef DEBUG
			default:
				printf("I don't care about this types of packages\n");
				break;
			#endif 
		}
	}
}
