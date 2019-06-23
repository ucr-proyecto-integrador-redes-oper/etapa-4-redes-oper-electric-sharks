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
		int Recvfrom(char *, int, unsigned short = DEF_PORT, struct sockaddr_in* = nullptr);
		int Shutdown(int);
		int Close();
		
		///Function that takes an ipv4 address in binary notation and returns it in dots notation.
		char* decode_ip(unsigned int ip, char* str)
		{
			unsigned char bytes[4];
			bytes[0] = ip & 0xFF;
			bytes[1] = (ip >> 8) & 0xFF;
			bytes[2] = (ip >> 16) & 0xFF;
			bytes[3] = (ip >> 24) & 0xFF;   
			sprintf(str, "%d.%d.%d.%d", bytes[0], bytes[1], bytes[2], bytes[3]);        
			return str;
		}

		unsigned int encode_ip(char* ip)
		{
			struct in_addr ip_addr;
			inet_aton(ip, &ip_addr);
			return (int) ip_addr.s_addr;
		}
};

#endif
