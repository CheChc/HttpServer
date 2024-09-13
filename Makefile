CXX = g++
CXXFLAGS = -std=c++11 -pthread
BOOSTFLAGS = -lboost_system -lboost_thread

TARGET = server
SRC = main.cpp ThreadPool.cpp HttpServer.cpp
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ) $(BOOSTFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

