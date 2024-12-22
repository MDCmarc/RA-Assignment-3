CXX = g++
CXXFLAGS = -std=c++17 -fopenmp -Wall -Wextra -O2

# Source files
#SRCS = utils/main.cpp
SRCS = main.cpp
# Object files
OBJS = $(SRCS:.cpp=.o)
# Output binary
TARGET = Cardinality

# Default target
all: $(TARGET)

# Link the object files to create the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile each .cpp file to .o file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up the build artifacts
clean:
	rm -f $(TARGET) $(OBJS)

# Phony targets
.PHONY: all clean