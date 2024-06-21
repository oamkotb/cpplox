#pragma once

#include "Token.h"

template <class R>
class Expr
{
  struct Visitor
  {
    R visitBinaryExpr(const Binary& expr);
    R visitGroupingExpr(const Grouping& expr);
    R visitLiteralExpr(const Literal& expr);
  };

  virtual R accept(Visitor visitor);

  class Binary
  {
  public:
    Binary(const Expr*& left, const Token& oper, const Expr*& right):
      left(left), oper(oper), right(right) {}
    
    R accept(const Visitor& visitor) { return visitior.visitBinaryExpr(this); }

    const Expr* left;
    const Token oper;
    const Expr* right;
  };

  class Grouping
  {
  public:
    Grouping(const Expr*& expression):
      expression(expression) {}
    
    R accept(const Visitor& visitor) { return visitor.visitGroupingExpr(this); }

    const Expr* expression;
  };

  class Literal
  {
  public:
    Literal(const Token::LiteralValue& value):
      value(value) {}
    
    R accept(const Visitor& visitor) { return visitor.visitLiteralExpr(this); }
    const Token::LiteralValue value;
  };

  class Unary
  {
  public:
    Unary(const Token& oper, const Expr*& right):
      oper(oper), right(right) {}
    
    R accept(const Visitor& visitor) { return visitor.visitUnaryExpr(this); }

    const Token oper;
    const Expr* right;
  };
};
