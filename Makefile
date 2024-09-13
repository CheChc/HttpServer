CXX = g++
CXXFLAGS = -std=c++11 -pthread
BOOSTFLAGS = -lboost_system -lboost_thread

TARGET = server
SRC = main.cpp ThreadPool.cpp HttpServer.cpp
OBJ = $(SRC:.cpp=.o)

server: main.o HttpServer.o HttpHandler.o ThreadPool.o
	g++ -std=c++11 -pthread -o server main.o HttpServer.o HttpHandler.o ThreadPool.o -lboost_system

main.o: main.cpp HttpServer.h
	g++ -std=c++11 -pthread -c main.cpp

HttpServer.o: HttpServer.cpp HttpServer.h
	g++ -std=c++11 -pthread -c HttpServer.cpp

HttpHandler.o: HttpHandler.cpp HttpHandler.h
	g++ -std=c++11 -pthread -c HttpHandler.cpp

ThreadPool.o: ThreadPool.cpp ThreadPool.h
	g++ -std=c++11 -pthread -c ThreadPool.cpp

clean:
	rm -f *.o server



