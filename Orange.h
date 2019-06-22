#pragma once

#include <iostream>
#include <thread>

#include "packet.h"
#include "socket.h"

#include <string>
#include <list>
#include <unordered_map>

using namespace std;

class Orange
{
	public:
		char* host_ip_addr;
	
		size_t num_total_oranges;
		
		unsigned int* ip_buffer;

		std::unordered_map<int, std::list<int>> blue_graph;
		
		void get_host_ip();
	
	public:
		Orange(std::string = "Grafo_Referencia.csv", int total_oranges = 6);
		
		~Orange();
		
		void begin_contention();
		
		void print_graph();
		
	
};
