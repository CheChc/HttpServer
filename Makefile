# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -pthread -Wall -Wextra -O2 -MMD -MP

# Boost libraries
LIBS = -lboost_system -lboost_thread
# macOS: 运行时按 rpath 找 boost dylib
RPATH = -Wl,-rpath,/usr/local/lib

# Source files
SRCS = main.cpp HttpHandler.cpp HttpServer.cpp GetRequestHandler.cpp PostRequestHandler.cpp ThreadPool.cpp Logger.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Auto-generated dependency files
DEPS = $(OBJS:.o=.d)

# Executable
TARGET = server

# Default rule to build the executable
all: $(TARGET)

# Rule to build the target
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS) $(RPATH)

# Rule to compile each source file into an object file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Include auto-generated dependency files
-include $(DEPS)

# Clean up the build
clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)

# Phony targets
.PHONY: all clean
