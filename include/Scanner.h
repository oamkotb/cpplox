#pragma once

#include <vector>

#include "Token.h"

class Scanner
{
public:
  Scanner(std::string source);
  ~Scanner();

  void scanTokens(std::vector<Token> &tokens);
  
private:
  std::string source;
};
