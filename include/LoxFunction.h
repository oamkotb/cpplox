#pragma once

#include "Environment.h"
#include "LoxCallable.h"

/**
 * @brief Represents a user-defined function in Lox.
 *
 * This class handles the behavior of a function defined by the user in the 
 * Lox programming language. It implements the `LoxCallable` interface, allowing 
 * the function to be called by the interpreter.
 */
class LoxFunction : public LoxCallable
{
public:
  /**
   * @brief Constructs a `LoxFunction` with a given declaration and closure.
   * 
   * @param declaration The function declaration containing the parameters and body.
   * @param closure The surrounding environment (closure) where the function was defined.
   */
  LoxFunction(const Stmt<LiteralValue>::Function& declaration, const Environment& closure)
    : _declaration(declaration), _closure(closure) {}
  
  /**
   * @brief Returns the number of arguments the function expects.
   * 
   * @return The arity (number of parameters) of the function.
   */
  size_t arity() override { return _declaration.params.size(); }

  /**
   * @brief Executes the function by calling it with the provided arguments.
   * 
   * @param interpreter The interpreter instance to execute the function.
   * @param arguments The list of arguments passed to the function.
   * @return The return value of the function or `std::monostate` if none.
   */
  LiteralValue call(Interpreter& interpreter, const std::vector<LiteralValue> arguments) override
  {
    Environment environment = _closure;
    
    for (size_t i = 0; i < _declaration.params.size(); ++i)
      environment.define(_declaration.params[i].lexeme, arguments[i]);
    try
    {
      interpreter.executeBlock(_declaration.body, environment);
    }
    catch(const Return& return_value)
    {
      return return_value.value;
    }
  
    return std::monostate();
  }

  /**
   * @brief Returns a string representation of the function.
   * 
   * @return A string indicating that this is a function and showing its name.
   */
  std::string toString() override { return "<fn " + _declaration.name.lexeme + ">"; }
  
private:
  const Stmt<LiteralValue>::Function& _declaration; ///< The function's declaration (parameters and body).
  const Environment _closure; ///< The environment in which the function was created (its closure).
};
