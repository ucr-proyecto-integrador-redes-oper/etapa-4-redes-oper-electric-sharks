#include "orange.h"
#include <iostream>

using namespace std;

Orange::Orange(){}

Orange::Orange(unsigned short int orangeInPort, unsigned short int orangeOutPort){
    this->orangeInPort = orangeInPort;
    this->orangeOutPort = orangeOutPort;
}

Orange::~Orange(){
}

void requestIp(){
    string leftIPStr;
    string rightIPStr;

    cout <<"Ingrese el número IP del vecino izquierdo"<<endl;
    cin >> leftIPStr;

    cout <<"Ingrese el número IP del vecino derecho"<<endl;
    cin >> rightIPStr;
}

