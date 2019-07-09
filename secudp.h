#ifndef REUDP
#define REUDP

#include <cstdint>
#include <cstdlib>
#include <ctime>

#include <unordered_map>
#include <queue>
#include <utility>

#include <arpa/inet.h>
#include <sys/types.h>

#include "socket.h"
#include "semaphore.h"
#include "bluePacket.h"

#define PAYLOAD_SIZE sizeof(BChunk)
#define WAIT_TIME 50

		/* Se tiene que utilizar este macro 
		Para asegurarse de que entre los campos del frame
		no haya padding */
struct __attribute__((__packed__)) data_frame{
	uint8_t type; /* Tipo del frame (de datos o de ACK) */
	uint16_t sn; /* SN del emisor */
	char payload[PAYLOAD_SIZE]; /* El payload del frame */
};

struct direction{
	struct in_addr addr; /* Struct que contiene a la IP en formato binario */
	uint16_t port; /* Puerto del socket */
};

struct map_entry{
	struct direction * direc; /* Puntero a struct que contiene la IP y el puerto del otro host */
	struct data_frame * frame; /* Puntero al frame */
	size_t len; /* Cantidad de bytes que tiene el payload del frame */
	bool received; /* Indica si ya se recibio el ACK */
};

class reUDP{
	private:
		uint16_t sn; /* SN actual */
		Socket sock; /* Socket que se utilizara */
		std::unordered_map<uint16_t, struct map_entry *> sent; /* Mapa de mensajes enviados que utiliza el SN como llave */
		std::queue<std::pair<struct data_frame *, struct sockaddr_in *>> processed_messages; /* Cola de mensajes procesados listos para ser leidos*/
		Semaphore sem_recv; /* Semaforo que bloquea el Recvfrom hasta que hayan mensajes procesados */
		Semaphore sem_map; /* Semaforo que hace que la modificacion de los miembros del mapa sent sean atomica */
		Semaphore sem_queue; /* Semaforo que hace que el modificar la cola sea atomica */
		/*
		* Es la funcion que estara corriendo el thread que procesa los mensajes recibidos y los coloca en la cola de procesados,
		* y elimina los mensajes del mapa que ya han sido recibidos.
		*/
		void receiver();
		/*
		* Es la funcion que estara corriendo el thread que enviara y reenviara los mensajes en el mapa de por enviar.
		*/
		void sender();
	public:
		/*
		* Crea un socket USL
		* @param uint16_t port: El puerto que se utilizara para los envios y recepciones
		*/
		reUDP(uint16_t);
		/*
		* Cierra el socket interno
		*/
		~reUDP();
		/*
		* Inicializa los threads que estaran enviando y recibiendo mensajes
		*/
		void run();
		
		/*
		* Prepara el frame y la informacion necesaria para que sender eventualmente envie este mensaje
		* @param const char * message: Buffer que se desea enviar.
		* @param const char * destination: IP del host al que se desea enviar.
		* @param uint16_t port: Puerto en el que esta el otro host
		* @param int len: Longitud del buffer que se desea enviar.
		*/
		void Sendto(const char * message, const char * destination, uint16_t port, size_t len = PAYLOAD_SIZE);
		/*
		* Recibe los mensajes procesados que se encuentren en la cola
		* @param char * message: Buffer al que se escribira lo recibido
		* @param struct sockaddr_in * client_addr: Puntero a struct en el que se guarda la informacion del otro host.
		*/
		void Recvfrom(char *, struct sockaddr_in * = nullptr);
		/*
		* Imprime en formato hexadecimal los contenidos de un frame
		* @param const struct data_frame * packet: El frame que se desea imprimir
		* @param size_t len: El tamano del data_frame
		*/
		void printPacket(const struct data_frame *, size_t);
};

#endif
