CXX = g++ 

FLAGS = -D DEBUG -g -Wall
 
orange: orange.o socket.o encoder.o error_handler.o loadCSV.o
	$(CXX) -g -o orange.o orange.cpp socket.cpp encoder.cpp error_handler.c loadCSV.cpp -pthread

warn: orange.o socket.o encoder.o error_handler.o loadCSV.o
	$(CXX) -Wall -Wextra -g -o orange.o orange.cpp socket.cpp encoder.cpp error_handler.c loadCSV.cpp -pthread
	
blue: socket.o encoder.o error_handler.o blue.o
	$(CXX) -g -o blue.o blue.cpp socket.cpp error_handler.c encoder.cpp -pthread

greenNode: greenNode.o secudp.o socket.o semaphore.o error_handler.o
	$(CXX) $(FLAGS) -pthread greenNode.o secudp.o socket.o semaphore.o error_handler.o -o greenNode

socket.o: socket.cpp
	$(CXX) $(FLAGS) -c socket.cpp

semaphore.o: semaphore.cpp
	$(CXX) $(FLAGS) -c semaphore.cpp

secudp.o: secudp.cpp
	$(CXX) $(FLAGS) -c secudp.cpp

orange.o: orange.cpp
	$(CXX) $(FLAGS) -c orange.cpp

greenNode.o: greenNode.cpp
	$(CXX) $(FLAGS) -c greenNode.cpp

loadCSV.o: loadCSV.cpp
	$(CXX) $(FLAGS) -c loadCSV.cpp

error_handler.o: error_handler.c
	$(CXX) $(FLAGS) -c error_handler.c

encoder.o: encoder.cpp
	$(CXX) $(FLAGS) -c encoder.cpp

.PHONY clean:
clean:
	rm -rf *.o
