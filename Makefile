# Define the compiler to use
CC=g++

# Define any compile-time flags
CFLAGS=-Wall -g

# Define any directories containing header files other than /usr/include
INCLUDES=-Iinclude

# Define the C source files
SRCS=$(wildcard src/*.cpp)

# Define the C object files 
OBJS=$(SRCS:src/%.cpp=build/%.o)

# Define the executable file 
MAIN=build/cpplox

.PHONY: clean

all: $(MAIN)
	@echo  Compiling cpplox...

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS)

# This is a suffix replacement rule for building .o's from .cpp's
# It uses automatic variables $<: the name of the prerequisite of 
# the rule(a .cpp file) and $@: the name of the target of the rule (a .o file)
# (see the gnu make manual section about automatic variables)
build/%.o: src/%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) build/* $(MAIN)
