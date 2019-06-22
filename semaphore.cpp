#include "semaphore.h"
#include <mutex>
#include <condition_variable>

#define DEBUG

#ifdef DEBUG
#include <iostream>
#endif

void Semaphore::wait(){
	std::unique_lock<std::mutex> lck(semMtx);
	valueMtx.lock();
	if(value == 0){
		valueMtx.unlock();
		cv.wait(lck);
	} else {
		valueMtx.unlock();
	}
	valueMtx.lock();
	value -= 1;
	valueMtx.unlock();
}

void Semaphore::signal(){
	std::unique_lock<std::mutex> lck(semMtx);
	valueMtx.lock();
	value += 1;
	valueMtx.unlock();
	cv.notify_one();
}
