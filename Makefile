orange : socket.cpp encoder.cpp error_handler.c orange.cpp
	g++ -g -o orange.o socket.cpp encoder.cpp error_handler.c orange.cpp -lpthread
.PHONY clean :
	rm orange.o
