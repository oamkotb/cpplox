#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Token.h"

class Environment
{
public:
  Environment()
    : _enclosing(nullptr) {}

  Environment(const std::shared_ptr<Environment>& enclosing)
    : _enclosing(enclosing) {}
    
  /**
   * @brief Defines a new variable in the environment with the given name and value.
   * 
   * @param name The name of the variable.
   * @param value The value to assign to the variable.
   */
  void define(const std::string& name, const LiteralValue& value);
  
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
  LiteralValue get(const Token& name);
  
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
  void assign(const Token& name, const LiteralValue& value);

private:
  friend class EnvironmentGuard;
  std::shared_ptr<Environment> _enclosing;
  std::unordered_map<std::string, LiteralValue> _values;
};

class EnvironmentGuard
{
public:
  EnvironmentGuard(Environment& current_env, const Environment& new_env)
    : _previous_env(std::move(current_env._enclosing)), _current_env(current_env)
  {
    _current_env._enclosing = std::move(std::make_shared<Environment>(new_env));
  }

  ~EnvironmentGuard()
  {
    _current_env._enclosing = std::move(_previous_env);  // Restore the previous environment
  }

private:
  std::shared_ptr<Environment> _previous_env;
  Environment& _current_env;
};
