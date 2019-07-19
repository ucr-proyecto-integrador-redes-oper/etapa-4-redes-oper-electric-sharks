#include <fstream>
#include <string>
#include <unordered_map>
#include <list> 
#include <iostream>

using namespace std;

void parseCSV(int i, std::string str, std::list<int> &list){
	int s = 0;
	unsigned long f = 0;
	int node = 0;
	while(f != std::string::npos){
		f = str.find(',', s);
		node = std::stoi(str.substr(s, f - s));
		if(f != std::string::npos){
			if(i != node)
				list.push_back(node);
			
		} else {
			if(i != node)
				list.push_back(std::stoi(str.substr(s, str.size() - s)));
		}
		s = f + 1;
	}
}

int loadCSV(std::string csv_file, std::unordered_map<int, std::list<int>> * mmap){
	std::string str;
	std::fstream fs(csv_file);
	if(!fs){
		cout << "No se pudo cargar el .csv!" << endl;
	}
	int i = 0;
	while(fs){
		++i;
		fs >> str;
		if(str.size() > 0){
			mmap->emplace(i, std::list<int>());
			parseCSV(i, str, mmap->at(i));
		}
		str.clear();
	}
	return i-1;
}
