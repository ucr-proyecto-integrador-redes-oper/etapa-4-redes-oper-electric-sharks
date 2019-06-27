#ifndef ORANGE_H
#define ORANGE_H


#include "packet.h"
#include "socket.h"
#include "encoder.h"
#include "error_handler.h"
#include "BlueOrange.h"

#include <pthread.h>
#include <queue>
#include <map>

using namespace std;

class Blue{
    public:
        queue<PacketEntry*> privateInBuffer;
        queue<PacketEntry*> privateOutBuffer;
        queue<PacketEntry*> sharedInBuffer;
        queue<PacketEntry*> sharedOutBuffer;
        queue<PacketEntry*> blueRequests;
    
        map <unsigned int, unsigned short int> mapNeighbors;
        //map <unsigned int, unsigned short int>::iterator i;
        vector<unsigned int> ports_Neighbors;
        Blue();
        virtual ~Blue();
        void neighbors(unsigned int,unsigned short int);
        void sendChunk(char []);

    protected:
    private:
};

#endif