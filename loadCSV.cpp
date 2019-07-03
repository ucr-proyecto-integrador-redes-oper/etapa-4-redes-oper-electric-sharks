#include <fstream>
#include <string>
#include <unordered_map>
#include <list> 

#include "error_handler.h"


using namespace std;

void parseCSV(std::string str, std::list<int> &list){
	int s = 0;
	unsigned long f = 0;
	while(f != std::string::npos){
		f = str.find(',', s);
		if(f != std::string::npos){
			list.push_back(std::stoi(str.substr(s, f - s)));
			s = f + 1;
		} else {
			list.push_back(std::stoi(str.substr(s, str.size() - s)));
		}
	}
}

void loadCSV(std::string csv_file, std::unordered_map<int, std::list<int>> * mmap){
	std::string str;
	std::fstream fs(csv_file);
	if(!fs){
		error_exit(-1, "No se pudo abrir el archivo csv!\n");
		return; 
	}
	int i = 1;
	while(fs){
		mmap->emplace(i, std::list<int>());
		fs >> str;
		if(str.size() > 0){
			parseCSV(str, mmap->at(i));
		}
		str.clear();
		++i;
	}
}
