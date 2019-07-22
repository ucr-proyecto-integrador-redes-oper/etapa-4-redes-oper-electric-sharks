CXX = g++ 

FLAGS = -g -Wall -Wextra

.DEFAULT_GOAL := all

all: orange blue greenNode blueMonitor
 
orange: orange.o socket.o encoder.o error_handler.o loadCSV.o secudp.o semaphore.o
	$(CXX) $(FLAGS) -o orange socket.o encoder.o error_handler.o loadCSV.o secudp.o semaphore.o orange.o -pthread
	
blue: socket.o encoder.o error_handler.o blue.o secudp.o semaphore.o message.o
	$(CXX) $(FLAGS) -o blue blue.o socket.o error_handler.o encoder.o secudp.o semaphore.o message.o -pthread 

greenNode: greenNode.o secudp.o socket.o semaphore.o error_handler.o
	$(CXX) $(FLAGS) -pthread greenNode.o secudp.o socket.o semaphore.o error_handler.o -o greenNode

blueMonitor: blueMonitor.o message.o error_handler.o
	$(CXX) $(FLAGS) -o blueMonitor blueMonitor.o message.o error_handler.o

blue.o: blue.cpp
	$(CXX) $(FLAGS) -c blue.cpp

blueMonitor.o: blueMonitor.cpp
	$(CXX) $(FLAGS) -c blueMonitor.cpp

message.o: message.cpp
	$(CXX) $(FLAGS) -c message.cpp

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
