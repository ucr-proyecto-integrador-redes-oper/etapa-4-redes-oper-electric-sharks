#include "secudp.h"
#include "socket.h"

#include <unordered_map>
#include <queue>
#include <thread>
#include <chrono>

#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <arpa/inet.h>

void reUDP::run(){
	std::thread(&reUDP::receiver, this).detach();
	std::thread(&reUDP::sender, this).detach();
}

void reUDP::Sendto(const char * message, const char * destination, uint16_t port){
	struct data_frame * frame = new struct data_frame();
	frame->type = 0;
	frame->sn = sn;
	memcpy((void *) frame->payload, (const void *) message, PAYLOAD_SIZE);

	struct direction * direc = new struct direction();
	inet_aton(destination, &direc->addr);
	direc->port = htons(port);
	
	struct map_entry * entry = new struct map_entry();
	entry->direc = direc;
	entry->frame = frame;
	entry->received = false;
	
	sent[sn] = entry;
	sn += 1;
}

void reUDP::Recvfrom(char * message){
	//Para que Recvfrom se bloquee mientras no hayan mensajes se deberia de hacer una clase semaforo
	if(processed_messages.size() > 0){
		char * received_msg = processed_messages.front();
		processed_messages.pop();
		memcpy((void *) message, (const void *) received_msg, PAYLOAD_SIZE);
		delete received_msg;
	}
}

void reUDP::receiver(){
	struct sockaddr_in return_addr;
	while(true){
		struct data_frame * receiver = new struct data_frame();
		sock.Recvfrom((char *) receiver, sizeof(struct data_frame), &return_addr);
		if(receiver->type == 0){
			receiver->type = 1;
			sock.Sendto((const char *) receiver, sizeof(struct data_frame), &return_addr);
		} else {
			if(sent.count(receiver->sn)){
				sent[sn]->received = true;
			}
		}
	}
}

void reUDP::sender(){
	while(true){
		std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));
		for(auto it = sent.begin(); it != sent.end(); ++it){
			if(it->second->received){
				delete it->second->direc;
				delete it->second->frame;
				delete it->second;
				sent.erase(it);
			} else {
				sock.Sendto((const char *) it->second->frame, 
							sizeof(struct data_frame), 
							it->second->direc->addr, 
							it->second->direc->port);
			}
		}
	}
}
