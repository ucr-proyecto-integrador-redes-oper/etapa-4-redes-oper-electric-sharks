#ifndef ORANGE_H
#define ORANGE_H

#include "packet.h"
#include "socket.h"
#include "encoder.h"
#include "error_handler.h"
#include "BlueOrange.h"
#include "secudp.h"

#include <pthread.h>
#include <queue>
#include <map>
#include <semaphore.h>

#define BUF_SIZE 1024
#define IP_LEN 16
#define ORANGE_PORT 9999
#define BLUE_PORT 12000
#define COMM_ORANGE 10
#define COMM_BLUE 11


using namespace std;

class Blue{
    private:
		char myIP[IP_LEN];
		char myOrangeIP[IP_LEN];
    
        queue<PacketEntry*> privateInBuffer;
        queue<PacketEntry*> privateOutBuffer;
        queue<PacketEntry*> sharedInBuffer;
        queue<PacketEntry*> sharedOutBuffer;
        queue<PacketEntry*> blueRequests;
    
        pthread_mutex_t lockIn;
        pthread_mutex_t lockOut;
        
        reUDP* orangeSocket;
        reUDP* blueSocket;
        Code coder;

        sem_t InBufferSem;
        sem_t OutBufferSem;
        
        unsigned short int myGraphID;
        unsigned short int myPort;

        void putInSendQueue(Blue* blue, Packet* p, int direction);

        map <unsigned short int, pair<unsigned int, unsigned short int>> mapNeighbors;
        //map <unsigned int, unsigned short int>::iterator i;
        vector<unsigned int> ports_Neighbors;
        
        void saveNeighbor(Blue*, PacketEntry*, bool);
        void sendChunk(char []);

        void * receiver(Blue*, int);
        void * processer(Blue*);
        void * sender(Blue*);
        
        void requestGraphNode(Blue*);
        void greetNeighbor(Blue*);

	public:
		Blue(char*, unsigned short int);
        ~Blue();
        
        char* getIP();
        
        static void * receiverHelper(void *context);
        static void * processerHelper(void *context);
        static void * senderHelper(void *context);
        
};

typedef struct BlueArgs
{
	Blue* node;
	int commWith;
}BlueArgs;

#endif
