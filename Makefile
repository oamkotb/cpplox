# Define the compiler to use
CC=g++

# Define any compile-time flags
CFLAGS=-Wall -Wextra -g -std=c++20  # Added C++20 standard flag and -Wextra

# Define any directories containing header files other than /usr/include
INCLUDES=-Iinclude

# Define the C source files
SRCS=$(wildcard src/*.cc)

# Define the C object files 
OBJS=$(SRCS:src/%.cc=build/%.o)  # Changed from %.cpp to %.cc

# Define the executable file 
MAIN=build/cpplox

.PHONY: clean debug

all: $(MAIN) clean_objs
	@echo  Compiling cpplox...

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS)

build/%.o: src/%.cc  # Changed from %.cpp to %.cc
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) build/* $(MAIN)

clean_objs:
	$(RM) $(OBJS)

debug:
	@echo "CC: $(CC)"
	@echo "CFLAGS: $(CFLAGS)"
	@echo "INCLUDES: $(INCLUDES)"
	@echo "SRCS: $(SRCS)"
	@echo "OBJS: $(OBJS)"
	@echo "MAIN: $(MAIN)"
