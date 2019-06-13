#ifndef ORANGE_H
#define ORANGE_H
#endif

#include "packet.h"
#include <pthread.h>
#include <queue>

using namespace std;

class Orange{
    private:
        unsigned short int bluePort;

        queue<Packet> privateInBuffer;
        queue<Packet> privateOutBuffer;
        queue<Packet> sharedInBuffer;
        queue<Packet> sharedOutBuffer;

        pthread_mutex_t semIn;
        pthread_mutex_t semOut;

        void * reciver();
        void * processer();
        void * sender();

    public:
        unsigned short int orangeInPort;
        unsigned short int orangeOutPort;
        char leftIP[15];
        char rightIP[15];
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

