#ifndef BLUE_H
#define BLUE_H


#include "packet.h"
#include "socket.h"
#include "encoder.h"
#include "error_handler.h"
#include "BlueOrange.h"
#include "secudp.h"

#include <pthread.h>
#include <queue>
#include <map>
#include <vector>
#include <semaphore.h>

#define BUF_SIZE 1024
#define IP_LEN 16
#define ORANGE_PORT 13000
#define PORT 7050


using namespace std;

class Blue{
    public:
        queue<PacketEntry*> privateInBuffer;
        queue<PacketEntry*> privateOutBuffer;
        queue<PacketEntry*> sharedInBuffer;
        queue<PacketEntry*> sharedOutBuffer;
        queue<PacketEntry*> blueRequests;
    
        pthread_mutex_t semIn;
        pthread_mutex_t semOut;
        
        Code coder;
        reUDP socketSecure;
        char bufferIP[16];

        sem_t InBufferSem;
        sem_t OutBufferSem;

        int orangePort;
        char* orangeIP;

        map <unsigned int, unsigned short int> mapNeighbors;
        //map <unsigned int, unsigned short int>::iterator i;
        vector<unsigned int> ports_Neighbors;
        
        Blue(char*, int);
        virtual ~Blue();
        void neighbors(unsigned int,unsigned short int);
        void sendChunk(Packet* );
        void putInSendQueue(Blue* blue, Packet* p, int direction);
        static void * receiverHelper(void *context);
        static void * processerHelper(void *context);
        static void * senderHelper(void *context);


        void * receiver(Blue*);
        void * processer(Blue*);
        void * sender(Blue*);

    protected:
    private:

        int validateIP(char* ip);
    
};

#endif