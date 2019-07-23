#ifndef ORANGE_H
#define ORANGE_H

#include "packet.h"
#include "socket.h"
#include "encoder.h"
#include "error_handler.h"
#include "BlueOrange.h"
#include "bluePacket.h"
#include "secudp.h"

#include <pthread.h>
#include <queue>
#include <map>
#include <semaphore.h>
#include "message.h"

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

        /*Buffers para compartir información entre threads */
        queue<PacketEntry*> privateInBuffer;
        queue<PacketEntry*> privateOutBuffer;
        queue<PacketEntry*> sharedInBuffer;
        queue<PacketEntry*> sharedOutBuffer;
        queue<PacketEntry*> blueRequests;
    
        /*Herramientas para controlar la concurrencia entre threads*/
        pthread_mutex_t lockIn;
        pthread_mutex_t lockOut;
        sem_t InBufferSem;
        sem_t OutBufferSem;
        
        /**Sockets para la comunicación con otros nodos, naranjas y azules.*/
        reUDP* orangeSocket;
        reUDP* blueSocket;
        Code coder;

		/* Mapa que contiene a los vecinos en el grafo del nodo */
        map <unsigned short int, pair<unsigned int, unsigned short int>> mapNeighbors;
        
        //ID asignado por el nodo naranja
        unsigned short int myGraphID;

        //Puerto por el cual se recibe la comunicacion con otros nodos
        unsigned short int myPort;

		//Cola de mensajes para monitorear el estado del nodo
		Message msgq;

		long seriesNumber;

        void putInSendQueue(Blue* blue, Packet* p, int direction, uint16_t = 0);


        //Vector en donde se almacenan los puertos de los nodos recibidos como vecinos. Se utiliza para iterar sobre el mapa de vecinos.
        vector<unsigned int> ports_Neighbors;
        
        void saveNeighbor(Blue*, PacketEntry*, bool);
		void attendNeighbor(Blue*, PacketEntry*);
        void sendChunk(char []);

        /**Threads */
        void * receiver(Blue*, int);
        void * processer(Blue*);
        void * sender(Blue*);
		void * monitor(Blue*, long);
        
        void requestGraphNode(Blue*);
        void greetNeighbor(Blue*, uint16_t);

	public:
		Blue(char*, unsigned short int, unsigned short int, long);
        ~Blue();
        
        char* getIP();
        
        static void * receiverHelper(void *context);
        static void * processerHelper(void *context);
        static void * senderHelper(void *context);
		static void * monitorHelper(void * context);
        
};

typedef struct BlueArgs
{
	Blue* node;
	int commWith;
}BlueArgs;

typedef struct MonitorArgs
{
	Blue* node;
	long seriesNumber;
} MonitorArgs;

#endif
