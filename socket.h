#ifndef SOCKET_H
#define SOCKET_H

#include <cstdint>
#include <cstdio>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>

#define DEF_PORT 6060
#define IP_LEN 16
#define NUM_INTERFACES 4


enum Protocol{
	TCP,
	UDP
};
class Socket{
	private:
		unsigned long udp_port; //Puerto utilizado por un socket UDP
		int sfd; //Socket File Descriptor
		int type; //Tipo, UDP o TCP
	public:
	
		/*
		* Crea un socket
		* @param Protocol p: El protocolo UDP o TCP que se va a utilizar con este socket
		* @param udp_port: El puerto UDP que se desea utilizar para los envios
		* @param bool ipv6: Para denotar que se desea utilizar IPv6 (no implementado)
		*/
		Socket(Protocol, unsigned long = -1, bool = false);
		/*
		* Crea un socket a partir de otro ya creado por syscalls de unix
		* @param int sfd: Un descriptor de socket que ya ha sido creado por otros medios.
		*/
		Socket(int);
		/*
		* Cierra el socket
		*/
		~Socket();
		/*
		* Para uso con TCP, conecta el socket con otro
		* @param const char * host: IP en formato de puntos del otro host
		* @param int port: Puerto utilizado en el otro host
		* @return Posible codigo de error de la operacion.
		*/
		int Connect(const char *, int);
		/*
		* Lee en un buffer hasta una cantidad de bytes
		* @param char * text: Buffer en el que se lee.
		* @param int len: Longitud maxima de lo que se va a leer.
		* @return Cantidad de bytes que se leyeron.
		*/
		int Read(char *, int);
		/*
		* Escribe en el socket hasta una cantidad de bytes.
		* @param const char * text: Buffer que se escribira.
		* @param int len: Longitud de lo que se escribira.
		* @return Cantidad de bytes que se escribieron.
		*/
		int Write(const char *, int);
		/*
		* Marca el socket como escuchando.
		* @param int queue: Cantidad maxima de conexiones
		* @return El codigo de error
		*/
		int Listen(int = SOMAXCONN);
		/*
		* Amarra el socket a un puerto.
		* @param uint16_t port: Puerto que se desea utilizar
		* @return El codigo de error
		*/
		int Bind(uint16_t);
		/*
		* Acepta la conexion que otro socket desea iniciar con este.
		* @return Un puntero al socket conectado.
		*/
		Socket * Accept();
		/*
		* Envia un buffer por medio del socket.
		* @param const char * message: Buffer que se desea enviar.
		* @param int len: Longitud del buffer que se desea enviar.
		* @param const char * destination: IP del host al que se desea enviar.
		* @param int port: Puerto en el que esta el otro host
		* @return Cantidad de bytes que se enviaron
		*/
		int Sendto(const char *, int, const char *, int = DEF_PORT);
		/*
		* Envia un buffer por medio del socket.
		* @param const char * message: Buffer que se desea enviar.
		* @param int len: Longitud del buffer que se desea enviar.
		* @param struct in_addr: Struct que contiene la IP del otro host en formato numerico.
		* @param uint16_t port: Puerto en el que esta el otro host
		* @return Cantidad de bytes que se enviaron
		*/
		int Sendto(const char *, int, struct in_addr, uint16_t);
		/*
		* Envia un buffer por medio del socket.
		* @param const char * message: Buffer que se desea enviar.
		* @param int len: Longitud del buffer que se desea enviar.
		* @param struct sockaddr_in *: Puntero a un struct que contiene la informacion (IP y puerto) del otro host.
		* @return Cantidad de bytes que se enviaron
		*/
		int Sendto(const char *, int, struct sockaddr_in *);
		/*
		* Recibe los mensajes que llegan (Utiliza el udp_port)
		* @param char * message: Buffer al que se escribira lo recibido
		* @param len: Cantidad maxima de bytes que se leeran
		* @param struct sockaddr_in * client_addr: Puntero a struct en el que se guarda la informacion del otro host.
		* @return Cantidad de bytes leidos.
		*/
		int Recvfrom(char *, int, struct sockaddr_in* = nullptr);
		/*
		* Recibe los mensajes que llegan
		* @param char * message: Buffer al que se escribira lo recibido
		* @param len: Cantidad maxima de bytes que se leeran
		* @param unsigned short port: Puerto por el que se recibira el mensaje
		* @param struct sockaddr_in * client_addr: Puntero a struct en el que se guarda la informacion del otro host.
		* @return Cantidad de bytes leidos.
		*/
		int Recvfrom(char *, int, unsigned short = DEF_PORT, struct sockaddr_in* = nullptr);
		/*
		* Cierra una comunicacion Full-duplex
		* @param how: El modo en que se cierra
		* @return El codigo de error de la accion
		*/
		int Shutdown(int);
		/*
		* Cierra el socket.
		* @return El codigo de error de la accion
		*/
		int Close();
		
		/*
		* Funcion que toma una IP en formato binario y la devuelve en formato de puntos
		* @param ip: IP en formato binario
		* @param char* str: Arreglo en que se guardara la IP
		* @return El mismo str
		*/
		static char* decode_ip(unsigned int ip, char* str);
		/*
		* Funcion que toma una IP en fomato de puntos y la pasa a formato binario
		* @param unsigned int ip: IP en formato de puntos
		* @return IP en formato binario
		*/
		static unsigned int encode_ip(char* ip);
		/*
		* Funcion que valida una ip en formato de puntos
		* @param char * ip: IP en formato de puntos
		* @return 1 si la IP es valida y 0 si no lo es
		*/
		static int validateIP(char* ip);
		/*
		* Funcion que lee la IP del host desde el sistema
		* @param char * buffer: Buffer en el que se guarda la IP en formato de puntos
		*/
		static void getHostIP(char* buffer);
};

#endif
