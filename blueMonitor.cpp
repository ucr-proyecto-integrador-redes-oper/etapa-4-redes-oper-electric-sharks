#include "message.h"
#include <iostream>

int main(){
	Message msgq;
	struct my_msgbuf message;
	long nodeSeries;
	int option;
	while(true){
		std::cout << "Escriba un numero de serie de nodo: ";
		std::cin >> nodeSeries;
		std::cout << "Escoja una opcion: " << std::endl;
		std::cout << "1. Averiguar el numero de nodo" << std::endl;
		std::cout << "2. Averiguar vecinos del nodo" << std::endl;
		std::cin >> option;

		message.question = (uint8_t) option;
		message.mtype = nodeSeries;
		msgq.send(&message);
		msgq.receive(&message, 9999);
		switch(option){
			case 1:
				std::cout << "El numero de nodo es: " << message.nodeID << std::endl;
				break;
			case 2:
				std::cout << message.message << std::endl;
				break;
			default:
				std::cout << "Not quite there yet " << std::endl;
				break;
		}
	}
	return 0;
}
