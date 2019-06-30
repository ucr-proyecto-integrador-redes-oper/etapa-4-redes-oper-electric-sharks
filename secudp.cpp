#include "secudp.h"
#include "socket.h"
#include "semaphore.h"

#include <unordered_map>
#include <queue>
#include <thread>
#include <chrono>

#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <arpa/inet.h>
#include <sys/types.h>

#include <iostream>

//#define DEBUG

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
	#ifdef DEBUG
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

void reUDP::Recvfrom(char * message){
	sem_recv.wait();
	if(processed_messages.size() > 0){
		sem_queue.wait();
		struct data_frame * received_msg = processed_messages.front();
		processed_messages.pop();
		sem_queue.signal();
		memcpy((void *) message, (const void *) received_msg->payload, PAYLOAD_SIZE);
		delete received_msg;
		received_msg = nullptr;
	} else { 
		std::cerr << "somehow empty" << std::endl;
	}
}

void reUDP::receiver(){
	struct sockaddr_in return_addr;
	while(true){
		struct data_frame * receiver = new struct data_frame();
		sock.Recvfrom((char *) receiver, sizeof(struct data_frame), &return_addr);
		if(!receiver){
			std::cerr << "Invalid pointer at receiver" << std::endl;
		}
		if(receiver->type == 0){
			receiver->type = 1;
			#ifdef DEBUG
				printf("Sending ACK with sn: %d to %s::%d\n", receiver->sn, inet_ntoa(return_addr.sin_addr), return_addr.sin_port);
			#endif
			sock.Sendto((const char *) receiver, sizeof(struct data_frame), &return_addr);
			sem_queue.wait();
			processed_messages.push(receiver);
			sem_queue.signal();
			sem_recv.signal();
		} else {
			if(sent.count(receiver->sn)){
				#ifdef DEBUG
					printf("Received ACK with sn: %d from %s::%d\n", receiver->sn, inet_ntoa(return_addr.sin_addr), return_addr.sin_port);
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
		#ifdef DEBUG
			printf("Sending %d to %s::%d\n", it->first, inet_ntoa(it->second->direc->addr), it->second->direc->port);
		#endif
			sock.Sendto((const char *) it->second->frame, 
						it->second->len + 3,
						it->second->direc->addr, 
						it->second->direc->port);
		}
		sem_map.signal();
	}
}

void reUDP::printPacket(const struct data_frame * packet, size_t len){
	for(int i = 0; i < len; ++i){
		if(i % 32 == 0){
			printf("\n0x%hhx ", i);
		}
		printf(" 0x%hhx ", ((char *) packet)[i]);
	}
}
