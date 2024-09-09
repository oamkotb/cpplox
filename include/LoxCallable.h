#pragma once

#include <chrono>
#include <string>
#include <vector>

#include "Interpreter.h"
#include "Token.h"

/**
 * @brief Abstract base class for callable entities in Lox.
 *
 * This class represents any entity that can be called, such as functions or native
 * functions. It provides a common interface for calling and obtaining information
 * about the callable object.
 */
class LoxCallable
{
public:
  virtual ~LoxCallable() = default;

  /**
   * @brief Returns the number of arguments required by the callable.
   * 
   * @return The arity (number of parameters) of the callable.
   */
  virtual size_t arity() { return 0; }

  /**
   * @brief Calls the callable entity with the provided arguments.
   * 
   * @param interpreter The interpreter instance to use for executing the call.
   * @param arguments The arguments to pass to the callable.
   * @return The result of the callable's execution.
   */
  virtual LiteralValue call(Interpreter&, const std::vector<LiteralValue>) = 0;

  /**
   * @brief Returns a string representation of the callable.
   * 
   * @return A string describing the callable entity.
   */
  virtual std::string toString() { return "<callable>"; }
};

/**
 * @brief Represents a native function that returns the current time.
 *
 * This class implements a native callable function that returns the current
 * system time in seconds since the epoch. It is used for timing and other
 * related purposes.
 */
class ClockCallable : public LoxCallable
{
  /**
   * @brief Returns the number of arguments required by the `clock` function.
   * 
   * @return The arity of the `clock` function, which is 0.
   */
  size_t arity() override { return 0; }
  
  /**
   * @brief Calls the `clock` function to get the current time in seconds.
   * 
   * @param interpreter The interpreter instance (unused).
   * @param arguments The arguments passed to the function (unused).
   * @return The current system time in seconds as a `LiteralValue`.
   */
  LiteralValue call(Interpreter&, const std::vector<LiteralValue>) override
  {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() / 1000.0;
  }
  
  /**
   * @brief Returns a string representation of the `clock` function.
   * 
   * @return A string indicating that this is a native function.
   */
  std::string toString() override { return "<native fn>"; }
};