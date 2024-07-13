#pragma once

#include "Token.h"

template <class R>
class Expr
{
public:
  /// Virtual destructor to allow for proper cleanup of derived classes.
  virtual ~Expr() = default;

  class Binary;
  class Grouping;
  class Literal;
  class Unary;

  struct Visitor
  {
    virtual R visitBinaryExpr(const Expr<R>::Binary& expr) = 0;
    virtual R visitGroupingExpr(const Expr<R>::Grouping& expr) = 0;
    virtual R visitLiteralExpr(const Expr<R>::Literal& expr) = 0;
    virtual R visitUnaryExpr(const Expr<R>::Unary& expr) = 0;
  };

  virtual R accept(Visitor& visitor) const = 0;
};

template <class R>
class Expr<R>::Binary : public Expr<R>
{
public:
  Binary(const Expr<R>* left, const Token& oper, const Expr<R>* right)
      : left(left), oper(oper), right(right) {}

  /// Destructor to clean up the operands.
  ~Binary()
  {
    delete left;
    delete right;
  }

  R accept(Expr<R>::Visitor& visitor) const override
  {
    return visitor.visitBinaryExpr(*this);
  }

  const Expr<R>* left; ///< The left operand.
  const Token oper; ///< The operator token.
  const Expr<R>* right; ///< The right operand.
};

template <class R>
class Expr<R>::Grouping : public Expr<R>
{
public:
  Grouping(const Expr<R>* expression)
    : expression(expression) {}

  /// Destructor to clean up the inner expression.
  ~Grouping()
  {
    delete expression;
  }

  R accept(Expr<R>::Visitor& visitor) const override
  {
    return visitor.visitGroupingExpr(*this);
  }

  const Expr<R>* expression; ///< The expression inside the grouping.
};

template <class R>
class Expr<R>::Literal : public Expr<R>
{
public:
  Literal(const Token::LiteralValue& value)
    : value(value) {}

  R accept(Expr<R>::Visitor& visitor) const override
  {
    return visitor.visitLiteralExpr(*this);
  }

  const Token::LiteralValue value; ///< The value of the literal expression.
};

template <class R>
class Expr<R>::Unary : public Expr<R>
{
public:
  Unary(const Token& oper, const Expr<R>* right)
    : oper(oper), right(right) {}

  /// Destructor to clean up the operand.
  ~Unary()
  {
    delete right;
  }

  R accept(Expr<R>::Visitor& visitor) const override
  {
    return visitor.visitUnaryExpr(*this);
  }

  const Token oper; ///< The operator token.
  const Expr<R>* right; ///< The operand of the unary expression.
};
