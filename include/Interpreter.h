#pragma once

#include <iostream>
#include <string>
#include <variant>

#include "Expr.h"
#include "RuntimeError.h"
#include "Token.h"
#include "utils.h"

class Interpreter : Expr<LiteralValue>::Visitor
{
public:
  LiteralValue visitBinaryExpr(const Expr<LiteralValue>::Binary& expr) override;
  LiteralValue visitLiteralExpr(const Expr<LiteralValue>::Literal& expr) override;
  LiteralValue visitGroupingExpr(const Expr<LiteralValue>::Grouping& expr) override;
  LiteralValue visitUnaryExpr(const Expr<LiteralValue>::Unary& expr) override;
  LiteralValue visitConditionalExpr(const Expr<LiteralValue>::Conditional& expr) override;

  void interpret(const Expr<LiteralValue>* expr);

private:
  LiteralValue evaluate(const Expr<LiteralValue>* expr);
  std::string stringify(const LiteralValue& value);  
  
  bool isTruthy(const LiteralValue& value);
  bool isEqual(const LiteralValue& a, const LiteralValue& b);

  void checkNumberOperand(const Token& oper, const LiteralValue& operand);
  void checkNumberOperands(const Token& oper, const LiteralValue& left, const LiteralValue& right);
};