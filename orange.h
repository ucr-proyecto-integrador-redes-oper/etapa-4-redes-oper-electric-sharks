#ifndef ORANGE_H
#define ORANGE_H

#include "packet.h"
#include "orangePacket.h"
#include "socket.h"
#include "encoder.h"

#include <pthread.h>
#include <queue>
#include <semaphore.h>

#define IP_LEN 15
#define BUF_SIZE 1024
#define ORANGE_PORT 11000
#define BLUE_PORT 12000

using namespace std;

class Orange{
    private:
        unsigned short int bluePort;
        size_t numTotalOranges;
        
        unsigned int* ipBuffer;

        queue<Packet> privateInBuffer;
        queue<Packet> privateOutBuffer;
        queue<Packet> sharedInBuffer;
        queue<Packet> sharedOutBuffer;

        pthread_mutex_t semIn;
        pthread_mutex_t semOut;
        
        sem_t InBufferSem;
        sem_t OutBufferSem;
        
        ID packetsID;
        
        Socket * orangeSocket;
        Socket * blueSocket;

        void * reciver();
        void * processer();
        void * sender();
        void beginContention();
        

    public:
        unsigned short int orangeInPort;
        unsigned short int orangeOutPort;
        char leftIP[IP_LEN];
        char rightIP[IP_LEN];
        char myIP[IP_LEN];
        int id;

        Orange();
        ~Orange();
        Orange(int, unsigned short int, unsigned short int, int);
        void requestIP();
        static void * reciverHelper(void *context);
        static void * processerHelper(void *context);
        static void * senderHelper(void *context);
        void init();
        void getHostIP();
};

#endif

