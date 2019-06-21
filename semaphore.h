#ifndef SEM_H
#define SEM_H

#include <mutex>
#include <condition_variable>

class Semaphore{
	private:
		int value;
		std::mutex valueMtx;
		std::mutex semMtx;
		std::condition_variable cv;
		std::unique_lock<std::mutex> lck;
		
	public:
		Semaphore(int initVal = 0) : lck(semMtx), value(initVal) {}
		void wait();
		void signal();
};

#endif
