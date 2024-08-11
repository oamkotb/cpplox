#pragma once

#include <string>
#include <unordered_map>

#include "Token.h"

class Environment
{
public:
  void define(const std::string& name, const LiteralValue& value);
  LiteralValue get(const Token& name);

private:
  std::unordered_map<std::string, LiteralValue> _values;
};