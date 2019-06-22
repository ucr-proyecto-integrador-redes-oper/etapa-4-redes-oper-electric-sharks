#include "semaphore.h"
#include <mutex>
#include <condition_variable>

void Semaphore::wait(){
	valueMtx.lock();
	if(value == 0){
		valueMtx.unlock();
		cv.wait(lck);

		valueMtx.lock();
		value -= 1;
		valueMtx.unlock();
		
	} else {
		value -= 1;
		valueMtx.unlock();
	}
}

void Semaphore::signal(){
	valueMtx.lock();
	value += 1;
	valueMtx.unlock();
	cv.notify_one();
}
