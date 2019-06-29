#include <assert.h>

#include "blue.h"

Blue::Blue(char* orangeIPAddr)
{
	sem_init(&this->InBufferSem, 0, 0);
	sem_init(&this->OutBufferSem, 0, 0);
	pthread_mutex_init(&this->lockIn, nullptr);
	pthread_mutex_init(&this->lockOut, nullptr);
	this->socket = new Socket(Protocol::UDP);
	memcpy(this->myOrangeIP, orangeIPAddr, IP_LEN);
}

Blue::~Blue()
{
    sem_destroy(&this->InBufferSem);
	sem_destroy(&this->OutBufferSem);
	delete this->socket;
}
/** \brief Agrega vecinos al mapa y sus ip al vector
 *
 * \param Ip de vecinos
 * \param Puerto de vecinos
 * \return
 *
 */

void *Blue::sender(Blue* blue){
    char* rawPacket = nullptr;
    
    while(true){
        if(!blue->privateOutBuffer.empty()){
			PacketEntry* currentEntry = nullptr;
			currentEntry = blue->privateOutBuffer.front();
			assert(currentEntry);
			Packet* toSend = currentEntry->packet;
			blue->privateOutBuffer.pop();
			rawPacket = coder.encode(toSend);
			assert(rawPacket);
			size_t packetLen = 0;
			packetLen = Code::findPacketLen(toSend);
	
			/*Solo se comunica con su naranja por ahora, hace falta comunicar con otros azules.*/
			if(currentEntry->sendTo == NODE_ORANGE)
				blue->socket->Sendto(rawPacket, packetLen, blue->myOrangeIP, ORANGE_PORT);
			
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
		switch(currentEntry->packet->id){
			case ID::BOGRAPH_POSITION_E:
				blue->saveNeighbor(blue, currentEntry, false);
			break;
			
			case ID::BOGRAPH_POSITION_N:
				blue->saveNeighbor(blue, currentEntry, true);
			break;
			
			default:
				error_exit(-1, "Blue error: Id desconocido!\n");
		}
    }
}

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

        blue->socket->Recvfrom(buffer, BUF_SIZE, (type == COMM_ORANGE? ORANGE_PORT : BLUE_PORT), &senderAddr);
        
        /*Transforma la tira de bytes en un paquete*/
        currentPacket = coder.decode(buffer);
        assert(currentPacket);
		
		currentEntry->packet = currentPacket;
		currentEntry->typeNode = type;
		currentEntry->senderIP = senderAddr.sin_addr.s_addr;
		currentEntry->senderPort = senderAddr.sin_port;
		
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

void Blue::saveNeighbor(Blue* blue, PacketEntry* currentEntry, bool instantiated){
	Packet* packet;
	if(!instantiated)
		packet = (BOGraphPosition_E*) currentEntry->packet;
	else
		packet = (BOGraphPosition_N*) currentEntry->packet;
		
	blue->myGraphID = ((BOGraphPosition_E*) packet)->nodeID;
	
	unsigned short int neighborID = ((BOGraphPosition_E*)packet)->neighborID;
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

/** \brief Round robin
 *
 * \param El chunk
 * \param
 * \return
 *
 */
 
/*
void Blue::sendChunk(char chunk[1024]){
    int num_Neighbors = ports_Neighbors.size();
    int pointer_vector = 0;

    if(pointer_vector < num_Neighbors{
      //Envia el chunk al vecino al que este apuntando a ese momento a ip y puerto del mapa
      ++pointer_vector;
    }else{
      pointer_vector = 0;
      //si tiene espacio se deja el chunk porque ya lo envió a todos los vecinos
    }
}*/

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

void Blue::requestGraphNode(Blue* blue)
{
	BOJoinGraph* joinRequest = (BOJoinGraph*) calloc(1, sizeof(BOJoinGraph));
	joinRequest->id = ID::BOJOIN_GRAPH;
	putInSendQueue(blue, joinRequest, NODE_ORANGE);
}

int main(int argc, char* argv[]){
	if(argc < 2)
		return (cout << "Usage: " << argv[0] << " <orange_IP_addr>" << endl), -1;
	if(!Socket::validateIP(argv[1]))
		error_exit(-1, "Ip inválida!\n");
	
	Blue blueNode(argv[1]);
	
    pthread_t receiverBlues;
    pthread_t receiverOranges;
    pthread_t processer;
    pthread_t sender;
    
    BlueArgs args1, args2;
    
    args1.node = args2.node = &blueNode;
    
    args1.commWith = COMM_BLUE;
    args2.commWith = COMM_ORANGE;
    
    Socket::getHostIP(blueNode.getIP());

    pthread_create(&receiverBlues, NULL, &Blue::receiverHelper, &args1);
    pthread_create(&receiverOranges, NULL, &Blue::receiverHelper, &args2);
	
	pthread_create(&processer, NULL, &Blue::processerHelper, &blueNode);
	
	pthread_create(&sender, NULL, &Blue::senderHelper, &blueNode);

    /*Nunca hacen exit*/
	pthread_join(receiverBlues, (void**) nullptr);
	pthread_join(receiverOranges, (void**) nullptr);
	pthread_join(processer, (void**) nullptr);
	pthread_join(sender, (void**) nullptr);
}
