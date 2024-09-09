#pragma once

#include <chrono>
#include <string>
#include <vector>

#include "Interpreter.h"
#include "Token.h"

/**
 * CHANGE THIS COMMENT
 */
class LoxCallable
{
public:
  virtual ~LoxCallable() = default;

  /**
   * CHANGE THIS COMMENT
   */
  virtual size_t arity() { return 0; }

  /**
   * CHANGE THIS COMMENT
   */
  virtual LiteralValue call(Interpreter&, const std::vector<LiteralValue>) = 0;

  /**
   * CHANGE THIS COMMENT
   */
  virtual std::string toString() { return "<callable>"; }
};

/**
 * CHANGE THIS COMMENT
 */
class ClockCallable : public LoxCallable
{
  /**
   * CHANGE THIS COMMENT
   */
  size_t arity() override { return 0; }
  
  
  /**
   * CHANGE THIS COMMENT
   */
  LiteralValue call(Interpreter&, const std::vector<LiteralValue>) override
  {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() / 1000.0;
  }
  
  /**
   * CHANGE THIS COMMENT
   */
  std::string toString() override { return "<native fn>"; }
};