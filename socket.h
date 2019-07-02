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
		unsigned long udp_port;
		int sfd; //Socket File Descriptor
		int type;
	public:
	
		Socket(Protocol p, unsigned long = -1, bool = false);
		Socket(int);
		~Socket();
		int Connect(const char *, int);
		int Read(char *, int);
		int Write(const char *, int);
		int Listen(int = SOMAXCONN);
		int Bind(long);
		Socket * Accept();
		int Sendto(const char *, int, const char *, int = DEF_PORT);
		int Sendto(const char *, int, struct in_addr, uint16_t);
		int Sendto(const char *, int, struct sockaddr_in *);
		int Recvfrom(char *, int, struct sockaddr_in* = nullptr);
		int Recvfrom(char *, int, unsigned short = DEF_PORT, struct sockaddr_in* = nullptr);
		int Shutdown(int);
		int Close();
		
		///Function that takes an ipv4 address in binary notation and returns it in dots notation.
		static char* decode_ip(unsigned int ip, char* str);

		static unsigned int encode_ip(char* ip);
		
		static int validateIP(char* ip);
		
		static void getHostIP(char* buffer);
};

#endif
