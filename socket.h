#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <cstdio>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#define DEF_PORT 8080

class Socket{
	private:
		
		int sfd; //Socket File Descriptor
	public:
		enum Protocol{
			TCP,
			UDP
		};
	
		Socket(Protocol p, bool = false);
		Socket(int);
		~Socket();
		int Connect(const char *, int);
		int Read(char *, int);
		int Write(const char *, int);
		int Listen(int = SOMAXCONN);
		int Bind(long);
		Socket * Accept();
		int Sendto(const char *, int, const char *, int = DEF_PORT);
		int Recvfrom(char *, int, struct sockaddr_in* = nullptr);
		int Shutdown(int);
		int Close();
};

#endif
