#pragma once

#include <chrono>
#include <string>
#include <vector>

#include "Interpreter.h"
#include "Token.h"

class LoxCallable
{
public:
  virtual ~LoxCallable() = default;

  virtual size_t arity() { return 0; }
  virtual LiteralValue call() = 0;
  virtual LiteralValue call(const Interpreter& interpreter, const std::vector<LiteralValue> arguments) = 0;
  virtual std::string toString() { return "<callable>"; }
};

class ClockCallable : public LoxCallable
{
  size_t arity() override { return 0; }
  
  LiteralValue call() override
  {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() / 1000.0;
  }

  std::string toString() override { return "<native fn>"; }
};