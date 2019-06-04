#include "socket.h"

class reUDP{
	Socket sock;
	public:
		reUDP();
		~reUDP();
		void Sendto(const char *, int, const char *, int = DEF_PORT);
		void Recvfrom(char *, int, struct sockaddr_in* = nullptr);
}
