#ifndef ORANGE_H
#define ORANGE_H

#include "packet.h"
#include "orangePacket.h"
#include "socket.h"
#include "encoder.h"
#include "error_handler.h"
#include "secudp.h"

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
#define NUM_ORANGES 2
#define COMM_ORANGE 10
#define COMM_BLUE 11

using namespace std;

/**
 * @class Orange
 * @brief La clase Orange representa un nodo naranja que está encargado de pasar el token, recibir y responder solicitudes de nodos 
 * azules, cargar el grafo de referencia, entre otras funcionalidades.
 */
class Orange{
	
    public:
		/**Identificador de este naranja [1, N].*/
        int id;
        
        /**Número de puerto por donde se recibirán comunicaciones de otros nodos naranja.*/
        unsigned short int orangeInPort;
        
        /**Número de puerto por donde se emitirán comunicaciones hacia otros nodos naranja.*/
        unsigned short int orangeOutPort;
        
        /**Número de puerto por donde se comunicarán los naranjas con azules.*/
        unsigned short int bluePort;
        
        /**Cantidad total de nodos naranja en la topología circular.*/
        size_t numTotalOranges;
        
        /**Bandera que marca si este nodo naranja creó el token vacío o no.*/
        bool tokenCreated;
        
        /**Bandera que marca si este nodo naranja cargó el token con datos de una asignación propia.*/
        bool tokenOccupied;	//bandera para saber si este nodo ocupó el token.
        
        /**Dirección IP en formato dotted-decimal del nodo vecino de este naranja.*/
        char rightIP[IP_LEN];
        
        /**Dirección IP en formato dotted-decimal de este nodo.*/
        char myIP[IP_LEN];
        
        /**Vector que contiene las direcciones IP de todos los nodos naranja en la topología, en formato de entero sin signo.*/
        vector<unsigned int> allNodesIP;

		/**Búfer privado de entrada para el proceso receptor.*/
        queue<PacketEntry*> privateInBuffer;
        
        /**Búfer privado de salida para el proceso emisor.*/
        queue<PacketEntry*> privateOutBuffer;
        
        /**Búfer compartido de entrada para el processer.*/
        queue<PacketEntry*> sharedInBuffer;
        
        /**Búfer compartido de salida para el processer.*/
        queue<PacketEntry*> sharedOutBuffer;
        
        /**Cola de solicitudes de unión al grafo de los nodos azules.*/
        queue<BlueRequest*> blueRequests;
        
        /**Mapa que relaciona un nodo del grafo con sus vecinos.*/
        unordered_map<int, list<int>> blue_graph;
        
        /**Mapa que contiene un nodo del grafo y su IP y puerto asociado, si existe.*/
        unordered_map<unsigned short int, pair<unsigned int, unsigned short int>> blueMapping;

		/**Candado para control del búfer compartido de entrada.*/
        pthread_mutex_t lockIn;
        
        /**Candado para control del búfer compartido de salida.*/
        pthread_mutex_t lockOut;
        
        /**Semáforos para control de entes productor-consumidor en los procesos sender y processer.*/
        sem_t InBufferSem;
        sem_t OutBufferSem;
        
        /**Sockets para la comunicación con otros nodos, naranjas y azules.*/
        Socket * orangeSocket;
        reUDP * blueSocket;
        
        /**Objeto para utilizar las funciones de codificación y decodificación de arreglos de bytes y estructuras.*/
        Code coder;

		/**
		 * @brief receiver Proceso receptor de comunicaciones en un nodo naranja.
		 * @param orange Puntero a "this".
		 * @param type Tipo de receptor, para distinguir entre receptor de naranjas y el de azules.
		 */
        void * receiver(Orange* orange, int type);
        
        /**
         * @brief processer Proceso que "procesa" los paquetes entrantes desde el receptor, y emite paquetes salientes para el emisor.
         * @param orange Puntero a "this".
         */
        void * processer(Orange* orange);
        
        /**
         * @brief sender Proceso que envía los paquetes recibidos para este efecto desde el procesador.
         * @param orange Puntero a "this".
         */
        void * sender(Orange* orange);
        
        /**
         * @brief beginContention Inicia la contención entre nodos naranja (hacer broadcast de IP's y elegir la menor).
         */
        void beginContention();
        
        /**
         * @brief addToIPList Agrega una dirección IP al vector de direcciones de todos los vecinos.
         * @param ip Direccion en formato de entero sin signo a agregar al vector.
         */
        void addToIPList(unsigned int ip);
        
        /**
         * @brief findMinIP Encuentra la IP menor entre las contenidas en el vector de IP de vecinos y la propia. 
         */  
        unsigned long findMinIP();
        
        /**
         * @brief createToken Crea el token inicial.
         * @param orange Puntero a "this".
         */
        void createToken(Orange* orange);
        void putInSendQueue(Orange*, Packet*, int destination = NODE_ORANGE, unsigned int destinationIP = 0, unsigned short int destinationPort = 0);
        void processInitialToken(PacketEntry*);
        void processEmptyToken(Orange*, PacketEntry*);
        void processFullRequestToken(Orange*, PacketEntry*);
        void processJoinRequest(Orange*, PacketEntry*);
        void respondToBlueRequest(Orange*, Token*);
        void initBlueMap();
        int findNextUnassigned(Orange*);
        

    public:

        Orange(int, unsigned short int, unsigned short int, int = 6, string = "Grafo_Referencia.csv");
        ~Orange();
        void requestIP();
        static void * receiverHelper(void *args);
        static void * processerHelper(void *context);
        static void * senderHelper(void *context);
        char* getIP();
        void init();
        void print_graph();
};

typedef struct orangeArgs
{
	Orange* node;
	int commWith;
} OrangeArgs;


#endif

