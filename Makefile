CXX = g++
CXXFLAGS = -std=c++11 -pthread
BOOSTFLAGS = -lboost_system -lboost_thread

all: server

server: main.o HttpServer.o HttpHandler.o ThreadPool.o Logger.o
	$(CXX) $(CXXFLAGS) $(BOOSTFLAGS) -o server main.o HttpServer.o HttpHandler.o ThreadPool.o Logger.o

main.o: main.cpp
	$(CXX) $(CXXFLAGS) $(BOOSTFLAGS)  -c main.cpp

HttpServer.o: HttpServer.cpp
	$(CXX) $(CXXFLAGS) $(BOOSTFLAGS) -c HttpServer.cpp

HttpHandler.o: HttpHandler.cpp
	$(CXX) -c HttpHandler.cpp

ThreadPool.o: ThreadPool.cpp
	$(CXX) $(CXXFLAGS) $(BOOSTFLAGS) -c ThreadPool.cpp

Logger.o: Logger.cpp
	$(CXX) $(CXXFLAGS) $(BOOSTFLAGS) -c Logger.cpp

clean:
	rm -f *.o server