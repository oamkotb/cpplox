#pragma once

#include <iostream>

bool had_error = false;

inline void error(int line, std::string message)
{
  report(line, "", message);
}

void report(int line, std::string where, std::string message)
{
  had_error = true;
  std::cerr << "[line ] " << line << "] Error" << where << ": " << message << std::endl;
}