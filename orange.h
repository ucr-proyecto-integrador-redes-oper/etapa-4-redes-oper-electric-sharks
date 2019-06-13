#ifndef ORANGE_H
#define ORANGE_H
#endif

#include "packet.h"
#include <pthread.h>

class Orange{
    unsigned int leftIP;
    unsigned int rightIP;
    int id;
    unsigned short int orangeInPort;
    unsigned short int orangeOutPort;
    unsigned short int bluePort;
    Packet* privateInBuffer;
    Packet* privateOutBuffer;
    Packet* sharedInBuffer;
    Packet* sharedOutBuffer;
    pthread_mutex_t semIn;
    pthread_mutex_t semOut;

    Orange();
    ~Orange();
    Orange(unsigned short int, unsigned short int);
    void requestIP();
    void reciver();
    void sender();
    void reciver();
    void init();
};

