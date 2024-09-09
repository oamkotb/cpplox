#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Token.h"

/**
 * @brief Represents an environment for variable storage.
 *
 * The `Environment` class maintains a mapping of variable names to their values.
 * It supports variable definition, retrieval, and assignment. It also supports
 * nested environments through an enclosing environment.
 */
class Environment
{
public:
  /**
   * @brief Constructs a new environment with no enclosing environment.
   */
  Environment()
    : _enclosing(nullptr) {}

  /**
   * @brief Constructs a new environment with the specified enclosing environment.
   * 
   * @param enclosing A shared pointer to the enclosing environment.
   */
  Environment(const std::shared_ptr<Environment>& enclosing)
    : _enclosing(enclosing) {}

  /**
   * @brief Defines a new variable in the environment with the given name and value.
   * 
   * Adds a new variable to the environment or updates the value if the variable already exists.
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
  
  std::shared_ptr<Environment> _enclosing; ///< A shared pointer to the enclosing environment.
  std::unordered_map<std::string, LiteralValue> _values; ///< Map of variable names to their values.
};

/**
 * @brief Temporarily changes the current environment.
 *
 * The `EnvironmentGuard` class allows for temporarily changing the environment, typically for
 * evaluating a block of code, and then restores the previous environment after the block's execution.
 */
class EnvironmentGuard
{
public:
  /**
   * @brief Constructs an environment guard that changes the current environment.
   * 
   * @param current_env The current environment to be changed.
   * @param new_env The new environment to use temporarily.
   */
  EnvironmentGuard(Environment& current_env, const Environment& new_env)
    : _previous_env(std::move(current_env._enclosing)), _current_env(current_env)
  {
    _current_env._enclosing = std::move(std::make_shared<Environment>(new_env));
  }

  /**
   * @brief Restores the previous environment when the guard goes out of scope.
   */
  ~EnvironmentGuard()
  {
    _current_env._enclosing = std::move(_previous_env);  // Restore the previous environment
  }

private:
  std::shared_ptr<Environment> _previous_env; ///< The previous enclosing environment to restore.
  Environment& _current_env; ///< The current environment being managed by the guard.
};