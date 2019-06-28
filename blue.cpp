#include <semaphore.h>
#include "blue.h"
 
Blue::~Blue()
{
    sem_destroy(&this->InBufferSem);
	sem_destroy(&this->OutBufferSem);
}
/** \brief Agrega vecinos al mapa y sus ip al vector
 *
 * \param Ip de vecinos
 * \param Puerto de vecinos
 * \return
 *
 */

void *Blue::receiver(Blue* blue){
}

void *Blue::processer(Blue* blue){
}

void *Blue::sender(Blue* blue){
}

void *Blue::senderHelper(void *context){
    return ((Blue *)context)->sender((Blue*) context);
}

void *Blue::processerHelper(void *context){
    return ((Blue *)context)->processer((Blue*) context);
}

void *Blue::receiverHelper(void *context){
    return ((Blue *)context)->receiver((Blue*) context);
}

void Blue::neighbors(unsigned int IP,unsigned short int puerto){
   //Mapa de puerto con ip de cada vecino
   mapNeighbors.insert(mapNeighbors.begin(),pair<unsigned int, unsigned short int>(IP, puerto));
   //vector que guarda solo ip de vecinos para iterar sobre estos para el round robin
   ports_Neighbors.push_back(IP);

}

/** \brief Round robin
 *
 * \param El chunk
 * \param
 * \return
 *
 */

int validateIP(char* ip)
{
	char ipBuffer[IP_LEN];
	return (inet_pton(AF_INET, ip, ipBuffer) == 1 ? 1 : 0);
}

///Funcion que verifica la cantidad de datos por consola y los convierte de strings a los datos de la clase Orange
void get_args(char* &orangeIP, unsigned short int &orangePort, int argc, char ** argv){
    switch(argc){
		case 1:
        case 2:
			perror("Número de argumentos menor al requerido\n");
			printf("Formato: %s <ip nodo naranja para solicitar asignacion> <puerto nodo naranja para solicitar asignacion>\n", argv[0]);
            exit(EXIT_FAILURE);
        default:

            if((orangeIP = argv[1]) == 0 || (orangePort = (unsigned short) stoi(argv[2], NULL)) == 0 || (validateIP(orangeIP) == 0)){ // Si el argumento no es un número válido el programa se cae aquí
				perror("Invalid argument!\n");
				printf("Format: %s ip nodo naranja para solicitar asignacion> <puerto nodo naranja para solicitar asignacion>\n", argv[0]);
				exit(EXIT_FAILURE);
            }
    }
}

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
	pthread_mutex_lock(&blue->semOut);
	blue->privateOutBuffer.push(newPacket);
	pthread_mutex_unlock(&blue->semOut);
	sem_post(&blue->OutBufferSem);
}

Blue::Blue(char* orangeIP, int orangePort)
{
    sem_init(&this->InBufferSem, 0, 0);
	sem_init(&this->OutBufferSem, 0, 0);
	pthread_mutex_init(&this->semIn, nullptr);
	pthread_mutex_init(&this->semOut, nullptr);

    this->orangeIP = orangeIP;
    this->orangePort = orangePort;
}

int main(int argc, char* argv[]){
    pthread_t receiver;
    pthread_t processer;
    pthread_t sender;
    
    unsigned short int orangePort;
    char* orangeIP;
     
    get_args(orangeIP, orangePort, argc, argv);

    Blue blueNode(orangeIP,orangePort);
/* 
    int resReceiver = pthread_create(&receiver, NULL, &Blue::receiverHelper, &blueNode);

*/

    /*Nunca hacen exit
	pthread_join(receiver, (void**) nullptr);
	pthread_join(processer, (void**) nullptr);
	pthread_join(sender, (void**) nullptr);*/
}