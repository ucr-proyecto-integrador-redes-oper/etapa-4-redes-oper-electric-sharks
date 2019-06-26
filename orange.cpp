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
	pthread_mutex_init(&this->semIn, nullptr);
	pthread_mutex_init(&this->semOut, nullptr);
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
    cout <<"Una vez que todos los nodos naranjas estén corriendo, ingrese el número IP del vecino izquierdo"<<endl;
    bool validIp = true;
	do{
		cin >> this->leftIP;
		validIp = (bool) validateIP(this->leftIP);
		if(!validIp)
			cout << "Direccion IP incorrecta! Ingrese una direccion valida: " << endl;
	}while(!validIp);
	
    cout <<"Ingrese el número IP del vecino derecho"<<endl;
    do{
		cin >> this->rightIP;
		validIp = (bool) validateIP(this->rightIP);
		if(!validIp)
			cout << "Direccion IP incorrecta! Ingrese una direccion valida: " << endl;
	}while(!validIp);
}

int Orange::validateIP(char* ip)
{
	char ipBuffer[IP_LEN];
	return (inet_pton(AF_INET, ip, ipBuffer) == 1 ? 1 : 0);
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
void *Orange::receiver(Orange* orange){
    /*Crea el socket */
    char* buffer = new char[BUF_SIZE];
    char senderBuffer[IP_LEN];
    memset(buffer, 0, BUF_SIZE);
    Packet* currentPacket;
    PacketEntry* currentEntry = nullptr;
    struct sockaddr_in senderAddr;
    int status = 0;
    
    
    while(true){
        /*Lee el socket */
        currentEntry = (PacketEntry*) calloc(1, sizeof(PacketEntry));
        
        orange->orangeSocket->Recvfrom(buffer, BUF_SIZE, ORANGE_PORT, &senderAddr);
        
        /*Transforma la tira de bytes en un paquete*/
        currentPacket = coder.decode(buffer);
        assert(currentPacket);
		
		orange->orangeSocket->decode_ip(senderAddr.sin_addr.s_addr, senderBuffer);
		
		currentEntry->packet = currentPacket;
		currentEntry->receivedFromLeft = (strcmp(buffer, orange->leftIP) == 0 ? true : false);
		
		/*Mete el paquete a la cola privada*/
		orange->privateInBuffer.push(currentEntry);
		
		memset(buffer, 0, BUF_SIZE);
		
		/*Si la cola compartida está siendo usada, sigue escuchando el socket, sino mete paquetes a la cola compartida*/
		status = pthread_mutex_trylock(&orange->semIn);
		
		if(status == EBUSY)
			continue;
			
		while(!orange->privateInBuffer.empty()){
			orange->sharedInBuffer.push(orange->privateInBuffer.front());
			orange->privateInBuffer.pop();
		}
		
		pthread_mutex_unlock(&orange->semIn);
		
		/*Avisa al processer que hay más paquetes para procesar con un signal*/
		sem_post(&orange->InBufferSem);
		
    }
}

void *Orange::receiverHelper(void *context){
    return ((Orange *)context)->receiver((Orange*) context);
}

///Funcion para el thread que toma un paquete de la cola compartida, lo procesa y pone mensajes en la cola compartida para el siguiente thread
void *Orange::processer(Orange* orange){
	PacketEntry* currentEntry;
	
	orange->beginContention();
	
    while(true){
        /*Lee la cola compartida, saca el paquete del frente y lo procesa */
        
        sem_wait(&orange->InBufferSem);	//Espera a que haya algo en la cola para procesar
        pthread_mutex_lock(&orange->semIn);
        assert(!orange->sharedInBuffer.empty());
        currentEntry = orange->sharedInBuffer.front();
        orange->sharedInBuffer.pop();
        
        pthread_mutex_unlock(&orange->semIn);
		assert(currentEntry);
		assert(currentEntry->packet);
		/*Hace las diferentes acciones con los paquetes */
		switch(currentEntry->packet->id){
			case ID::INITIAL_TOKEN:
				orange->processInitialToken(currentEntry);
			break;
			
			case ID::TOKEN_EMPTY:
				//assert(false);
				Token* token = (Token*)currentEntry->packet;
				if(token->boolean){	//si está ocupado
					
					//si fue el mismo quien lo ocupó, lo libera
					if(orange->tokenOccupied){
						orange->tokenOccupied = false;
						token->boolean = false;
						token->assignedIp = 0;
						token->assignedPort = 0;
					}else{
					//sino, anota la asignacion en su tabla local
						auto node = orange->blueMapping.find(token->node);
		
						if(node != orange->blueMapping.end())
							orange->blueMapping[token->node] = make_pair(token->assignedIp, token->assignedPort);
						else
							assert(false); //no debe ocurrir;
					}
				}else{
					//si el token está libre, y tiene asignaciones pendientes
					if(!orange->blueRequests.empty()){
						//asigna un azul a un nodo del grafo
						unsigned short int newAssignment = orange->findNextUnassigned(orange);
						if(newAssignment == -1){
							error_exit(-1, "Error! No se pueden asignar más nodos del grafo!\n");
						}
						/*BlueRequest* blueRequest = orange->blueRequests.front(); //hay que crearlo, no existe
						orange->blueRequests.pop();
						token->node = newAssignment;
						token->assignedIp = blueRequest->ip;
						token->assignedPort = blueRequest->port;
						token->boolean = true;
						orange->tokenOccupied = true;*/
					}
					//sino, solo pasa el token al vecino derecho
				}
				if(orange->tokenCreated){
					token->assignedPort = 0;
					token->assignedIp = 37;
				}else{
					token->assignedPort++;
				}
				cout << "yo cree el token? " << std::boolalpha << orange->tokenCreated << endl;
				cout << "recibí el token # "<< token->assignedPort << " de " << orange->leftIP << endl;
				cout << "pasando el token a " << orange->rightIP << endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(2000));
				orange->putInSendQueue(orange, token, SEND_TO_RIGHT);
		}
    }
}

void *Orange::processerHelper(void *context){
    return ((Orange *)context)->processer((Orange*) context);
}

///Funcion que toma paquetes de la cola compartida y los envia por el socket
void *Orange::sender(Orange* orange){
    char* rawPacket = nullptr;
    char testBuf[IP_LEN];
    
    while(true){
        if(!orange->privateOutBuffer.empty()){
			PacketEntry* currentEntry = nullptr;
			currentEntry = orange->privateOutBuffer.front();
			assert(currentEntry);
			Packet* toSend = currentEntry->packet;
			orange->privateOutBuffer.pop();
			rawPacket = coder.encode(toSend);
			assert(rawPacket);
			size_t packetLen = 0;
			packetLen = findPacketLen(toSend);
			cout << "largo del paquete con strlen: " << packetLen << endl;
			switch(currentEntry->sendTo){
				case SEND_TO_RIGHT:
					orange->orangeSocket->Sendto(rawPacket, packetLen, orange->rightIP, ORANGE_PORT);
				break;
				
				case SEND_TO_LEFT:
					orange->orangeSocket->Sendto(rawPacket, packetLen, orange->leftIP, ORANGE_PORT);
				break;
				
				case SEND_TO_BOTH:
					orange->orangeSocket->Sendto(rawPacket, packetLen, orange->rightIP, ORANGE_PORT);
					orange->orangeSocket->Sendto(rawPacket, packetLen, orange->leftIP, ORANGE_PORT);
				break;
				
				default:
					cout << "Unknown direction for packet!" << endl;
			}
			
			free(toSend);
            free(currentEntry);
        }
        else{                                       //Si la cola privada está vacía, busca en la cola compartida
			sem_wait(&orange->OutBufferSem);
            pthread_mutex_lock(&orange->semOut);
            if(!orange->sharedOutBuffer.empty()){
                orange->privateOutBuffer.push(orange->sharedOutBuffer.front());
                orange->sharedOutBuffer.pop();
            }
            pthread_mutex_unlock(&orange->semOut);
        }
    }
}

void *Orange::senderHelper(void *context){
    return ((Orange *)context)->sender((Orange*) context);
}

void Orange::getHostIP()
{
	struct ifaddrs *ifaddr, *ifa;
    int family, s;
    char host[NI_MAXHOST];
    memset(host, 0, NI_MAXHOST);
    memset(this->myIP, 0, IP_LEN);
    
    string interfaces[NUM_INTERFACES];
    
    /*Posibles interfaces de las que se puede obtener la IP del host.*/
    interfaces[0] = string("eno1");
    interfaces[1] = string("wlo1");
    interfaces[2] = string("eth0");
    interfaces[3] = string("enp1s0");

    if (getifaddrs(&ifaddr) == -1) 
    {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

	for(int interface = 0; interface < NUM_INTERFACES; ++interface){
		for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
			if (ifa->ifa_addr == NULL){
				continue;  
			}

			s = getnameinfo(ifa->ifa_addr,sizeof(struct sockaddr_in),host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

			if((strcmp(ifa->ifa_name, interfaces[interface].c_str())==0)){
				/*Si la direccion es valida y no es loopback, se asigna al nodo naranja*/
				if(validateIP(host) && strcmp("127.0.0.1", host)){
					memcpy(this->myIP, host, IP_LEN);
					cout << "Dirección IP: " << this->myIP << ", interfaz: " << ifa->ifa_name << endl;
					break;
				}
			}
		}
	}

    freeifaddrs(ifaddr);
    if(!validateIP(this->myIP)){
		error_exit(EXIT_FAILURE, "No se pudo obtener la IP del host!\n");
	}
}

void Orange::beginContention()
{
	InitialToken* p = (InitialToken*) calloc(1, sizeof(InitialToken));
	p->id = ID::INITIAL_TOKEN;
	p->ip = this->orangeSocket->encode_ip(this->myIP);
	this->putInSendQueue(this, p, SEND_TO_BOTH);
}

void Orange::createToken(Orange* orange)
{
	Packet* token = (Token*) calloc(1, sizeof(Token));
	token->id = ID::TOKEN_EMPTY;
	((Token*)token)->assignedPort = 0;
	((Token*)token)->boolean = false;
	orange->tokenCreated = true;
	orange->putInSendQueue(orange, token, SEND_TO_RIGHT);
}

void Orange::putInSendQueue(Orange* orange, Packet* p, int direction)
{	
	PacketEntry* newPacket = (PacketEntry*) calloc(1, sizeof(PacketEntry));
	newPacket->packet = p;
	newPacket->sendTo = direction;
	pthread_mutex_lock(&orange->semOut);
	orange->privateOutBuffer.push(newPacket);
	pthread_mutex_unlock(&orange->semOut);
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
		currentEntry->sendTo = (currentEntry->receivedFromLeft ? SEND_TO_RIGHT : SEND_TO_LEFT);
		this->putInSendQueue(this, currentEntry->packet, currentEntry->sendTo);
		free(currentEntry);
	}else{
		/*Si el paquete que recibió lo creó este nodo, lo bota de la red.*/
		free(currentEntry->packet);
		free(currentEntry);
	}
}

size_t Orange::findPacketLen(Packet* p)
{
	switch((unsigned int) p->id){
		case ID::INITIAL_TOKEN:
			return sizeof(InitialToken);
		break;
		
		case ID::TOKEN_EMPTY:
		case ID::TOKEN_FULL_AND_COMPLETE:
		case ID::TOKEN_FULL_AND_REQUEST:
			return sizeof(Token);
		break;
		case ID::BCHUNK:
		    return sizeof(BChunk);
		    break;
		case ID::BHELLO:
		    return sizeof(BHello);
		    break;
		case ID::BEXIST:
		    return sizeof(BExist);
		    break;
		case ID::BCOMPLETE:
		    return sizeof(BComplete);
		    break;
		case ID::BGET:
		    return sizeof(BGet);
		    break;
		case ID::BLOCALIZE:
		    return sizeof(BLocalize);
		    break;
		case ID::BDELETE:
		    return sizeof(BDelete);
		    break;
		case ID::ASSIGNMENT:
		    return sizeof(Assignment);
		    break;
		case ID::CONNECT:
		    return sizeof(Connect);
		    break;
		case ID::GO:
		    return sizeof(Go);
		    break;
		case ID::GCHUNK:
		    return sizeof(GChunk);
		    break;
		case ID::GEXIST:
		    return sizeof(GExist);
		    break;
		case ID::GCOMPLETE:
		    return sizeof(GComplete);
		    break;
		case ID::GDELETE:
		    return sizeof(GDelete);
		    break;
		case ID::GLOCALIZE:
		    return sizeof(GLocalize);
		    break;
		case ID::GKILL:
		    return sizeof(GKill);
		    break;
		default:
			assert(false);
	}
	return -1;
}

void Orange::initBlueMap()
{
	for(auto node : this->blue_graph){
		cout << "node: " << node.first << endl;
		this->blueMapping[(unsigned short) node.first] = make_pair(0, 0);
	}
}

unsigned short int Orange::findNextUnassigned(Orange* orange)
{
	unsigned short int nextFree = -1;
	for(auto node : orange->blueMapping)
		if(node.second.first == 0 && node.second.second == 0)
			nextFree = node.first;
	
	return nextFree;
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

int main(int argc, char* argv[]){
    int id;
    unsigned short int orangeInPort;
    unsigned short int orangeOutPort;
    //get_args(id, orangeInPort, orangeOutPort, argc, argv);
    Orange orangeNode(id, orangeInPort, orangeOutPort, NUM_ORANGES);
    
    pthread_t receiver;
    pthread_t processer;
    pthread_t sender;

	int resReceiver = pthread_create(&receiver, NULL, &Orange::receiverHelper, &orangeNode);
	
	orangeNode.print_graph();
	
    orangeNode.requestIP();
	orangeNode.getHostIP();

    int resProcesser = pthread_create(&processer, NULL, &Orange::processerHelper, &orangeNode);
    int resSender = pthread_create(&sender, NULL, &Orange::senderHelper, &orangeNode);
	
	/*Nunca hacen exit*/
	pthread_join(receiver, (void**) nullptr);
	pthread_join(processer, (void**) nullptr);
	pthread_join(sender, (void**) nullptr);
}
