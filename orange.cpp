#include "orange.h"
#include "loadCSV.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <pthread.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <thread>
#include <limits>
#include <iomanip>

using namespace std;

Orange::Orange(int id, unsigned short int orangeInPort, unsigned short int orangeOutPort, int totalOranges, string csv_file)
: id(id), orangeInPort(orangeInPort), orangeOutPort(orangeOutPort), numTotalOranges(totalOranges), tokenCreated(false)
{
	sem_init(&this->InBufferSem, 0, 0);
	sem_init(&this->OutBufferSem, 0, 0);
	pthread_mutex_init(&this->lockIn, nullptr);
	pthread_mutex_init(&this->lockOut, nullptr);
	this->orangeSocket = new Socket(Protocol::UDP);
	this->blueSocket = new Socket(Protocol::UDP);
	loadCSV(csv_file, &this->blue_graph);
	initBlueMap();
	this->allNodesIP.resize(0);
}

Orange::~Orange()
{
	sem_destroy(&this->InBufferSem);
	sem_destroy(&this->OutBufferSem);
	/*delete this->orangeSocket;
	delete this->blueSocket;*/
}

void Orange::requestIP(){
    bool validIp = true;
    cout <<"Ingrese el número IP del vecino derecho"<<endl;
    do{
		cin >> this->rightIP;
		validIp = (bool) Socket::validateIP(this->rightIP);
		if(!validIp)
			cout << "Direccion IP incorrecta! Ingrese una direccion valida: " << endl;
	}while(!validIp);
}

///Funcion que verifica la cantidad de datos por consola y los convierte de strings a los datos de la clase Orange
void get_args(int &id, unsigned short int &orangeInPort, unsigned short int &orangeOutPort, int argc, char ** argv){
    switch(argc){
		case 1:
        case 2:
		case 3:
			perror("Número de argumentos menor al requerido\n");
			printf("Formato: %s <id> <puerto de entrada> <puerto de salida>\n", argv[0]);
            exit(EXIT_FAILURE);
        default:

            if((id = stoi(argv[1])) == 0 || (orangeInPort = (unsigned short) stoi(argv[2], NULL)) == 0  || \
            (orangeOutPort = (unsigned short) stoi(argv[3], NULL)) == 0){ // Si el argumento no es un número válido el programa se cae aquí
				perror("Invalid argument!\n");
				printf("Format: %s <port>\n", argv[0]);
				exit(EXIT_FAILURE);
            }
    }
}

///Funcion para el thread que recibe paquetes del socket y los pone en la cola compartida para el siguiente thread
void *Orange::receiver(Orange* orange, int type){
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
			orange->blueSocket->Recvfrom(buffer, BUF_SIZE, BLUE_PORT, &senderAddr);
        else
			orange->orangeSocket->Recvfrom(buffer, BUF_SIZE, ORANGE_PORT, &senderAddr);
        /*Transforma la tira de bytes en un paquete*/
        currentPacket = coder.decode(buffer, (type == COMM_BLUE? PACKET_BLUE : PACKET_ORANGE));
        assert(currentPacket);
		
		currentEntry->packet = currentPacket;
		currentEntry->typeNode = type;
		currentEntry->senderIP = senderAddr.sin_addr.s_addr;
		currentEntry->senderPort = senderAddr.sin_port;
		
		/*Mete el paquete a la cola privada*/
		orange->privateInBuffer.push(currentEntry);
		
		memset(buffer, 0, BUF_SIZE);
		
		/*Si la cola compartida está siendo usada, sigue escuchando el socket, sino mete paquetes a la cola compartida*/
		status = pthread_mutex_trylock(&orange->lockIn);
		
		if(status == EBUSY)
			continue;
			
		while(!orange->privateInBuffer.empty()){
			orange->sharedInBuffer.push(orange->privateInBuffer.front());
			orange->privateInBuffer.pop();
		}
		
		pthread_mutex_unlock(&orange->lockIn);
		
		/*Avisa al processer que hay más paquetes para procesar con un signal*/
		sem_post(&orange->InBufferSem);
		
    }
}

void *Orange::receiverHelper(void *args){
	OrangeArgs* arg = (OrangeArgs*) args;
    return ((Orange*)arg->node)->receiver((Orange*)arg->node, ((OrangeArgs*)args)->commWith);
}

///Funcion para el thread que toma un paquete de la cola compartida, lo procesa y pone mensajes en la cola compartida para el siguiente thread
void *Orange::processer(Orange* orange){
	PacketEntry* currentEntry;
	
	orange->beginContention();
	
    while(true){
        /*Lee la cola compartida, saca el paquete del frente y lo procesa */
        
        sem_wait(&orange->InBufferSem);	//Espera a que haya algo en la cola para procesar
        pthread_mutex_lock(&orange->lockIn);
        assert(!orange->sharedInBuffer.empty());
        currentEntry = orange->sharedInBuffer.front();
        orange->sharedInBuffer.pop();
        
        pthread_mutex_unlock(&orange->lockIn);
		assert(currentEntry);
		assert(currentEntry->packet);
		/*Hace las diferentes acciones con los paquetes */
		switch(currentEntry->packet->id){
			case ID::INITIAL_TOKEN:
				orange->processInitialToken(currentEntry);
			break;
			
			case ID::TOKEN_EMPTY:
				orange->processEmptyToken(orange, currentEntry);
			break;
			
			case ID::TOKEN_FULL_AND_REQUEST:
				orange->processFullRequestToken(orange, currentEntry);
			break;
			
			case ID::BOJOIN_GRAPH:
				orange->processJoinRequest(orange, currentEntry);
			break;
			
			default:
				error_exit(-1, "Orange error: Id desconocido!\n");
		}
    }
}

void *Orange::processerHelper(void *context){
    return ((Orange *)context)->processer((Orange*) context);
}

///Funcion que toma paquetes de la cola compartida y los envia por el socket
void *Orange::sender(Orange* orange){
    char* rawPacket = nullptr;
    
    while(true){
        if(!orange->privateOutBuffer.empty()){
			PacketEntry* currentEntry = nullptr;
			currentEntry = orange->privateOutBuffer.front();
			assert(currentEntry);
			Packet* toSend = currentEntry->packet;
			orange->privateOutBuffer.pop();
			
			size_t packetLen = 0;
			packetLen = Code::findPacketLen(toSend);
			/*Envía el paquete a su vecino derecho.*/
			if(currentEntry->sendTo == NODE_ORANGE){
				rawPacket = coder.encode(toSend, NODE_ORANGE);
				assert(rawPacket);
				orange->orangeSocket->Sendto(rawPacket, packetLen, orange->rightIP, ORANGE_PORT);
			}else if(currentEntry->sendTo == NODE_BLUE){
				assert(currentEntry->sendToPort != 0);
				rawPacket = coder.encode(toSend, NODE_BLUE);
				assert(rawPacket);
				char buffer[IP_LEN];
				cout << "Respondiendo solicitud de azul al puerto: " << currentEntry->sendToPort << endl;
				orange->orangeSocket->Sendto(rawPacket, packetLen, Socket::decode_ip(currentEntry->sendToIP, buffer), currentEntry->sendToPort);
			}
			free(toSend);
            free(currentEntry);
        }
        else{                                       //Si la cola privada está vacía, busca en la cola compartida
			sem_wait(&orange->OutBufferSem);
            pthread_mutex_lock(&orange->lockOut);
            if(!orange->sharedOutBuffer.empty()){
                orange->privateOutBuffer.push(orange->sharedOutBuffer.front());
                orange->sharedOutBuffer.pop();
            }
            pthread_mutex_unlock(&orange->lockOut);
        }
    }
}

void *Orange::senderHelper(void *context){
    return ((Orange *)context)->sender((Orange*) context);
}



void Orange::beginContention()
{
	InitialToken* p = (InitialToken*) calloc(1, sizeof(InitialToken));
	p->id = ID::INITIAL_TOKEN;
	p->ip = this->orangeSocket->encode_ip(this->myIP);
	this->putInSendQueue(this, p, NODE_ORANGE);
}

void Orange::createToken(Orange* orange)
{
	Packet* token = (Token*) calloc(1, sizeof(Token));
	token->id = ID::TOKEN_EMPTY;
	((Token*)token)->assignedPort = 0;
	((Token*)token)->boolean = false;
	orange->tokenCreated = true;
	orange->putInSendQueue(orange, token);
}

void Orange::putInSendQueue(Orange* orange, Packet* p, int destination, unsigned int destinationIP, unsigned short int destinationPort)
{	
	PacketEntry* newPacket = (PacketEntry*) calloc(1, sizeof(PacketEntry));
	newPacket->packet = p;
	newPacket->sendTo = destination;
	newPacket->sendToIP = destinationIP;
	newPacket->sendToPort = destinationPort;
	pthread_mutex_lock(&orange->lockOut);
	orange->privateOutBuffer.push(newPacket);
	pthread_mutex_unlock(&orange->lockOut);
	sem_post(&orange->OutBufferSem);
}

void Orange::print_graph(){
	for(auto it = blue_graph.begin(); it != blue_graph.end(); ++it){
		cout << it->first << ": ";
		for(auto lit = it->second.begin(); lit != it->second.end(); ++lit){
			cout << *lit << " ";
		}
		cout << endl;
	}
}

void Orange::addToIPList(unsigned int ip)
{
	/*Si ya tiene las IP de todos sus vecinos, no se debe hacer nada.*/
	if(this->allNodesIP.size() >= this->numTotalOranges - 1)
		return;
		
	/*Si la IP ya está en el vector, no hace nada, sino la agrega.*/
	auto position = find(this->allNodesIP.begin(), this->allNodesIP.end(), ip);
	if(position != this->allNodesIP.end())
		return;
	else
		this->allNodesIP.push_back(ip);
	
}

void Orange::processInitialToken(PacketEntry* currentEntry)
{
	char buffer[IP_LEN];
	this->orangeSocket->decode_ip((static_cast<InitialToken*>(currentEntry->packet))->ip, buffer);
	assert(currentEntry);
	assert(currentEntry->packet);
	assert(((InitialToken*)currentEntry->packet)->ip != 0);
	cout << "Node " << buffer <<" says hello! " << endl;
	
	/*Si el paquete que recibió no lo creó este nodo*/
	if(strcmp(buffer, this->myIP) != 0){
		this->addToIPList(((InitialToken*)currentEntry->packet)->ip);
		if(this->allNodesIP.size() == this->numTotalOranges - 1){
			
		/*Si la ip de este naranja es menor que la de todos sus vecinos, crea el token.*/
			if(!this->tokenCreated && this->orangeSocket->encode_ip(this->myIP) < this->findMinIP()){
				cout << "Gané!" << endl;
				//crear token
				this->createToken(this);
				//TO-DO: poner un timer aquí, en 3 minutos, por si el token no llega de nuevo se vuelve a crear.
			}
		}
		/*Si el paquete lo recibió por el lado izquierdo, lo reenvía al nodo derecho, y si no al izquierdo.*/
		this->putInSendQueue(this, currentEntry->packet);
		free(currentEntry);
	}else{
		/*Si el paquete que recibió lo creó este nodo, lo bota de la red.*/
		free(currentEntry->packet);
		free(currentEntry);
	}
}

void Orange::processEmptyToken(Orange* orange, PacketEntry* currentEntry)
{
	Token* token = (Token*)currentEntry->packet;
	//si el token está libre, y tiene asignaciones pendientes
	if(!orange->blueRequests.empty()){
		BlueRequest* request = orange->blueRequests.front();
		orange->blueRequests.pop();
		//asigna un azul a un nodo del grafo
		int newAssignment = orange->findNextUnassigned(orange);
		if(newAssignment == -1){
			error_exit(-1, "Error! No se pueden asignar más nodos del grafo!\n");
		}
		token->id = ID::TOKEN_FULL_AND_REQUEST;
		token->node = newAssignment;
		token->assignedIp = request->blueIP;
		token->assignedPort = request->bluePort;
		orange->tokenOccupied = true;
		
		orange->blueMapping[newAssignment] = make_pair(request->blueIP, request->bluePort);
		char buffer[IP_LEN];
		cout << "Se cargó asignación de nodo del grafo: " << newAssignment << " al azul en: " << Socket::decode_ip(request->blueIP, buffer) << ":" << request->bluePort << endl; 
		free(request);
		/*Primero manda a circular el token con la asignación, y luego le responde al azul.*/
		orange->putInSendQueue(orange, token, NODE_ORANGE);
		orange->respondToBlueRequest(orange, token);
	}else{
		//sino, solo pasa el token al vecino derecho
		cout << "recibí el token " << endl;
		cout << "pasando el token a " << orange->rightIP << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		orange->putInSendQueue(orange, token, NODE_ORANGE);
	}
}

void Orange::processFullRequestToken(Orange* orange, PacketEntry* currentEntry)
{
	Token* token = (Token*) currentEntry->packet;
	//si fue el mismo quien lo ocupó, lo libera y limpia la información de la asignación
	if(orange->tokenOccupied){
		token->id = ID::TOKEN_EMPTY;
		orange->tokenOccupied = false;
		token->boolean = false;
		token->assignedIp = 0;
		token->assignedPort = 0;
	}else{
	//sino, anota la asignacion en su tabla local
		try{
			orange->blueMapping.at(token->node);
		}catch(exception &e){
			cerr << "El nodo " << token->node << " no existe en el grafo! " << endl;
			assert(false);
		}

		/*Crea un par con la id y puerto de la asignación en el token, y se los asocia al nodo del grafo
		 * indicado en el campo token->node.*/
		char buffer[IP_LEN];
		cout << "Anotando asignación azul:\nNodo del grafo: " << token->node << "\nIP: " << Socket::decode_ip(token->assignedIp, buffer)\
		<< "\nPuerto: " << token->assignedPort << endl;
		orange->blueMapping[token->node] = make_pair(token->assignedIp, token->assignedPort);
	}
	orange->putInSendQueue(orange, token, NODE_ORANGE);
}

void Orange::processJoinRequest(Orange* orange, PacketEntry* currentEntry)
{
	/*Crea un paquete para manejo de solicitudes a nivel interno, y lo agrega a la cola de solicitudes
	 * para atenderlo cuando llegue el token vacío.*/
	 
	BlueRequest* newRequest = (BlueRequest*) calloc(1, sizeof(BlueRequest));
	newRequest->blueIP = currentEntry->senderIP;
	newRequest->bluePort = currentEntry->senderPort;
	orange->blueRequests.push(newRequest);
	free(currentEntry->packet);
	free(currentEntry);
}

void Orange::respondToBlueRequest(Orange* orange, Token* token)
{
	Packet* answer;
	
	/*Un nodo del grafo no puede no tener vecinos.*/
	assert(orange->blue_graph[token->node].size() > 0);
	
	/*Para cada vecino que tenga, toma acciones distintas dependiendo de si el vecino ya se instanció o no.*/
	for(auto neighbor : orange->blue_graph[token->node]){
		if(orange->blueMapping[neighbor].first != 0){	//si el vecino está instanciado
			//toma la dirección ip y puerto del vecino, los mete en un paquete de respuesta y lo manda
			answer = (BOGraphPosition_N*) calloc(1, sizeof(BOGraphPosition_N));
			answer->id = ID::BOJOIN_GRAPH;
			((BOGraphPosition_N*)answer)->nodeID = token->node;
			((BOGraphPosition_N*)answer)->neighborID = neighbor;
			((BOGraphPosition_N*)answer)->neighborIP = orange->blueMapping[neighbor].first;
			((BOGraphPosition_N*)answer)->neighborPort = orange->blueMapping[neighbor].second;	
		}else{ //si el vecino no está instanciado, solo toma la id del nodo y de su vecino, y las manda
			answer = (BOGraphPosition_E*) calloc(1, sizeof(BOGraphPosition_E));
			answer->id = ID::BOJOIN_GRAPH;
			((BOGraphPosition_E*)answer)->nodeID = token->node;
			((BOGraphPosition_E*)answer)->neighborID = neighbor;
		}
		orange->putInSendQueue(orange, answer, NODE_BLUE, token->assignedIp, token->assignedPort);
	}
}

void Orange::initBlueMap()
{
	for(auto node : this->blue_graph){
		/*Llena el mapa secundario con id de nodos, sin ip ni puerto asociados por el momento.*/
		this->blueMapping[(unsigned short) node.first] = make_pair(0, 0);
	}
}

int Orange::findNextUnassigned(Orange* orange)
{
	for(auto node : orange->blueMapping)
		if(node.second.first == 0 && node.second.second == 0)
			return node.first;
	
	return -1;
}

unsigned long Orange::findMinIP()
{
	unsigned int min = std::numeric_limits<int>::max();
	if(!this->allNodesIP.empty()){
		for(auto ipAddr : this->allNodesIP)
			if(ipAddr < min)
				min = ipAddr;
	}
	return min;
}

char* Orange::getIP()
{
	return this->myIP;
}

int main(int argc, char* argv[]){
	if(argc < 2)
		return (cout << "Usage: " << argv[0] << " <num_oranges>" << endl), 0;
	int numOranges = atoi(argv[1]);
    int id = 0;
    unsigned short int orangeInPort = ORANGE_PORT;
    unsigned short int orangeOutPort = ORANGE_PORT;
    //get_args(id, orangeInPort, orangeOutPort, argc, argv);
    Orange orangeNode(id, orangeInPort, orangeOutPort, numOranges);
    
    pthread_t receiverOranges;
    pthread_t receiverBlues;
    pthread_t processer;
    pthread_t sender;
    
    OrangeArgs args1;
    OrangeArgs args2;
    
    args1.node = &orangeNode;
    args1.commWith = COMM_ORANGE;
    args2.node = &orangeNode;
	args2.commWith = COMM_BLUE;

	pthread_create(&receiverOranges, NULL, &Orange::receiverHelper, &args1);
	pthread_create(&receiverBlues, NULL, &Orange::receiverHelper, &args2);
	
	orangeNode.print_graph();
	
    orangeNode.requestIP();
	Socket::getHostIP(orangeNode.getIP());

    pthread_create(&processer, NULL, &Orange::processerHelper, &orangeNode);
	
	pthread_create(&sender, NULL, &Orange::senderHelper, &orangeNode);
	
	/*Nunca hacen exit*/
	pthread_join(receiverOranges, (void**) nullptr);
	pthread_join(receiverBlues, (void**) nullptr);
	pthread_join(processer, (void**) nullptr);
	pthread_join(sender, (void**) nullptr);
	
}
