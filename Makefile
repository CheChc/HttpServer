# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -pthread -Wall

# Boost libraries
LIBS = -lboost_system -lboost_thread

# Source files
SRCS = main.cpp HttpHandler.cpp HttpServer.cpp GetRequestHandler.cpp PostRequestHandler.cpp ThreadPool.cpp Logger.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable
TARGET = server

# Default rule to build the executable
all: $(TARGET)

# Rule to build the target
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Rule to compile each source file into an object file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up the build
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
