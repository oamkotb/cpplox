
# CppLox

CppLox is a C++ implementation of the JLox compiler, offering full functional parity with the original. It serves as a Tree-Walk Interpreter for Lox, a dynamically typed, interpreted scripting language designed by Robert Nystrom for his book [Crafting Interpreters](https://craftinginterpreters.com/).

## Getting Started
### Prerequisites
* C++17 or later
* GCC compiler

## Building the Project

1. Clone the repository:
```bash
git clone https://github.com/oamkotb/cpplox
```

2. Navigate to the project directory:
```bash
cd cpplox
```

3. Run the Makefile:
```bash
make
```

## Running the Project
Run interactive prompt:
```bash
./build/cpplox
```

Run a Lox file:
```bash
./build/cpplox [lox file]
```
