#ifndef CODE_H
#define CODE_H
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "packet.h"
#include "orangePacket.h"
#include "greenBlue.h"
#include "bluePacket.h"
#include "bluePacket.h"
#include "BlueOrange.h"

class Code
{
    public:
        Code();
        ~Code();
        char* encode(Packet *, int type);
        Packet* decode(char *, char = PACKET_ORANGE);
        static size_t findPacketLen(Packet* p);
    protected:
    private:
};

#endif // CODE_H
