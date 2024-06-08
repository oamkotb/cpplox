# Define the compiler to use
CC=g++

# Define any compile-time flags
CFLAGS=-Wall -g

# Define any directories containing header files other than /usr/include
INCLUDES=-Iinclude

# Define the C source files
SRCS=$(wildcard src/*.cc)

# Define the C object files 
OBJS=$(SRCS:src/%.cpp=build/%.o)

# Define the executable file 
MAIN=build/cpplox

.PHONY: clean debug

all: $(MAIN)
	@echo  Compiling cpplox...

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS)

build/%.o: src/%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) build/* $(MAIN)

debug:
	@echo "CC: $(CC)"
	@echo "CFLAGS: $(CFLAGS)"
	@echo "INCLUDES: $(INCLUDES)"
	@echo "SRCS: $(SRCS)"
	@echo "OBJS: $(OBJS)"
	@echo "MAIN: $(MAIN)"
