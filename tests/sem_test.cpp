#include "../semaphore.h"
#include <iostream>
#include <thread>


Semaphore sem0(0);
Semaphore sem1(0);

void test(){
	std::cout << "0" << std::endl;
	sem0.signal();
}

void test1(){
	sem1.wait();
	std::cout << "1" << std::endl;
	sem0.signal();
}

int main(){
	std::thread second(test1);
	std::thread first(test);
	sem0.wait();
	sem1.signal();
	sem0.wait();
	std::cout << "2" << std::endl;
	first.join();
	second.join();
	return 0;
}
