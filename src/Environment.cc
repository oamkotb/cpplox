#include "Environment.h"
#include "RuntimeError.h"

/**
 * @brief Defines a new variable in the environment with the given name and value.
 * 
 * @param name The name of the variable.
 * @param value The value to assign to the variable.
 */
void Environment::define(const std::string& name, const LiteralValue& value)
{
  _values.insert({name, value});
}

/**
 * @brief Retrieves the value of a variable from the environment.
 * 
 * Searches for the variable in the current environment. If not found, it checks the enclosing environments.
 * Throws a RuntimeError if the variable is undefined.
 * 
 * @param name The token representing the variable name.
 * @return The value of the variable.
 * @throws RuntimeError if the variable is not found in the current or enclosing environments.
 */
LiteralValue Environment::get(const Token& name)
{
  auto it = _values.find(name.lexeme);
  if (it != _values.end()) return it->second;

  if (_enclosing != nullptr) return _enclosing->get(name);

  throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}

/**
 * @brief Assigns a new value to an existing variable in the environment.
 * 
 * Updates the value of the variable in the current environment if it exists. If not, it updates the value in the enclosing environments.
 * Throws a RuntimeError if the variable is not found in the current or enclosing environments.
 * 
 * @param name The token representing the variable name.
 * @param value The new value to assign to the variable.
 * @throws RuntimeError if the variable is not found in the current or enclosing environments.
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

  throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}
