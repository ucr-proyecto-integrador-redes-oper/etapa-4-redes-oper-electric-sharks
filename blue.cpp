#include <assert.h>
#include <string>
#include <cstdio>

#include "blue.h"

Blue::Blue(char* orangeIPAddr, unsigned short int bluePort, unsigned short int orangePort)
{
	sem_init(&this->InBufferSem, 0, 0);
	sem_init(&this->OutBufferSem, 0, 0);
	pthread_mutex_init(&this->lockIn, nullptr);
	pthread_mutex_init(&this->lockOut, nullptr);
	this->orangeSocket = new reUDP(orangePort);
	this->blueSocket = new reUDP(bluePort);
	this->orangeSocket->run();
	this->blueSocket->run();
	
	memcpy(this->myOrangeIP, orangeIPAddr, IP_LEN);
}

Blue::~Blue()
{
    sem_destroy(&this->InBufferSem);
	sem_destroy(&this->OutBufferSem);
	delete this->orangeSocket;
	delete this->blueSocket;
}

///Funcion ejecutada en paralelo por el thread que va a enviar a otos nodos naranjas y azules información en la cola de salida
void *Blue::sender(Blue* blue){
    char* rawPacket = nullptr;
    
    while(true){
        if(!blue->privateOutBuffer.empty()){
			PacketEntry* currentEntry = nullptr;
			currentEntry = blue->privateOutBuffer.front();
			assert(currentEntry);
			Packet* toSend = currentEntry->packet;
			blue->privateOutBuffer.pop();
			rawPacket = coder.encode(toSend, NODE_BLUE);
			assert(rawPacket);
			size_t packetLen = 0;
			packetLen = Code::findPacketLen(toSend);
	
			/*Solo se comunica con su naranja por ahora, hace falta comunicar con otros azules.*/
			if(currentEntry->sendTo == NODE_ORANGE)
				blue->orangeSocket->Sendto(rawPacket, blue->myOrangeIP, BLUE_PORT, packetLen);
			
			free(toSend);
            free(currentEntry);
        }
        else{                                       //Si la cola privada está vacía, busca en la cola compartida
			sem_wait(&blue->OutBufferSem);
            pthread_mutex_lock(&blue->lockOut);
            if(!blue->sharedOutBuffer.empty()){
                blue->privateOutBuffer.push(blue->sharedOutBuffer.front());
                blue->sharedOutBuffer.pop();
            }
            pthread_mutex_unlock(&blue->lockOut);
        }
    }
}

///Funcion que toma mensajes de la cola privada con receiver y los procesa según su tipo
void *Blue::processer(Blue* blue)
{
	blue->requestGraphNode(blue);
	PacketEntry* currentEntry;
	
    while(true){
        /*Lee la cola compartida, saca el paquete del frente y lo procesa */
        
        sem_wait(&blue->InBufferSem);	//Espera a que haya algo en la cola para procesar
        pthread_mutex_lock(&blue->lockIn);
        assert(!blue->sharedInBuffer.empty());
        currentEntry = blue->sharedInBuffer.front();
        blue->sharedInBuffer.pop();
        
        pthread_mutex_unlock(&blue->lockIn);
		assert(currentEntry);
		assert(currentEntry->packet);
		
		/*Hace las diferentes acciones con los paquetes. Por ahora solo trabaja con los paquetes de asignación de nodo del grafo.*/
		if(static_cast<BlueOrange*>(currentEntry->packet)){
			switch(currentEntry->packet->id){
				case ID::BOGRAPH_POSITION_E:
					blue->saveNeighbor(blue, currentEntry, false);
				break;
				
				case ID::BOGRAPH_POSITION_N:
					blue->saveNeighbor(blue, currentEntry, true);
					blue->greetNeighbor(blue);
				break;
				
				case ID::BOGRAPH_COMPLETE:
					std::cout << "yeah boi" << std::endl;
				break;
				
				default:
					error_exit(-1, "Blue error: Id desconocido!\n");
			}
		}
    }
}

///Función ejecutada en paralelo para escuchar a los otros nodos del sistema
void *Blue::receiver(Blue* blue, int type){
    char* buffer = new char[BUF_SIZE];
    memset(buffer, 0, BUF_SIZE);
    Packet* currentPacket;
    PacketEntry* currentEntry = nullptr;
    struct sockaddr_in senderAddr;
    int status = 0;
    
    
    while(true){
        currentEntry = (PacketEntry*) calloc(1, sizeof(PacketEntry));
        /*Lee del socket*/

       if(type == COMM_BLUE)
			blue->blueSocket->Recvfrom(buffer, &senderAddr);
        else
			blue->orangeSocket->Recvfrom(buffer, &senderAddr);
        /*Transforma la tira de bytes en un paquete*/
        currentPacket = coder.decode(buffer, PACKET_BLUE);
        assert(currentPacket);
		
		currentEntry->packet = currentPacket;
		currentEntry->typeNode = PACKET_BLUE;
		currentEntry->senderIP = senderAddr.sin_addr.s_addr;
		currentEntry->senderPort = senderAddr.sin_port;	//problema
		
		/*Mete el paquete a la cola privada*/
		blue->privateInBuffer.push(currentEntry);
		
		memset(buffer, 0, BUF_SIZE);
		
		/*Si la cola compartida está siendo usada, sigue escuchando el socket, sino mete paquetes a la cola compartida*/
		status = pthread_mutex_trylock(&blue->lockIn);
		
		if(status == EBUSY)
			continue;
			
		while(!blue->privateInBuffer.empty()){
			blue->sharedInBuffer.push(blue->privateInBuffer.front());
			blue->privateInBuffer.pop();
		}
		
		pthread_mutex_unlock(&blue->lockIn);
		
		/*Avisa al processer que hay más paquetes para procesar con un signal*/
		sem_post(&blue->InBufferSem);
		
    }
}

/* Funciones estaticas para ser llamadas por pthreads */
void *Blue::senderHelper(void *context){
    return ((Blue *)context)->sender((Blue*) context);
}

void *Blue::processerHelper(void *context){
    return ((Blue *)context)->processer((Blue*) context);
}

void *Blue::receiverHelper(void *args){
	BlueArgs* arg = (BlueArgs*) args;
    return ((Blue*)arg->node)->receiver((Blue*)arg->node, ((BlueArgs*)args)->commWith);
}

void *Blue::monitorHelper(void *args){
	MonitorArgs* arg = (MonitorArgs *) args;
	return ((Blue *)arg->node)->monitor((Blue*)arg->node, ((MonitorArgs*)args)->seriesNumber);
}

///Funcion que guarda un nuevo vecino en el mapa de vecinos. Ademas, se guarda el puerto en un vector para luego iterar sobre el mapa
void Blue::saveNeighbor(Blue* blue, PacketEntry* currentEntry, bool instantiated){
	Packet* packet;
	if(!instantiated)
		packet = (BOGraphPosition_E*) currentEntry->packet;
	else
		packet = (BOGraphPosition_N*) currentEntry->packet;
		
	blue->myGraphID = ((BOGraphPosition_E*) packet)->nodeID;
	
	assert(blue->myGraphID > 0);
	
	unsigned short int neighborID = ((BOGraphPosition_E*)packet)->neighborID;
	//unsigned short int node = ((BOGraphPosition_E*)packet)->nodeID;
	
	cout << "Guardando asignación: soy el nodo del grafo: " << blue->myGraphID << " vecino: " << neighborID << " vecino instanciado: " << std::boolalpha << instantiated << endl;
	
	//Mapa de puerto con ip de cada vecino
	if(instantiated){
		unsigned int neighborIP = ((BOGraphPosition_N*)packet)->neighborIP;
		unsigned short int neighborPort = ((BOGraphPosition_N*)packet)->neighborPort;
		mapNeighbors[neighborID] = make_pair(neighborIP, neighborPort);
		//vector que guarda solo ip de vecinos para iterar sobre estos para el round robin
		ports_Neighbors.push_back(neighborIP);
	}else{
		mapNeighbors[neighborID] = make_pair(0, 0);
	}
	free(currentEntry->packet);
	free(currentEntry);
}

//Funcion que toma un paquete y le agrega informacion en una nueva estructura para ser enviado por el thread sender
void Blue::putInSendQueue(Blue* blue, Packet* p, int direction)
{	
	PacketEntry* newPacket = (PacketEntry*) calloc(1, sizeof(PacketEntry));
	newPacket->packet = p;
	newPacket->sendTo = direction;
	pthread_mutex_lock(&blue->lockOut);
	blue->privateOutBuffer.push(newPacket);
	pthread_mutex_unlock(&blue->lockOut);
	sem_post(&blue->OutBufferSem);
}

char* Blue::getIP()
{
	return this->myIP;
}

///Crea un paquete para solicitar a un nodo naranja la asignación en la red de almacenamiento
void Blue::requestGraphNode(Blue* blue)
{
	BOJoinGraph* joinRequest = (BOJoinGraph*) calloc(1, sizeof(BOJoinGraph));
	joinRequest->id = ID::BOJOIN_GRAPH;
	putInSendQueue(blue, joinRequest, NODE_ORANGE);
}

void Blue::greetNeighbor(Blue* blue)
{
	assert(!blue->mapNeighbors.empty());
	for(auto entry : blue->mapNeighbors){
		//if(entry.second.first != 0)
			std::cout << "GID: " << blue->myGraphID << " NID: " << entry.first << std::endl;
	}
	
}


void * Blue::monitor(Blue * blue, long id){
	struct my_msgbuf message;
	while(true){
		msgq.receive(&message, id);
		switch(message.question){
			case 1:
				message.mtype = 9999;
				message.nodeID = blue->myGraphID;
				break;
			default:
				std::cout << "Not quite there yet" << std::endl;
		}
		msgq.send(&message);
	}
}

int main(int argc, char* argv[]){
	if(argc < 6)
		return (cout << "Usage: " << argv[0] << " <orange_IP_addr> <blue_port> <orange_port> <blue_host_IP> <series_number>" << endl), -1;
	if(!Socket::validateIP(argv[1]))
		error_exit(-1, "IP inválida!\n");

	unsigned short int bluePort = 0;
	unsigned short int orangePort = 0;
	bluePort = (unsigned short int) atoi(argv[2]);
	orangePort = (unsigned short int) atoi(argv[3]);

	//validar puerto!!
	Blue blueNode(argv[1], bluePort, orangePort);

	pthread_t receiverBlues;
	pthread_t receiverOranges;
	pthread_t processer;
	pthread_t sender;
	pthread_t monitor;
 
	BlueArgs args1, args2;
	MonitorArgs mArgs;
	mArgs.node = &blueNode;
	mArgs.seriesNumber = std::stol(argv[5]);

	args1.node = args2.node = &blueNode;

	args1.commWith = COMM_BLUE;
	args2.commWith = COMM_ORANGE;

	strcpy(blueNode.getIP(), argv[4]);


	pthread_create(&receiverBlues, NULL, &Blue::receiverHelper, &args1);
	pthread_create(&receiverOranges, NULL, &Blue::receiverHelper, &args2);

	pthread_create(&processer, NULL, &Blue::processerHelper, &blueNode);

	pthread_create(&sender, NULL, &Blue::senderHelper, &blueNode);
	
	pthread_create(&monitor, NULL, &Blue::monitorHelper, &mArgs);

	/*Nunca hacen exit*/
	pthread_join(receiverBlues, (void**) nullptr);
	pthread_join(receiverOranges, (void**) nullptr);
	pthread_join(processer, (void**) nullptr);
	pthread_join(sender, (void**) nullptr);
	pthread_join(monitor, (void**) nullptr);
}
