#pragma once

#include "Environment.h"
#include "LoxCallable.h"

/**
 * CHANGE THIS COMMENT
 */
class LoxFunction : public LoxCallable
{
public:
  /**
   * CHANGE THIS COMMENT
   */
  LoxFunction(const Stmt<LiteralValue>::Function& declaration)
    : _declaration(declaration) {}
  
  /**
   * CHANGE THIS COMMENT
   */
  size_t arity() override { return _declaration.params.size(); }

  /**
   * CHANGE THIS COMMENT
   */
  LiteralValue call(Interpreter& interpreter, const std::vector<LiteralValue> arguments) override
  {
    Environment environment(interpreter.globals);

    for (size_t i = 0; i < _declaration.params.size(); ++i)
      environment.define(_declaration.params[i].lexeme, arguments[i]);
    
    interpreter.executeBlock(_declaration.body, environment);

    return std::monostate();
  }

  /**
   * CHANGE THIS COMMENT
   */
  std::string toString() override { return "<fn " + _declaration.name.lexeme + ">"; }
  
private:
  const Stmt<LiteralValue>::Function& _declaration; ///< CHANGE THIS COMMENT

};