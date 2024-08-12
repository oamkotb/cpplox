#include "Environment.h"
#include "RuntimeError.h"

/**
 * CHANGE THIS COMMENT
 */
void Environment::define(const std::string& name, const LiteralValue& value)
{
  _values.insert({name, value});
}

/**
 * CHANGE THIS COMMENT
 */
LiteralValue Environment::get(const Token& name)
{
  auto it = _values.find(name.lexeme);
  if (it != _values.end()) return it->second;

  if (_enclosing != nullptr) return _enclosing->get(name);

  throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}

/**
 * CHANGE THIS COMMENT
 */
void Environment::assign(const Token& name, const LiteralValue& value)
{
  auto it = _values.find(name.lexeme);
  if (it != _values.end())
  {
    it->second = value;
    return; 
  }

  if (_enclosing != nullptr)
  {
    _enclosing->assign(name, value);
    return;
  }

  throw RuntimeError(name, "Undenfined varaible '" + name.lexeme + "'.");
}
