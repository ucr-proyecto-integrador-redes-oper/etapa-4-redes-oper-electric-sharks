CXX = g++

FLAGS = -g -Wall -Wextra

client.o: client.cpp
	$(CXX) $(FLAGS) -c client.cpp

server.o: server.cpp
	$(CXX) $(FLAGS) -c server.cpp

sem_test.o: sem_test.cpp
	$(CXX) $(FLAGS) -c sem_test.cpp

chat.o: chat.cpp
	$(CXX) $(FLAGS) -c chat.cpp

secudp.o: ../secudp.cpp
	$(CXX) $(FLAGS) -c ../secudp.cpp

socket.o: ../socket.cpp
	$(CXX) $(FLAGS) -c ../socket.cpp

semaphore.o: ../semaphore.cpp
	$(CXX) $(FLAGS) -c ../semaphore.cpp

error_handler.o: ../error_handler.c
	$(CXX) $(FLAGS) -c ../error_handler.c

srv: server.o secudp.o socket.o semaphore.o error_handler.o
	$(CXX) $(FLAGS) -pthread server.o secudp.o socket.o semaphore.o error_handler.o -o srv

cln: client.o secudp.o socket.o semaphore.o error_handler.o
	$(CXX) $(FLAGS) -pthread client.o secudp.o socket.o semaphore.o error_handler.o -o cln

sem_test: sem_test.o semaphore.o
	$(CXX) $(FLAGS) -pthread sem_test.o semaphore.o -o sem_test

chat: chat.o secudp.o socket.o semaphore.o error_handler.o
	$(CXX) $(FLAGS) -pthread chat.o secudp.o socket.o semaphore.o error_handler.o -o chat

clean:
	rm *.o
