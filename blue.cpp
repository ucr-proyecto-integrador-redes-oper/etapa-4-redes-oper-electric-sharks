
#include "blue.h"

Blue::Blue()
{

}

Blue::~Blue()
{
    //dtor
}
/** \brief Agrega vecinos al mapa y sus ip al vector
 *
 * \param Ip de vecinos
 * \param Puerto de vecinos
 * \return
 *
 */

void Blue::neighbors(unsigned int IP,unsigned short int puerto){
   //Mapa de puerto con ip de cada vecino
   mapNeighbors.insert(mapNeighbors.begin(),pair<unsigned int, unsigned short int>(IP, puerto));
   //vector que guarda solo ip de vecinos para iterar sobre estos para el round robin
   ports_Neighbors.push_back(IP);

}

/** \brief Round robin
 *
 * \param El chunk
 * \param
 * \return
 *
 */

void Blue::sendChunk(char chunk[1024]){
    int num_Neighbors = ports_Neighbors.size();
    int pointer_vector = 0;

    if(pointer_vector < num_Neighbors{
      //Envia el chunk al vecino al que este apuntando a ese momento a ip y puerto del mapa
      ++pointer_vector;
    }else{
      pointer_vector = 0;
      //si tiene espacio se deja el chunk porque ya lo envió a todos los vecinos
    }
}