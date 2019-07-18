#include "semaphore.h"
#include <mutex>
#include <condition_variable>

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
