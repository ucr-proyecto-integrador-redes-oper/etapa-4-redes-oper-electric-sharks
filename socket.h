#ifndef SOCKET_H
#define SOCKET_H
#include <sys/socket.h>

class Socket{
	private:
		int sfd; //Socket File Descriptor
	public:
		Socket(char = 's', bool = false);
		Socket(int);
		~Socket();
		int Connect(const char *, int);
		int Read(char *, int);
		int Write(const char *, int);
		int Listen(int = SOMAXCONN);
		int Bind(long);
		Socket * Accept();
		int Sendto(const char *, int, const char *, int);
		int Recvfrom(char *, int);
		int Shutdown(int);
		int Close();
};

#endif
