#ifndef ORANGE_H
#define ORANGE_H
#endif

#include "packet.h"
#include <pthread.h>

class Orange{
    private:
        unsigned short int orangeInPort;
        unsigned short int orangeOutPort;
        unsigned short int bluePort;
        Packet* privateInBuffer;
        Packet* privateOutBuffer;
        Packet* sharedInBuffer;
        Packet* sharedOutBuffer;
        pthread_mutex_t semIn;
        pthread_mutex_t semOut;

        void * reciver();
        void * processer();
        void * sender();

    public:
        unsigned int leftIP;
        unsigned int rightIP;
        int id;

        Orange();
        ~Orange();
        Orange(int, unsigned short int, unsigned short int);
        void requestIP();
        static void * reciverHelper(void *context);
        static void * processerHelper(void *context);
        static void * senderHelper(void *context);
        void init();
};

