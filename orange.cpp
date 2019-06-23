#include "orange.h"

#include <cassert>
#include <iostream>
#include <string>
#include <pthread.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <thread>

using namespace std;

Orange::Orange(){
}

Orange::Orange(int id, unsigned short int orangeInPort, unsigned short int orangeOutPort, int totalOranges)
: id(id), orangeInPort(orangeInPort), orangeOutPort(orangeOutPort), numTotalOranges(totalOranges), ipBuffer(nullptr)
{
	sem_init(&this->InBufferSem, 0, 0);
	sem_init(&this->OutBufferSem, 0, 0);
	pthread_mutex_init(&this->semIn, nullptr);
	pthread_mutex_init(&this->semOut, nullptr);
	this->orangeSocket = new Socket(Socket::Protocol::UDP);
	this->blueSocket = new Socket(Socket::Protocol::UDP);
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
    memset(buffer, 0, BUF_SIZE);
    Packet* currentPacket;
    int status = 0;
    
    
    while(true){
        /*Lee el socket */
        orange->orangeSocket->Recvfrom(buffer, BUF_SIZE, ORANGE_PORT);
        
        /*Transforma la tira de bytes en un paquete*/
        currentPacket = coder.decode(buffer);
		
		/*Mete el paquete a la cola privada*/
		orange->privateInBuffer.push(currentPacket);
		
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
		
		//free(currentPacket);
		
    }
}

void *Orange::receiverHelper(void *context){
    return ((Orange *)context)->receiver((Orange*) context);
}

///Funcion para el thread que toma un paquete de la cola compartida, lo procesa y pone mensajes en la cola compartida para el siguiente thread
void *Orange::processer(Orange* orange){
	beginContention(orange);
	
    while(true){
        /*Lee la cola compartida, saca el paquete del frente y lo procesa */
        
        sem_wait(&orange->InBufferSem);	//Espera a que haya algo en la cola para procesar
        pthread_mutex_lock(&orange->semIn);
        
        Packet* packet = orange->sharedInBuffer.front();
        orange->sharedInBuffer.pop();
        
        pthread_mutex_unlock(&orange->semIn);

		/*Hace las diferentes acciones con los paquetes */
		switch(packet->id){
			case ID::INITIAL_TOKEN:
				char buffer[IP_LEN];
				orange->orangeSocket->decode_ip((static_cast<InitialToken*>(packet))->ip, buffer);
				cout << "Node says: " << buffer << endl;
			break;
		}
        
        /*Packet packetOut; //Paquete que se crea dentro del switch dependiendo de lo que pida el paquete entrante. Vacio mientras se hacen pruebas
        pthread_mutex_lock(&semOut);
        sharedOutBuffer.push(packetOut);
        pthread_mutex_unlock(&semOut);
        sem_post(&this->OutBufferSem);	//le avisa al sender que hay algo para enviar
        */

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
			Packet* toSend = orange->privateOutBuffer.front();
			orange->privateOutBuffer.pop();
			rawPacket = coder.encode(toSend);
			assert(rawPacket);
			orange->orangeSocket->Sendto(rawPacket, sizeof(rawPacket), orange->rightIP, ORANGE_PORT);
            /*Lo envia con el socket */
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

void Orange::beginContention(Orange* orange)
{
	InitialToken* p = (InitialToken*) calloc(1, sizeof(InitialToken));
	p->id = ID::INITIAL_TOKEN;
	p->ip = orange->orangeSocket->encode_ip(orange->myIP);
	
	pthread_mutex_lock(&orange->semOut);
	orange->privateOutBuffer.push(p);
	pthread_mutex_unlock(&orange->semOut);
	sem_post(&orange->OutBufferSem);
}

int main(int argc, char* argv[]){
    int id;
    unsigned short int orangeInPort;
    unsigned short int orangeOutPort;
    //get_args(id, orangeInPort, orangeOutPort, argc, argv);
    Orange orangeNode(id, orangeInPort, orangeOutPort, 2);
    
    pthread_t receiver;
    pthread_t processer;
    pthread_t sender;
	
    orangeNode.requestIP();
	orangeNode.getHostIP();

    int resProcesser = pthread_create(&processer, NULL, &Orange::processerHelper, &orangeNode);
    int resReceiver = pthread_create(&receiver, NULL, &Orange::receiverHelper, &orangeNode);
    int resSender = pthread_create(&sender, NULL, &Orange::senderHelper, &orangeNode);
	
	/*Nunca hacen exit*/
	pthread_join(receiver, (void**) nullptr);
	pthread_join(processer, (void**) nullptr);
	pthread_join(sender, (void**) nullptr);
}
