#ifndef BLUE_H
#define BLUE_H
#include <iostream>
#include <utility>
#include <map>

using namespace std;
class Blue
{
    public:
        map <unsigned int, unsigned short int> mapNeighbors;
        map <unsigned int, unsigned short int>::iterator i;
        Blue();
        virtual ~Blue();
        void neighbors(unsigned int,unsigned short int);
        void sendChunk(char []);
    protected:
    private:
};

#endif // BLUE_H
