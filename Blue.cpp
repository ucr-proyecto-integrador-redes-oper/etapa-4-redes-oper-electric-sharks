#include "Blue.h"

Blue::Blue()
{
    i=mapNeighbors.begin();//inicializamos el iterador del mapa de vecinos
}

Blue::~Blue()
{
    //dtor
}

void Blue::neighbors(unsigned int IP,unsigned short int puerto){

   mapNeighbors.insert(mapNeighbors.begin(),pair<unsigned int, unsigned short int>(IP, puerto));

}

void Blue::sendChunk(char chunk[1024]){
    if(i!=mapNeighbors.end()){
      //Envia el chunk al vecino al que este apuntando a ese momento
      ++i;
    }else{
      i=mapNeighbors.begin();
      //si tiene espacio se deja el chunk porque ya lo envió a los vecinos
    }
}
