#pragma once

#include "Token.h"

template <class R>
class Expr
{
public:
  class Binary;
  class Grouping;
  class Literal;
  class Unary;

  struct Visitor
  {
    virtual R visitBinaryExpr(const Binary& expr) = 0;
    virtual R visitGroupingExpr(const Grouping& expr) = 0;
    virtual R visitLiteralExpr(const Literal& expr) = 0;
    virtual R visitUnaryExpr(const Unary& expr) = 0;
  };

  virtual R accept(Visitor& visitor) = 0;

  class Binary : public Expr<R>
  {
  public:
    Binary(const Expr*& left, const Token& oper, const Expr*& right):
      left(left), oper(oper), right(right) {}
    
    R accept(const Visitor& visitor) override
    {
      return visitor.visitBinaryExpr(this);
    }

    const Expr* left;
    const Token oper;
    const Expr* right;
  };

  class Grouping : public Expr<R>
  {
  public:
    Grouping(const Expr*& expression):
      expression(expression) {}
    
    R accept(const Visitor& visitor) override
    { 
      return visitor.visitGroupingExpr(this);
    }

    const Expr* expression;
  };

  class Literal : public Expr<R>
  {
  public:
    Literal(const Token::LiteralValue& value):
      value(value) {}
    
    R accept(const Visitor& visitor) override
    {
      return visitor.visitLiteralExpr(this);
    }

    const Token::LiteralValue value;
  };

  class Unary : public Expr<R>
  {
  public:
    Unary(const Token& oper, const Expr*& right):
      oper(oper), right(right) {}
    
    R accept(const Visitor& visitor) override
    {
      return visitor.visitUnaryExpr(this);
    }

    const Token oper;
    const Expr* right;
  };
};
