#include "socket.h"
#include "error_handler.h"

#include <cstdint>
#include <iostream>
#include <ifaddrs.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

Socket::Socket(Protocol p, unsigned long udp_port, bool ipv6){
	int domain = (ipv6 ? AF_INET6 : AF_INET);
	type = (p == TCP ? SOCK_STREAM : SOCK_DGRAM);
	this->udp_port = (p == UDP ? udp_port : -1);
	sfd = socket(domain, type, 0);
	if(sfd == -1){
		error_exit(errno, "Error creating socket\n");
	}
}

Socket::Socket(int sfd){
	this->sfd = sfd;
}

Socket::~Socket(){
	if(close(sfd) == -1){
		error_exit(errno, "Error closing socket\n");
	}
}

int Socket::Connect(const char * host, int port){
	int status;
	struct sockaddr_in client_addr;
	struct in_addr addr;
	if(inet_aton(host, &addr) == 0){
		error_exit(errno, "Error parsing the IP address socket\n");
	}
	client_addr.sin_addr = addr;
	client_addr.sin_port = htons(port);
	client_addr.sin_family = AF_INET;
	if((status = connect(sfd, (const struct sockaddr *) &client_addr, sizeof(struct sockaddr_in))) == -1){
		perror("Error connecting!\n");
	}
	return status;
}

int Socket::Read(char * text, int len){
	int bytes;
	if((bytes = recv(sfd, text, len, MSG_WAITALL)) == -1){
		perror("Error reading!\n");
	}
	return bytes;
}

int Socket::Write(const char * text, int len){
	int bytes;
	if((bytes = write(sfd, text, len)) == -1){
		perror("Error writing!\n");
	}
	return bytes;
}

int Socket::Listen(int queue){
	int status;
	if((status = listen(sfd, queue)) == -1){
		perror("Error listening!\n");
	}
	return status;
}

int Socket::Bind(uint16_t port){
	int status;
	struct sockaddr_in server_addr;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);
	server_addr.sin_family = AF_INET;
	if((status = bind(sfd, (const struct sockaddr *) &server_addr, sizeof(struct sockaddr_in))) == -1){
		perror("Error connecting!\n");
	}
	return status;
}

Socket * Socket::Accept(){
	Socket * accepted_socket = nullptr;
	int accepted_sfd;
	unsigned int add_len;
	struct sockaddr_in incoming_addr;
	if((accepted_sfd = accept(sfd, (struct sockaddr *) &incoming_addr, &add_len)) == -1){
		perror("Error accepting!\n");
	} else {
		accepted_socket = new Socket(accepted_sfd);
	}
	return accepted_socket;
}

int Socket::Shutdown(int how){
	int status;
	if((status = shutdown(sfd, how)) == -1){
		perror("Error shutting down!\n");
	}
	return status;
}

int Socket::Sendto(const char * message, int len, const char * destination, int port){
	struct sockaddr_in dest_addr;
	struct in_addr addr;
	inet_aton(destination, &addr);
	dest_addr.sin_addr = addr;
	dest_addr.sin_port = htons(port);
	dest_addr.sin_family = AF_INET;
	
	int sent;
	sent = sendto(sfd, (const void *) message, len, 0, (struct sockaddr *) &dest_addr, sizeof(struct sockaddr_in));
	return sent;
}

int Socket::Sendto(const char * message, int len, struct in_addr addr, uint16_t port){
	struct sockaddr_in dest_addr;
	dest_addr.sin_addr = addr;
	dest_addr.sin_port = htons(port);
	dest_addr.sin_family = AF_INET;
	
	int sent;
	sent = sendto(sfd, (const void *) message, len, 0, (struct sockaddr *) &dest_addr, sizeof(struct sockaddr_in));
	return sent;
}

int Socket::Sendto(const char * message, int len, struct sockaddr_in * dest_addr){
	int sent;
	sent = sendto(sfd, (const void *) message, len, 0, (struct sockaddr *) dest_addr, sizeof(struct sockaddr_in));
	return sent;
}

int Socket::Recvfrom(char * message, int len, struct sockaddr_in * client_addr){
	struct sockaddr_in receiver_addr;
	if(!client_addr)
		client_addr = &receiver_addr;
	
	receiver_addr.sin_family = AF_INET;
	receiver_addr.sin_addr.s_addr = INADDR_ANY;
	receiver_addr.sin_port = htons(udp_port);
	
	bind(sfd, (const struct sockaddr *) &receiver_addr,  sizeof(receiver_addr));
	
	int received;
	unsigned int struct_size = sizeof(struct sockaddr_in);
	received = recvfrom(sfd, (void *) message, len, 0, (struct sockaddr *) client_addr, &struct_size);
	return received;
}

int Socket::Recvfrom(char * message, int len, unsigned short port, struct sockaddr_in* client_addr){
	struct sockaddr_in receiver_addr;
	if(!client_addr)
		client_addr = &receiver_addr;
	
	receiver_addr.sin_family = AF_INET;
	receiver_addr.sin_addr.s_addr = INADDR_ANY;
	receiver_addr.sin_port = htons(port);
	
	bind(sfd, (const struct sockaddr *)&receiver_addr,  sizeof(receiver_addr));
	
	int received;
	unsigned int struct_size = sizeof(struct sockaddr_in);
	received = recvfrom(sfd, (void *) message, len, 0, (struct sockaddr *) client_addr, &struct_size);
	return received;
}
int Socket::Close(){
	int status;
	if(type == SOCK_STREAM && (status = close(sfd)) == -1){
		perror("Error closing socket file descriptor!\n");
	}
	return status;
}

///Function that takes an ipv4 address in binary notation and returns it in dots notation.
char* Socket::decode_ip(unsigned int ip, char* str)
{
	unsigned char bytes[4];
	bytes[0] = ip & 0xFF;
	bytes[1] = (ip >> 8) & 0xFF;
	bytes[2] = (ip >> 16) & 0xFF;
	bytes[3] = (ip >> 24) & 0xFF;   
	sprintf(str, "%d.%d.%d.%d", bytes[0], bytes[1], bytes[2], bytes[3]);        
	return str;
}

unsigned int Socket::encode_ip(char* ip)
{
	struct in_addr ip_addr;
	inet_aton(ip, &ip_addr);
	return (int) ip_addr.s_addr;
}

int Socket::validateIP(char* ip)
{
	char ipBuffer[IP_LEN];
	return (inet_pton(AF_INET, ip, ipBuffer) == 1 ? 1 : 0);
}

void Socket::getHostIP(char* buffer)
{
	struct ifaddrs *ifaddr, *ifa;
    char host[NI_MAXHOST];
    memset(host, 0, NI_MAXHOST);
    memset(buffer, 0, IP_LEN);
    
    string interfaces[NUM_INTERFACES];
    
    /*Posibles interfaces de las que se puede obtener la IP del host.*/
    interfaces[0] = string("eno1");
    interfaces[1] = string("wlo1");
    interfaces[2] = string("eth0");
    interfaces[3] = string("enp1s0");

    if (getifaddrs(&ifaddr) == -1) 
    {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

	for(int interface = 0; interface < NUM_INTERFACES; ++interface){
		for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
			if (ifa->ifa_addr == NULL){
				continue;  
			}

			getnameinfo(ifa->ifa_addr,sizeof(struct sockaddr_in),host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

			if((strcmp(ifa->ifa_name, interfaces[interface].c_str())==0)){
				/*Si la direccion es valida y no es loopback, se asigna al nodo naranja*/
				if(Socket::validateIP(host) && strcmp("127.0.0.1", host) && strcmp("192.168.194.1", host)){
					memcpy(buffer, host, IP_LEN);
					cout << "Dirección IP: " << buffer << ", interfaz: " << ifa->ifa_name << endl;
					break;
				}
			}
		}
	}

    freeifaddrs(ifaddr);
    if(!Socket::validateIP(buffer)){
		error_exit(EXIT_FAILURE, "No se pudo obtener la IP del host!\n");
	}
}
