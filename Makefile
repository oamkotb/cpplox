# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Iinclude

# Source files
SRC = $(wildcard src/*.cc)

# Object files
OBJ = $(SRC:.cc=.o)

# Output binary
TARGET = build/cpplox

# Rules
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
