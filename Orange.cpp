#include "Orange.h"

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <errno.h> 
#include <netdb.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 

Orange::Orange(int total_oranges)
: num_total_oranges(total_oranges), ip_buffer(nullptr), host_ip_addr(nullptr)
{
	this->ip_buffer = new unsigned int[this->num_total_oranges]();
}

Orange::~Orange()
{
	/*if(this->ip_buffer)
		delete this->ip_buffer;
	if(this->host_ip_addr)
		delete this->host_ip_addr;*/
}

void Orange::get_host_ip()
{
	char hostbuffer[256]; 
    struct hostent *host_entry; 
    int hostname; 
  
    // To retrieve hostname 
    hostname = gethostname(hostbuffer, sizeof(hostbuffer)); 
    //checkHostName(hostname); 
  
    // To retrieve host information 
    host_entry = gethostbyname(hostbuffer); 
    //checkHostEntry(host_entry); 
  
    // To convert an Internet network 
    // address into ASCII string 
    this->host_ip_addr = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0])); 
  
    printf("Hostname: %s\n", hostbuffer); 
    printf("Host IP: %s\n", this->host_ip_addr); 
}

int main()
{
	Orange node;
	node.get_host_ip();
	return 0;
}
