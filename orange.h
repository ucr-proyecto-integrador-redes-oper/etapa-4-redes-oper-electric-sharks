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
#include <vector>
#include <unordered_map>
#include <utility>
#include <semaphore.h>
#include <chrono>

#define IP_LEN 16
#define BUF_SIZE 1024
#define ORANGE_PORT 9999
#define BLUE_PORT 12000
#define NUM_INTERFACES 4
#define NUM_ORANGES 2
#define COMM_ORANGE 10
#define COMM_BLUE 11

using namespace std;

class Orange{
    public:
        unsigned short int bluePort;
        size_t numTotalOranges;
        bool tokenCreated;
        bool tokenOccupied;	//bandera para saber si este nodo ocupó el token.
        
        vector<unsigned int> allNodesIP;

        queue<PacketEntry*> privateInBuffer;
        queue<PacketEntry*> privateOutBuffer;
        queue<PacketEntry*> sharedInBuffer;
        queue<PacketEntry*> sharedOutBuffer;
        queue<PacketEntry*> blueRequests;
        
        unordered_map<int, list<int>> blue_graph;
        
        //Contiene un nodo del grafo y su IP y puerto asociado, si existe.
        unordered_map<unsigned short int, pair<unsigned int, unsigned short int>> blueMapping;

        pthread_mutex_t semIn;
        pthread_mutex_t semOut;
        
        sem_t InBufferSem;
        sem_t OutBufferSem;
        
        ID packetsID;
        Socket * orangeSocket;
        Socket * blueSocket;
        Code coder;

        void * receiver(Orange*, int type);
        void * processer(Orange*);
        void * sender(Orange*);
        void beginContention();
        int validateIP(char* ip);
        void addToIPList(unsigned int ip);
        unsigned long findMinIP();
        void createToken(Orange*);
        void putInSendQueue(Orange*, Packet*, int);
        void processInitialToken(PacketEntry*);
        void initBlueMap();
        unsigned short int findNextUnassigned(Orange*);
        size_t findPacketLen(Packet*);
        

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
        static void * receiverHelper(void *args);
        static void * processerHelper(void *context);
        static void * senderHelper(void *context);
        void init();
        void getHostIP();
        void print_graph();
};

typedef struct orangeArgs
{
	Orange* node;
	int commWith;
} OrangeArgs;


#endif

