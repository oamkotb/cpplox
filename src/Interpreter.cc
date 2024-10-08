#include "Interpreter.h"

#include "LoxCallable.h"
#include "LoxFunction.h"

/**
 * @brief Initializes the interpreter's global environment and sets up built-in functions.
 */
Interpreter::Interpreter()
  : globals(Environment()), environment(globals)
{
  globals.define("clock", std::make_shared<ClockCallable>());
}

/**
 * @brief Interprets a series of statements.
 * 
 * @param statements A vector of shared pointers to statements to be interpreted.
 */
void Interpreter::interpret(const std::vector<std::shared_ptr<Stmt<LiteralValue>>>& statements)
{
  try
  {
    for (const auto& statement : statements)
      execute(statement);
  }
  catch (const RuntimeError& error)
  {
    Lox::runtimeError(error);
  }
}

/**
 * @brief Visits a binary expression and evaluates it.
 * 
 * @param expr The binary expression to evaluate.
 * @return The result of evaluating the binary expression.
 */
LiteralValue Interpreter::visitBinaryExpr(const Expr<LiteralValue>::Binary& expr)
{
  LiteralValue left = evaluate(expr.left);
  LiteralValue right = evaluate(expr.right);

  switch (expr.oper.type)
  {
    case GREATER:
      checkNumberOperands(expr.oper, left, right);
      return std::get<double>(left) > std::get<double>(right);

    case GREATER_EQUAL:
      checkNumberOperands(expr.oper, left, right);
      return std::get<double>(left) >= std::get<double>(right);

    case LESS:
      checkNumberOperands(expr.oper, left, right);
      return std::get<double>(left) < std::get<double>(right);

    case LESS_EQUAL:
      checkNumberOperands(expr.oper, left, right);
      return std::get<double>(left) <= std::get<double>(right);

    case EQUAL_EQUAL:
      return isEqual(left, right);

    case BANG_EQUAL:
      return !isEqual(left, right);

    case MINUS:
      checkNumberOperands(expr.oper, left, right);
      return std::get<double>(left) - std::get<double>(right);

    case PLUS:
      if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right))
        return std::get<std::string>(left) + std::get<std::string>(right);
      if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right))
        return std::get<double>(left) + std::get<double>(right);

      throw RuntimeError(expr.oper, "Operands must be two numbers or two strings.");

    case STAR:
      checkNumberOperands(expr.oper, left, right);
      return std::get<double>(left) * std::get<double>(right);

    case SLASH:
      checkNumberOperands(expr.oper, left, right);
      return std::get<double>(left) / std::get<double>(right);

    default:
      return std::monostate();
  }
}

/**
 * @brief Evaluates a function or class call expression.
 *
 * @param expr The call expression to evaluate, containing the callee, parentheses for validation, and the arguments.
 * @return The result of calling the callable with the provided arguments.
 * @throws RuntimeError If the callee is not a function or class, or if the argument count is incorrect.
 */
LiteralValue Interpreter::visitCallExpr(const Expr<LiteralValue>::Call& expr)
{
  LiteralValue callee = evaluate(expr.callee);

  std::vector<LiteralValue> arguments;
  for (auto argument : expr.arguments)
    arguments.push_back(evaluate(argument));

  if (!std::holds_alternative<std::shared_ptr<LoxCallable>>(callee))
    throw RuntimeError(expr.paren, "Can only call functions and classes.");

  std::shared_ptr<LoxCallable> function = std::get<std::shared_ptr<LoxCallable>>(callee);

  if (arguments.size() != function->arity())
    throw RuntimeError(expr.paren, "Expected " + 
      std::to_string(function->arity()) + " arguments, but got " +
      std::to_string(arguments.size()) + ".");

  return function->call(*this, arguments);
}

/**
 * @brief Visits a literal expression and returns its value.
 * 
 * @param expr The literal expression to evaluate.
 * @return The value of the literal expression.
 */
LiteralValue Interpreter::visitLiteralExpr(const Expr<LiteralValue>::Literal& expr)
{
  return expr.value;
}

/**
 * @brief Evaluates a logical expression. 
 * 
 * @param expr The logical expression to be evaluated.
 * @return The resulting LiteralValue after evaluating the logical expression.
 */
LiteralValue Interpreter::visitLogicalExpr(const Expr<LiteralValue>::Logical& expr)
{
  LiteralValue left = evaluate(expr.left);

  if (expr.oper.type == TokenType::OR)
  {
    if (isTruthy(left)) return left;
  }
  else
  {
    if (!isTruthy(left)) return left;
  }
  return evaluate(expr.right);
}
  
/**
 * @brief Visits a grouping expression and evaluates the expression inside the group.
 * 
 * @param expr The grouping expression to evaluate.
 * @return The result of evaluating the grouped expression.
 */
LiteralValue Interpreter::visitGroupingExpr(const Expr<LiteralValue>::Grouping& expr)
{
  return evaluate(expr.expression);
}

/**
 * @brief Visits a unary expression and evaluates it.
 * 
 * @param expr The unary expression to evaluate.
 * @return The result of evaluating the unary expression.
 */
LiteralValue Interpreter::visitUnaryExpr(const Expr<LiteralValue>::Unary& expr)
{
  LiteralValue right = evaluate(expr.right);

  switch (expr.oper.type)
  {
    case BANG:
      return !isTruthy(right);
    case MINUS:
      checkNumberOperand(expr.oper, right);
      return -std::get<double>(right);

    default:
      return std::monostate();
  }
}

/**
 * @brief Visits a ternary expression (ternary operator) and evaluates it.
 * 
 * @param expr The ternary expression to evaluate.
 * @return The result of evaluating the ternary expression.
 */
LiteralValue Interpreter::visitTernaryExpr(const Expr<LiteralValue>::Ternary& expr)
{
  LiteralValue condition = evaluate(expr.condition);

  if (isTruthy(condition))
    return evaluate(expr.then_branch);

  return evaluate(expr.else_branch);
}

/**
 * @brief Visits a variable expression and returns its value from the environment.
 * 
 * @param expr The variable expression to evaluate.
 * @return The value of the variable.
 */
LiteralValue Interpreter::visitVariableExpr(const Expr<LiteralValue>::Variable& expr)
{
  return environment.get(expr.name);
}

/**
 * @brief Visits an assignment expression and updates the variable value in the environment.
 * 
 * Assigns a new value to a variable and updates its value in the environment.
 * 
 * @param expr The assignment expression to evaluate.
 * @return The result of the assignment operation.
 */
LiteralValue Interpreter::visitAssignExpr(const Expr<LiteralValue>::Assign& expr)
{
  LiteralValue value = evaluate(expr.value);
  environment.assign(expr.name, value);

  return value;
}

/**
 * @brief Visits a block statement and executes all statements in the block.
 * 
 * Executes a block of statements in a new environment that is a child of the current environment.
 * 
 * @param stmt The block statement to execute.
 * @return A `std::monostate` indicating that a statement does not return a value.
 */
LiteralValue Interpreter::visitBlockStmt(const Stmt<LiteralValue>::Block& stmt)
{
  executeBlock(stmt.statements, Environment(std::make_shared<Environment>(environment)));  
  return std::monostate();
}

/**
 * @brief Visits an expression statement and evaluates the expression.
 * 
 * @param stmt The expression statement to execute.
 * @return  A `std::monostate` indicating that the expression statement does not return a value.
 */
LiteralValue Interpreter::visitExpressionStmt(const Stmt<LiteralValue>::Expression& stmt)
{
  evaluate(stmt.expression);
  return std::monostate();
}

/**
 * @brief Evaluates a function declaration statement.
 * 
 * @param stmt The function declaration statement, containing the function's name and its body.
 * @return Always returns `std::monostate()` since function declarations don't produce a runtime value.
 */
LiteralValue Interpreter::visitFunctionStmt(const Stmt<LiteralValue>::Function& stmt)
{
  LoxFunction function(stmt, environment);
  environment.define(stmt.name.lexeme, std::make_shared<LoxFunction>(function));

  return std::monostate();
}

/**
 * @brief Evaluates an if statement.
 * 
 * @param stmt The if statement to be evaluated.
 * @return A `std::monostate` indicating that the if statement does not return a value.
 */
LiteralValue Interpreter::visitIfStmt(const Stmt<LiteralValue>::If& stmt)
{
  if (isTruthy(evaluate(stmt.condition)))
    execute(stmt.then_branch);
  else if (stmt.else_branch != nullptr)
    execute(stmt.else_branch);
  
  return std::monostate();
}

/**
 * @brief Visits a print statement and evaluates the expression, then prints its value.
 * 
 * @param stmt The print statement to execute.
 * @return A `std::monostate` indicating that a statement does not return a value.
 */
LiteralValue Interpreter::visitPrintStmt(const Stmt<LiteralValue>::Print& stmt)
{
  LiteralValue value = evaluate(stmt.expression);
  std::cout << stringify(value) << std::endl;
  return std::monostate();
}

/**
 * @brief Evaluates a return statement and exits the current function.
 *
 * @param stmt The return statement, containing an optional return value expression.
 * @throw Return Thrown with the evaluated return value, signaling a return from the function.
 * @return This method does not return a value since it throws an exception to exit the function.
 */
LiteralValue Interpreter::visitReturnStmt(const Stmt<LiteralValue>::Return& stmt)
{
  LiteralValue value = std::monostate();

  if (stmt.value != nullptr)
    value = evaluate(stmt.value);
  
  throw Return(value);
}

/**
 * @brief Visits a variable declaration statement and adds the variable to the environment.
 * 
 * @param stmt The variable declaration statement to execute.
 * @return A `std::monostate` indicating that a statement does not return a value.
 */
LiteralValue Interpreter::visitVarStmt(const Stmt<LiteralValue>::Var& stmt)
{
  LiteralValue value = std::monostate();
  if (stmt.initializer != nullptr)
    value = evaluate(stmt.initializer);

  environment.define(stmt.name.lexeme, value);
  return std::monostate();
}

/**
 * @brief Evaluates a while loop statement.
 * 
 * @param stmt The while statement to be evaluated.
 * @return A `std::monostate` indicating that the while statement does not return a value.
 */
LiteralValue Interpreter::visitWhileStmt(const Stmt<LiteralValue>::While& stmt)
{
  while (isTruthy(evaluate(stmt.condition)))
  {
    try
    {
      execute(stmt.body);  
    }
    catch (const Continue&)
    {
      continue;
    }
    catch (const Break&)
    {
      break;
    }
  }
  return std::monostate();
}

/**
 * @brief Executes a jump statement, such as `break` or `continue`.
 *
 * @param stmt The jump statement to be executed, containing the jump keyword.
 * @return A `LiteralValue`, though this method typically throws an exception before returning.
 *
 * @throws Continue If the jump statement is a `continue`.
 * @throws Break If the jump statement is a `break`.
 */
LiteralValue Interpreter::visitJumpStmt(const Stmt<LiteralValue>::Jump& stmt)
{
  if (stmt.keyword.type == CONTINUE)
    throw Continue();
  
  throw Break();
}

/**
 * @brief Executes a block of statements in a new environment.
 * 
 * Executes a sequence of statements within a new environment, which is a child of the current environment.
 * 
 * @param statements The statements to execute.
 * @param environment The environment to use for executing the block.
 */
void Interpreter::executeBlock(const std::vector<std::shared_ptr<const Stmt<LiteralValue>>>& statements, const Environment& environment)
{
  EnvironmentGuard guard(this->environment, environment);
  for (const std::shared_ptr<const Stmt<LiteralValue>> statement : statements)
    execute(statement);
}

/**
 * @brief Evaluates an expression.
 * 
 * @param expr A shared pointer to the expression to evaluate.
 * @return The result of evaluating the expression.
 */
LiteralValue Interpreter::evaluate(const std::shared_ptr<const Expr<LiteralValue>>& expr)
{
  return expr->accept(*this);
}

/**
 * @brief Executes a statement.
 * 
 * @param stmt A shared pointer to the statement to execute.
 * @return The result of executing the statement.
 */
LiteralValue Interpreter::execute(const std::shared_ptr<const Stmt<LiteralValue>>& stmt)
{
  if (stmt == nullptr) return std::monostate();
  
  return stmt->accept(*this);
}

/**
 * @brief Converts a literal value to a string for printing.
 * 
 * @param value The literal value to convert.
 * @return The string representation of the literal value.
 */
std::string Interpreter::stringify(const LiteralValue& value)
{
  if (std::holds_alternative<double>(value))
  {
    std::string text = std::to_string(std::get<double>(value));
    if (text.size() > 2 && text.substr(text.size() - 2) == ".0")
      text = text.substr(0, text.size() - 2);

    return text;
  }

  if (std::holds_alternative<std::string>(value))
    return std::get<std::string>(value);

  if (std::holds_alternative<bool>(value))
    return std::get<bool>(value) ? "True" : "False";

  return "nil";
}

/**
 * @brief Checks if a literal value is truthy (i.e., true in a boolean context).
 * 
 * @param value The literal value to check.
 * @return True if the value is truthy, otherwise false.
 */
bool Interpreter::isTruthy(const LiteralValue& value)
{
  if (std::holds_alternative<std::monostate>(value)) return false;
  if (std::holds_alternative<bool>(value)) return std::get<bool>(value);

  return true;
}

/**
 * @brief Checks if two literal values are equal.
 * 
 * @param a The first literal value to compare.
 * @param b The second literal value to compare.
 * @return True if the values are equal, otherwise false.
 */
bool Interpreter::isEqual(const LiteralValue& a, const LiteralValue& b)
{
  if (std::holds_alternative<std::monostate>(a) && std::holds_alternative<std::monostate>(b)) return true;
  if (a.index() == b.index()) return a == b;

  return false;
}

/**
 * @brief Ensures that a literal value is a number, throwing an error if not.
 * 
 * @param oper The operator token that expects a number.
 * @param operand The literal value to check.
 */
void Interpreter::checkNumberOperand(const Token& oper, const LiteralValue& operand)
{
  if (std::holds_alternative<double>(operand)) return;

  throw RuntimeError(oper, "Operand must be a number.");
}

/**
 * @brief Ensures that two literal values are numbers, throwing an error if not.
 * 
 * @param oper The operator token that expects numbers.
 * @param left The first literal value to check.
 * @param right The second literal value to check.
 */
void Interpreter::checkNumberOperands(const Token& oper, const LiteralValue& left, const LiteralValue& right)
{
  if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) return;

  throw RuntimeError(oper, "Operands must be numbers.");
}
