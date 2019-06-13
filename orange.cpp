#include "orange.h"
#include "socket.h"


#include <iostream>
#include <string>
#include <pthread.h>

using namespace std;

Orange::Orange(){
}

Orange::Orange(int id, unsigned short int orangeInPort, unsigned short int orangeOutPort){
    this->id = id;
    this->orangeInPort = orangeInPort;
    this->orangeOutPort = orangeOutPort;
}

Orange::~Orange(){
}

void Orange::requestIP(){
    cout <<"Una vez que todos los nodos naranjas estén corriendo, ingrese el número IP del vecino izquierdo"<<endl;
    cin >> this->leftIP;

    cout <<"Ingrese el número IP del vecino derecho"<<endl;
    cin >> this->rightIP;
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

            if((id = stoi(argv[1])) == 0 || (orangeInPort = (unsigned short) stoi(argv[2], NULL, 0)) == 0  || (orangeOutPort = (unsigned short) stoi(argv[3], NULL, 0)) == 0){ // Si el argumento no es un número válido el programa se cae aquí
				perror("Invalid argument!\n");
				printf("Format: %s <port>\n", argv[0]);
				exit(EXIT_FAILURE);
            }
    }
}

///Funcion para el thread que recibe paquetes del socket y los pone en la cola compartida para el siguiente thread
void *Orange::reciver(){
    /*Crea el socket */
    while(1){
        /*Lee el socket */
        Packet packet();    //Paquete que se recibe por el socket. Vacio mientras se hacen pruebas

    }
}

void *Orange::reciverHelper(void *context){
    return ((Orange *)context)->reciver();
}

///Funcion para el thread que toma un paquete de la cola compartida, lo procesa y pone mensajes en la cola compartida para el siguiente thread
void *Orange::processer(){
    while(true){
        /*Lee la cola compartida, saca el paquete del frente y lo procesa */
        pthread_mutex_lock(&semIn);
        if(!sharedInBuffer.empty()){
            Packet packet = sharedInBuffer.front();
            sharedInBuffer.pop();

            switch(packet.id){
                /*Hace las diferentes acciones con los paquetes */
            }

        }
        pthread_mutex_unlock(&semIn);
        
        Packet packetOut; //Paquete que se crea dentro del switch dependiendo de lo que pida el paquete entrante. Vacio mientras se hacen pruebas
        pthread_mutex_lock(&semOut);
        sharedOutBuffer.push(packetOut);
        pthread_mutex_unlock(&semOut);

    }
}

void *Orange::processerHelper(void *context){
    return ((Orange *)context)->sender();
}

///Funcion que toma paquetes de la cola compartida y los envia por el socket
void *Orange::sender(){
    
    while(true){
        if(!privateOutBuffer.empty()){
            /*Lo envia con el socket */
        }
        else{                                       //Si la cola privada está vacía, busca en la cola compartida
            pthread_mutex_lock(&semOut);
            if(!sharedOutBuffer.empty()){
                privateOutBuffer.push(sharedOutBuffer.front());
                sharedOutBuffer.pop();
            }
            pthread_mutex_unlock(&semOut);
        }
    }
}

void *Orange::senderHelper(void *context){
    return ((Orange *)context)->sender();
}

int main(int argc, char* argv[]){
    int id;
    unsigned short int orangeInPort;
    unsigned short int orangeOutPort;
    get_args(id, orangeInPort, orangeOutPort, argc, argv);
    Orange orangeNode(id, orangeInPort, orangeOutPort);
    
    pthread_t reciver;
    pthread_t processer;
    pthread_t sender;

    int resReciver = pthread_create(&reciver, NULL, &Orange::reciverHelper, &orangeNode);
    int resProcesser = pthread_create(&processer, NULL, &Orange::processerHelper, &orangeNode);
    int resSender = pthread_create(&sender, NULL, &Orange::senderHelper, &orangeNode);
    
    orangeNode.requestIP();

}