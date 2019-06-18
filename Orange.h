#pragma once

#include <iostream>
#include <thread>

#include "packet.h"
#include "socket.h"

using namespace std;

class Orange
{
	public:
		char* host_ip_addr;
	
		size_t num_total_oranges;
		
		unsigned int* ip_buffer;
		
		void get_host_ip();
	
	
	public:
		Orange(int total_oranges = 6);
		
		~Orange();
		
		void begin_contention();
		
		
	
};
