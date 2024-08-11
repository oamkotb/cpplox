#include "Environment.h"
#include "RuntimeError.h"

void Environment::define(const std::string& name, const LiteralValue& value)
{
  _values.insert({name, value});
}

LiteralValue Environment::get(const Token& name)
{
  auto it = _values.find(name.lexeme);
  if (it != _values.end())
    return it->second;

  throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}