#include "secudp.h"
#include "socket.h"
#include "semaphore.h"
#include "bluePacket.h"

#include <unordered_map>
#include <utility>
#include <queue>
#include <thread>
#include <chrono>

#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <arpa/inet.h>
#include <sys/types.h>

#include <iostream>

reUDP::reUDP(uint16_t port) : sock(UDP, port), sem_map(1), sem_queue(1) {
	srand(time(NULL));
	sn = rand() % UINT16_MAX;
	sock.Bind(port);
}

reUDP::~reUDP(){
	sock.Close();
}

void reUDP::run(){
	std::thread(&reUDP::receiver, this).detach();
	std::thread(&reUDP::sender, this).detach();
}

void reUDP::Sendto(const char * message, const char * destination, uint16_t port, size_t len){
	struct data_frame * frame = new struct data_frame();
	frame->type = 0;
	frame->sn = sn;
	memcpy((void *) frame->payload, (const void *) message, len);
	#ifdef SECDEBUG
		printPacket(frame, len);
	#endif

	struct direction * direc = new struct direction();
	inet_aton(destination, &direc->addr);
	direc->port = port;

	struct map_entry * entry = new struct map_entry();
	entry->direc = direc;
	entry->frame = frame;
	entry->len = len;
	entry->received = false;

	sent[sn] = entry;
	sn += 1;
}

void reUDP::Recvfrom(char * message, struct sockaddr_in * addr){
	sem_recv.wait();
	std::pair<struct data_frame *, struct sockaddr_in *> myPair;
	if(processed_messages.size() > 0){
		sem_queue.wait();
		myPair = processed_messages.front();
		//struct data_frame * received_msg = processed_messages.front();
		processed_messages.pop();
		sem_queue.signal();
		memcpy((void *) message, (const void *) myPair.first->payload, PAYLOAD_SIZE);
		if(addr){
			memcpy((void *) addr, (const void *) myPair.second, sizeof(struct sockaddr_in));
		}
		delete myPair.first;
		delete myPair.second;
	} else { 
		std::cerr << "somehow empty" << std::endl;
	}
}

void reUDP::receiver(){
	struct sockaddr_in * return_addr;
	struct data_frame * receiver = new struct data_frame();
	while(true){
		return_addr = new struct sockaddr_in();
		receiver = new struct data_frame();
		sock.Recvfrom((char *) receiver, sizeof(struct data_frame), return_addr);
		#ifdef SECDEBUG
			printPacket(receiver, sizeof(struct data_frame));
		#endif
		if(!receiver){
			std::cerr << "Invalid pointer at receiver" << std::endl;
		}
		if(receiver->type == 0){
			receiver->type = 1;
			#ifdef SECDEBUG
				printf("Sending ACK with sn: %d to %s::%d\n", receiver->sn, inet_ntoa(return_addr->sin_addr), return_addr->sin_port);
			#endif
			sock.Sendto((const char *) receiver, 3, return_addr);
			sem_queue.wait();
			processed_messages.push(std::pair<struct data_frame *, struct sockaddr_in *>(receiver, return_addr));
			sem_queue.signal();
			sem_recv.signal();
		} else {
			if(sent.count(receiver->sn)){
				#ifdef SECDEBUG
					printf("Received ACK with sn: %d from %s::%d\n", receiver->sn, inet_ntoa(return_addr->sin_addr), return_addr->sin_port);
				#endif
				sem_map.wait();
				delete sent[receiver->sn]->direc;
				delete sent[receiver->sn]->frame;
				delete sent[receiver->sn];
				sent.erase(receiver->sn);
				sem_map.signal();
			}
		}
	}
}

void reUDP::sender(){
	while(true){
		std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));
		sem_map.wait();
		for(auto it = sent.begin(); it != sent.end(); ++it){
		#ifdef SECDEBUG
			printf("Sending %d to %s::%d\n", it->first, inet_ntoa(it->second->direc->addr), it->second->direc->port);
		#endif
			sock.Sendto((const char *) it->second->frame, 
						it->second->len + 3, //El tamano del buffer + 3 de encabezado
						it->second->direc->addr, 
						it->second->direc->port);
		}
		sem_map.signal();
	}
}

void reUDP::printPacket(const struct data_frame * packet, size_t len){
	printf("Size of data_frame is %ld\n", sizeof(struct data_frame));
	printf("Size of chunk packet is %ld\n", sizeof(BChunk));
	for(size_t i = 0; i < 3 + len; ++i){
		if(i % 32 == 0){
			printf("\n|0x%hhx|\t", (int) i);
		}
		printf(" 0x%hhx ", ((char *) packet)[i]);
	}
	printf("\n");
}
