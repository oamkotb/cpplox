#pragma once 

#include <iostream>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "Expr.h"

/**
 * @brief A class to print the abstract syntax tree (AST) of expressions.
 * 
 * This class implements the Visitor pattern to traverse and print the AST nodes.
 */
class AstPrinter : public Expr<std::string>::Visitor
{
public:
  /**
   * @brief Print the given expression as a string.
   * 
   * @param expr The expression to print.
   * @return A string representation of the expression.
   */
  std::string print(const Expr<std::string>& expr)
  {
    return expr.accept(*this);
  }

  /**
   * @brief Visit and print a binary expression.
   * 
   * @param expr The binary expression to visit.
   * @return A string representation of the binary expression.
   */
  std::string visitBinaryExpr(const Expr<std::string>::Binary& expr) override
  {
    return parenthesize(expr.oper.lexeme, { expr.left, expr.right });
  }

  /**
   * @brief Visit and print a grouping expression.
   * 
   * @param expr The grouping expression to visit.
   * @return A string representation of the grouping expression.
   */
  std::string visitGroupingExpr(const Expr<std::string>::Grouping& expr) override
  {
    return parenthesize("group", { expr.expression });
  }

  /**
   * @brief Visit and print a literal expression.
   * 
   * @param expr The literal expression to visit.
   * @return A string representation of the literal expression.
   */
  std::string visitLiteralExpr(const Expr<std::string>::Literal& expr) override
  {
    if (std::holds_alternative<std::string>(expr.value))
      return std::get<std::string>(expr.value);
    else if (std::holds_alternative<double>(expr.value))
      return std::to_string(std::get<double>(expr.value));
    return "null";
  }

  /**
   * @brief Visit and print a unary expression.
   * 
   * @param expr The unary expression to visit.
   * @return A string representation of the unary expression.
   */
  std::string visitUnaryExpr(const Expr<std::string>::Unary& expr) override
  {
    return parenthesize(expr.oper.lexeme, { expr.right });
  }

  /**
   * @brief Visit and print a conditional expression.
   * 
   * @param expr The conditional expression to visit.
   * @return A string representation of the conditional expression.
   */
  std::string visitConditionalExpr(const Expr<std::string>::Conditional& expr) override
  {
    return parenthesize("?:", { expr.condition, expr.then_branch, expr.else_branch });
  }

private:
  /**
   * @brief Parenthesize the expression with the given name and list of sub-expressions.
   * 
   * @param name The name of the expression.
   * @param exprs The sub-expressions to parenthesize.
   * @return A string representation of the parenthesized expression.
   */
  std::string parenthesize(const std::string& name, const std::vector<const Expr<std::string>*>& exprs)
  {
    std::ostringstream out;
    out << "(" << name;
    for (const Expr<std::string>* expr : exprs)
      out << " " << expr->accept(*this);
    out << ")";
    return out.str();
  }
};
