#ifndef LCSV_H
#define LCSV_H

#include <string>
#include <unordered_map>
#include <list>

void parseCSV(std::string, std::list<int> *);
void loadCSV(std::string, std::unordered_map<int, std::list<int>> *);

#endif
