CXX = g++

client.o: client.cpp
	$(CXX) -c client.cpp

server.o: server.cpp
	$(CXX) -c server.cpp

sem_test.o: sem_test.cpp
	$(CXX) -c sem_test.cpp

secudp.o: ../secudp.cpp
	$(CXX) -c ../secudp.cpp

socket.o: ../socket.cpp
	$(CXX) -c ../socket.cpp

semaphore.o: ../semaphore.cpp
	$(CXX) -c ../semaphore.cpp

error_handler.o: ../error_handler.c
	$(CXX) -c ../error_handler.c

srv: server.o secudp.o socket.o semaphore.o error_handler.o
	$(CXX) -pthread server.o secudp.o socket.o semaphore.o error_handler.o -o srv

cln: client.o secudp.o socket.o semaphore.o error_handler.o
	$(CXX) -pthread client.o secudp.o socket.o semaphore.o error_handler.o -o cln

sem_test: sem_test.o semaphore.o
	$(CXX) -pthread sem_test.o semaphore.o -o sem_test

clean:
	rm *.o
