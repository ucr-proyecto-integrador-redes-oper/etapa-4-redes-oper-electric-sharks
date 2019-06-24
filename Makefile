CXX = g++

socket.o: socket.cpp
	$(CXX) -c socket.cpp

semaphore.o: semaphore.cpp
	$(CXX) -c semaphore.cpp

secudp.o: secudp.cpp
	$(CXX) -c secudp.cpp

orange.o: orange.cpp
	$(CXX) -c orange.cpp

loadCSV.o: loadCSV.cpp
	$(CXX) -c loadCSV.cpp

error_handler.o: error_handler.c
	$(CXX) -c error_handler.c

encoder.o: encoder.cpp
	$(CXX) -c encoder.cpp

orange: orange.o socket.o encoder.o error_handler.o loadCSV.o
	$(CXX) orange.o socket.o encoder.o error_handler.o loadCSV.o -pthread -o orange

clean:
	rm *.o
