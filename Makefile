CXX = g++ 
 
orange: orange.o socket.o encoder.o error_handler.o loadCSV.o
	$(CXX) -g -o orange.o orange.cpp socket.cpp encoder.cpp error_handler.c loadCSV.cpp -pthread

blue: blue.o  socket.o encoder.o error_handler.o secudp.o
	$(CXX) -g -o blue.o blue.cpp socket.cpp encoder.cpp error_handler.c secudp.cpp -pthread

warn: orange.o socket.o encoder.o error_handler.o loadCSV.o
	$(CXX) -Wall -Wextra -g -o orange.o orange.cpp socket.cpp encoder.cpp error_handler.c loadCSV.cpp -pthread

socket.o: socket.cpp
	$(CXX) -c socket.cpp

semaphore.o: semaphore.cpp
	$(CXX) -c semaphore.cpp

secudp.o: secudp.cpp
	$(CXX) -c secudp.cpp

orange.o: orange.cpp
	$(CXX) -c orange.cpp

blue.o: blue.cpp
	$(CXX) -c blue.cpp

loadCSV.o: loadCSV.cpp
	$(CXX) -c loadCSV.cpp

error_handler.o: error_handler.c
	$(CXX) -c error_handler.c

encoder.o: encoder.cpp
	$(CXX) -c encoder.cpp

clean:
	rm *.o
