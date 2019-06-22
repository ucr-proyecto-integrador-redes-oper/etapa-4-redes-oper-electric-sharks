#ifndef SOCKET_H
#define SOCKET_H

#include <cstdint>
#include <cstdio>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>


enum Protocol{
	TCP,
	UDP
};
class Socket{
	private:
		unsigned long udp_port;
		int sfd; //Socket File Descriptor
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
		int Sendto(const char *, int, const char *, int);
		int Sendto(const char *, int, struct in_addr, uint16_t);
		int Sendto(const char *, int, struct sockaddr_in *);
		int Recvfrom(char *, int, struct sockaddr_in* = nullptr);
		int Shutdown(int);
		int Close();
};

#endif
