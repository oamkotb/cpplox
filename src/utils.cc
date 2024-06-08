#include "utils.h"
#include <iostream>

void Lox::error(int line, std::string message)
{
  report(line, "", message);
}

void Lox::report(int line, std::string where, std::string message)
{
  had_error = true;
  std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
}

