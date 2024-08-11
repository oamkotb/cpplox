#pragma once

#include <iostream>
#include <string>
#include <variant>
#include <vector>
#include <memory>

#include "Environment.h"
#include "Expr.h"
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
   * @brief Visits a literal expression and returns its value.
   * 
   * @param expr The literal expression to evaluate.
   * @return The value of the literal expression.
   */
  LiteralValue visitLiteralExpr(const Expr<LiteralValue>::Literal& expr) override;

  /**
   * @brief Visits a grouping expression and evaluates the expression inside the group.
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
   * @brief Visits a conditional expression (ternary operator) and evaluates it.
   * 
   * @param expr The conditional expression to evaluate.
   * @return The result of evaluating the conditional expression.
   */
  LiteralValue visitConditionalExpr(const Expr<LiteralValue>::Conditional& expr) override;

  /**
   * @brief Visits a variable expression and returns its value from the environment.
   * 
   * @param expr The variable expression to evaluate.
   * @return The value of the variable.
   */
  LiteralValue visitVariableExpr(const Expr<LiteralValue>::Variable& expr) override;

  /**
   * @brief Visits an expression statement and evaluates the expression.
   * 
   * @param stmt The expression statement to execute.
   * @return The result of evaluating the expression.
   */
  LiteralValue visitExpressionStmt(const Stmt<LiteralValue>::Expression& stmt) override;

  /**
   * @brief Visits a print statement and evaluates the expression, then prints its value.
   * 
   * @param stmt The print statement to execute.
   * @return The result of evaluating the print statement.
   */
  LiteralValue visitPrintStmt(const Stmt<LiteralValue>::Print& stmt) override;

  /**
   * @brief Visits a variable declaration statement and adds the variable to the environment.
   * 
   * @param stmt The variable declaration statement to execute.
   * @return The result of executing the variable declaration statement.
   */
  LiteralValue visitVarStmt(const Stmt<LiteralValue>::Var& stmt) override;

private:
  Environment _environment;  ///< The environment that stores variables and their values.

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