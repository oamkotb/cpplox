#pragma once

#include <iostream>
#include <string>
#include <variant>
#include <vector>
#include <memory>

#include "Environment.h"
#include "Expr.h"
#include "JumpExceptions.h"
#include "RuntimeError.h"
#include "Stmt.h"
#include "Token.h"
#include "utils.h"

/**
 * @class Interpreter
 * @brief The Interpreter class is responsible for evaluating expressions and executing statements.
 * 
 * The Interpreter class implements the Visitor pattern for both expressions and statements,
 * allowing different types of expressions and statements to be evaluated and executed.
 */
class Interpreter : public Expr<LiteralValue>::Visitor, public Stmt<LiteralValue>::Visitor
{
public:
  Environment globals; ///< The global environment that stores global variables and their values.
  Environment environment;  ///< The environment that stores variables and their values.

/**
 * @brief Initializes the interpreter's global environment and sets up built-in functions.
 */
  Interpreter();

  /**
   * @brief Interprets a series of statements.
   * 
   * @param statements A vector of shared pointers to statements to be interpreted.
   */
  void interpret(const std::vector<std::shared_ptr<Stmt<LiteralValue>>>& statements);

  /**
   * @brief Visits a binary expression and evaluates it.
   * 
   * @param expr The binary expression to evaluate.
   * @return The result of evaluating the binary expression.
   */
  LiteralValue visitBinaryExpr(const Expr<LiteralValue>::Binary& expr) override;

  /**
   * @brief Evaluates a function or class call expression.
   *
   * @param expr The call expression to evaluate, containing the callee, parentheses for validation, and the arguments.
   * @return The result of calling the callable with the provided arguments.
   * @throws RuntimeError If the callee is not a function or class, or if the argument count is incorrect.
   */
  LiteralValue visitCallExpr(const Expr<LiteralValue>::Call& expr) override;

  /**
   * @brief Visits a literal expression and returns its value.
   * 
   * @param expr The literal expression to evaluate.
   * @return The value of the literal expression.
   */
  LiteralValue visitLiteralExpr(const Expr<LiteralValue>::Literal& expr) override;

  /**
   * @brief Evaluates a logical expression.
   * @param expr The logical expression to be evaluated.
   * @return The resulting LiteralValue after evaluating the logical expression.
   */
  LiteralValue visitLogicalExpr(const Expr<LiteralValue>::Logical& expr) override;

  /**
   * @brief Visit s a grouping expression and evaluates the expression inside the group.
   * 
   * @param expr The grouping expression to evaluate.
   * @return The result of evaluating the grouped expression.
   */
  LiteralValue visitGroupingExpr(const Expr<LiteralValue>::Grouping& expr) override;

  /**
   * @brief Visits a unary expression and evaluates it.
   * 
   * @param expr The unary expression to evaluate.
   * @return The result of evaluating the unary expression.
   */
  LiteralValue visitUnaryExpr(const Expr<LiteralValue>::Unary& expr) override;

  /**
   * @brief Visits a ternary expression (ternary operator) and evaluates it.
   * 
   * @param expr The ternary expression to evaluate.
   * @return The result of evaluating the ternary expression.
   */
  LiteralValue visitTernaryExpr(const Expr<LiteralValue>::Ternary& expr) override;

  /**
   * @brief Visits a variable expression and returns its value from the environment.
   * 
   * @param expr The variable expression to evaluate.
   * @return The value of the variable.
   */
  LiteralValue visitVariableExpr(const Expr<LiteralValue>::Variable& expr) override;

  /**
   * @brief Visits an assignment expression and updates the variable value in the environment.
   * 
   * Assigns a new value to a variable and updates its value in the environment.
   * 
   * @param expr The assignment expression to evaluate.
   * @return The result of the assignment operation.
   */
  LiteralValue visitAssignExpr(const Expr<LiteralValue>::Assign& expr) override;


  /**
   * @brief Visits a block statement and executes all statements in the block.
   * 
   * Executes a block of statements in a new environment that is a child of the current environment.
   * 
   * @param stmt The block statement to execute.
   * @return A `std::monostate` indicating that a statement does not return a value.
   */
  LiteralValue visitBlockStmt(const Stmt<LiteralValue>::Block& stmt) override;

  /**
   * @brief Visits an expression statement and evaluates the expression.
   * 
   * @param stmt The expression statement to execute.
   * @return  A `std::monostate` indicating that a statement does not return a value.
   */
  LiteralValue visitExpressionStmt(const Stmt<LiteralValue>::Expression& stmt) override;

  /**
   * @brief Evaluates a function declaration statement.
   * 
   * @param stmt The function declaration statement, containing the function's name and its body.
   * @return Always returns `std::monostate()` since function declarations don't produce a runtime value.
   */
  LiteralValue visitFunctionStmt(const Stmt<LiteralValue>::Function& stmt) override;

  /**
   * @brief Evaluates an if statement.
   * @param stmt The if statement to be evaluated.
   * @return A `std::monostate` indicating that the if statement does not return a value.
   */
  LiteralValue visitIfStmt(const Stmt<LiteralValue>::If& stmt) override;

  /**
   * @brief Visits a print statement and evaluates the expression, then prints its value.
   * 
   * @param stmt The print statement to execute.
   * @return A `std::monostate` indicating that a statement does not return a value.
   */
  LiteralValue visitPrintStmt(const Stmt<LiteralValue>::Print& stmt) override;

  /**
   * @brief Evaluates a return statement and exits the current function.
   *
   * @param stmt The return statement, containing an optional return value expression.
   * @throw Return Thrown with the evaluated return value, signaling a return from the function.
   * @return This method does not return a value since it throws an exception to exit the function.
   */
  LiteralValue visitReturnStmt(const Stmt<LiteralValue>::Return& stmt) override;

  /**
   * @brief Visits a variable declaration statement and adds the variable to the environment.
   * 
   * @param stmt The variable declaration statement to execute.
   * @return A `std::monostate` indicating that a statement does not return a value.
   */
  LiteralValue visitVarStmt(const Stmt<LiteralValue>::Var& stmt) override;

  /**
   * @brief Evaluates a while loop statement.
   * @param stmt The while statement to be evaluated.
   * @return A `std::monostate` indicating that the while statement does not return a value.
   */
  LiteralValue visitWhileStmt(const Stmt<LiteralValue>::While& stmt) override;

  /**
   * @brief Executes a jump statement, such as `break` or `continue`.
   *
   *
   * @param stmt The jump statement to be executed, containing the jump keyword.
   * @return A `LiteralValue`, though this method typically throws an exception before returning.
   *
   * @throws ContinueException If the jump statement is a `continue`.
   * @throws BreakException If the jump statement is a `break`.
   */
  LiteralValue visitJumpStmt(const Stmt<LiteralValue>::Jump& stmt) override;

  /**
   * @brief Executes a block of statements in a new environment.
   * 
   * Executes a sequence of statements within a new environment, which is a child of the current environment.
   * 
   * @param statements The statements to execute.
   * @param environment The environment to use for executing the block.
   */
  void executeBlock(const std::vector<std::shared_ptr<const Stmt<LiteralValue>>>& statements, const Environment& environment);
  
private:
  /**
   * @brief Evaluates an expression.
   * 
   * @param expr A shared pointer to the expression to evaluate.
   * @return The result of evaluating the expression.
   */
  LiteralValue evaluate(const std::shared_ptr<const Expr<LiteralValue>>& expr);

  /**
   * @brief Executes a statement.
   * 
   * @param stmt A shared pointer to the statement to execute.
   * @return The result of executing the statement.
   */
  LiteralValue execute(const std::shared_ptr<const Stmt<LiteralValue>>& stmt);

  /**
   * @brief Converts a literal value to a string for printing.
   * 
   * @param value The literal value to convert.
   * @return The string representation of the literal value.
   */
  std::string stringify(const LiteralValue& value);

  /**
   * @brief Checks if a literal value is truthy (i.e., true in a boolean context).
   * 
   * @param value The literal value to check.
   * @return True if the value is truthy, otherwise false.
   */
  bool isTruthy(const LiteralValue& value);

  /**
   * @brief Checks if two literal values are equal.
   * 
   * @param a The first literal value to compare.
   * @param b The second literal value to compare.
   * @return True if the values are equal, otherwise false.
   */
  bool isEqual(const LiteralValue& a, const LiteralValue& b);

  /**
   * @brief Ensures that a literal value is a number, throwing an error if not.
   * 
   * @param oper The operator token that expects a number.
   * @param operand The literal value to check.
   */
  void checkNumberOperand(const Token& oper, const LiteralValue& operand);

  /**
   * @brief Ensures that two literal values are numbers, throwing an error if not.
   * 
   * @param oper The operator token that expects numbers.
   * @param left The first literal value to check.
   * @param right The second literal value to check.
   */
  void checkNumberOperands(const Token& oper, const LiteralValue& left, const LiteralValue& right);
};