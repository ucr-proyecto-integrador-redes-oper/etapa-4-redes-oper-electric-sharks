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
        virtual ~Code();
        char* encode(Packet *);
        Packet* decode(char *);
    protected:
    private:
};

#endif // CODE_H
