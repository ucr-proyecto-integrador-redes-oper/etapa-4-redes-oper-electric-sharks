CXX = g++ 
 
orange: orange.o socket.o encoder.o error_handler.o loadCSV.o
	$(CXX) -g -o orange.o orange.cpp socket.cpp encoder.cpp error_handler.c loadCSV.cpp -pthread

warn: orange.o socket.o encoder.o error_handler.o loadCSV.o
	$(CXX) -Wall -Wextra -g -o orange.o orange.cpp socket.cpp encoder.cpp error_handler.c loadCSV.cpp -pthread
	
blue: socket.o encoder.o error_handler.o blue.o
	$(CXX) -g -o blue.o blue.cpp socket.cpp error_handler.c encoder.cpp -pthread

greenNode: greenNode.o secudp.o socket.o semaphore.o error_handler.o
	$(CXX) -pthread greenNode.o secudp.o socket.o semaphore.o error_handler.o -o greenNode

socket.o: socket.cpp
	$(CXX) -c socket.cpp

semaphore.o: semaphore.cpp
	$(CXX) -c semaphore.cpp

secudp.o: secudp.cpp
	$(CXX) -c secudp.cpp

orange.o: orange.cpp
	$(CXX) -c orange.cpp

greenNode.o: greenNode.cpp
	$(CXX) -c greenNode.cpp

loadCSV.o: loadCSV.cpp
	$(CXX) -c loadCSV.cpp

error_handler.o: error_handler.c
	$(CXX) -c error_handler.c

encoder.o: encoder.cpp
	$(CXX) -c encoder.cpp

.PHONY clean:
clean:
	rm -rf *.o
