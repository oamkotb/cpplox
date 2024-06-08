#include "utils.h"
#include <iostream>

namespace Lox {
    bool had_error = false;

    void error(int line, std::string message){
        report(line, "", message);
    }

    void report(int line, std::string where, std::string message){
        had_error = true;
        std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
    }
}

