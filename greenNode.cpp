#include "bluePacket.h"
#include "secudp.h"

#include <cstring>
#include <cstdio>


void GreenNode::send(const char * filename){
	/*
	FILE * source = fopen(full_path, "rb"); //Aquí tendremos el puntero a FILE del archivo
	if(source == NULL){
		error_exit(errno, "Error opening file\n"); //Si hubo algún error le avisamos al usuario y nos salimos del proceso
	}

	buf.mtype = getpid(); //Tomamos como ID el PID del proceso para no tener que hacer que el emisor nos asigne un ID.
	buf.piece_number = 0;
	buf.bytes = 0;
	strcpy(buf.mtext, filename);

	in_queue.send(&buf); //Enviamos el primer paquete con piece_number = 0 para que el receptor sepa que viene un nuevo archivo y cree un thread.

	int read_result;
	int full_groups;
	int remainder_bytes;
	char aux[SIZE];
	do{
		read_result = fread((void *) aux, 1, SIZE, source);
		full_groups = read_result/MSGSIZE;
		remainder_bytes = read_result%MSGSIZE;

		for(int i = 0; i < full_groups; ++i){
			memcpy((void *) buf.mtext, (void *) (aux + i*MSGSIZE), MSGSIZE);
			++buf.piece_number;
			buf.bytes = MSGSIZE;
			in_queue.send(&buf);
		}
		if(remainder_bytes > 0){
			memcpy((void *) buf.mtext, (void *) (aux+full_groups*MSGSIZE), remainder_bytes);
			++buf.piece_number;
			buf.bytes = remainder_bytes;
			in_queue.send(&buf);
		}

	} while(read_result);
	++buf.piece_number;
	buf.bytes = 0;
	in_queue.send(&buf); //Enviamos el último paquete con bytes = 0 (y mtype distinto de 0) para que el receptor sepa que ya se terminó el archivo

	fclose(source);
	*/
}
