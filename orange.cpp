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
    string leftIPStr;
    string rightIPStr;

    cout <<"Una vez que todos los nodos naranjas estén corriendo, ingrese el número IP del vecino izquierdo"<<endl;
    cin >> leftIPStr;

    cout <<"Ingrese el número IP del vecino derecho"<<endl;
    cin >> rightIPStr;
}

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

void *Orange::reciver(){
    Socket socketIn(Socket::Protocol::TCP,false);
    Socket * conn_sock;
    
    socketIn.Bind((long)orangeInPort);
    socketIn.Listen();
    conn_sock = socketIn.Accept();
    printf("%d",id);

    while(1){
        printf("Nani?");
    }
}

void *Orange::reciverHelper(void *context){
    return ((Orange *)context)->reciver();
}

void *Orange::processer(){
}

void *Orange::processerHelper(void *context){
    return ((Orange *)context)->sender();
}

void *Orange::sender(){
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