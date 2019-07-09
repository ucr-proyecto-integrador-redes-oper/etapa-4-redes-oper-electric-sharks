#ifndef SEM_H
#define SEM_H

#include <mutex>
#include <condition_variable>

class Semaphore{
	private:
		int value; /*Valor actual del semaforo*/
		std::mutex valueMtx; /*Mutex para atomizar el valor*/
		std::mutex semMtx; /*Mutex que se utiliza para la variable de condicion*/
		std::condition_variable cv; /*Variable de condicion que se utiliza para despertar al semaforo*/
		
	public:
		/*
		* Crea un Semaforo
		* @param int initVal: El valor inicial del semaforo
		*/
		Semaphore(int initVal = 0) :  value(initVal) {}
		/*
		* Le resta uno al valor del semaforo, si queda en 0, el thread duerme.
		*/
		void wait();
		/*
		* Le suma uno al valor del semaforo y despierta a algun thread que este dormido
		*/
		void signal();
};

#endif
