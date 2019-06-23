#ifndef ORANGE_H
#define ORANGE_H

#include "packet.h"
#include "orangePacket.h"
#include "socket.h"
#include "encoder.h"
#include "error_handler.h"

#include <pthread.h>
#include <queue>
#include <list>
#include <unordered_map>
#include <semaphore.h>

#define IP_LEN 16
#define BUF_SIZE 1024
#define ORANGE_PORT 11000
#define BLUE_PORT 12000
#define NUM_INTERFACES 4

using namespace std;

class Orange{
    private:
        unsigned short int bluePort;
        size_t numTotalOranges;
        
        unsigned int* ipBuffer;

        queue<Packet*> privateInBuffer;
        queue<Packet*> privateOutBuffer;
        queue<Packet*> sharedInBuffer;
        queue<Packet*> sharedOutBuffer;
        
        unordered_map<int, list<int>> blue_graph;

        pthread_mutex_t semIn;
        pthread_mutex_t semOut;
        
        sem_t InBufferSem;
        sem_t OutBufferSem;
        
        ID packetsID;
        Socket * orangeSocket;
        Socket * blueSocket;
        Code coder;

        void * receiver(Orange*);
        void * processer(Orange*);
        void * sender(Orange*);
        void beginContention(Orange*);
        int validateIP(char* ip);
        

    public:
        unsigned short int orangeInPort;
        unsigned short int orangeOutPort;
        char leftIP[IP_LEN];
        char rightIP[IP_LEN];
        char myIP[IP_LEN];
        int id;

        Orange(int, unsigned short int, unsigned short int, int = 6, string = "Grafo_Referencia.csv");
        ~Orange();
        void requestIP();
        static void * receiverHelper(void *context);
        static void * processerHelper(void *context);
        static void * senderHelper(void *context);
        void init();
        void getHostIP();
        void print_graph();
};

#endif

