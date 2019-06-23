#include <fstream>
#include <string>
#include <unordered_map>
#include <list> 
#include <iostream>

using namespace std;

void parseCSV(std::string str, std::unordered_map<int, std::list<int>> * mmap){
	int s = 0;
	int f = 0;
	std::list<int> * aux = new std::list<int>();
	while(f != std::string::npos){
		f = str.find(',', s);
		if(f != std::string::npos){
			aux->push_back(std::stoi(str.substr(s, f - s)));
			s = f + 1;
		} else {
			aux->push_back(std::stoi(str.substr(s, str.size() - s)));
		}
	}
	auto it = aux->begin();
	++it;
	aux->splice((*mmap)[aux->front()].begin(), *aux, it, aux->end());
	delete aux;
}

void loadCSV(std::string csv_file, std::unordered_map<int, std::list<int>> * mmap){
	std::string str;
	std::fstream fs(csv_file);
	if(!fs){
		cout << "Error! No se pudo cargar el archivo " << csv_file << endl;
		return; 
	}
	int i = 0;
	while(fs){
		fs >> str;
		if(str.size() > 0){
			parseCSV(str, mmap);
		}
		str.clear();
		++i;
	}
}