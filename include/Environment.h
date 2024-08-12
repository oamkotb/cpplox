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
   * CHANGE THIS COMMENT
   */
  void define(const std::string& name, const LiteralValue& value);
  
  /**
   * CHANGE THIS COMMENT
   */
  LiteralValue get(const Token& name);
  
  /**
   * CHANGE THIS COMMENT
   */
  void assign(const Token& name, const LiteralValue& value);

private:
  std::shared_ptr<Environment> _enclosing;
  std::unordered_map<std::string, LiteralValue> _values;
};

class EnvironmentGuard
{
public:
  EnvironmentGuard(Environment& currentEnv, const Environment& newEnv)
    : _previousEnv(currentEnv), _currentEnv(currentEnv)
  {
    currentEnv = newEnv;
  }

  ~EnvironmentGuard()
  {
    _currentEnv = _previousEnv;  // Restore the previous environment
  }

private:
  const Environment _previousEnv;
  Environment& _currentEnv;
};