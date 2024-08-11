#pragma once

#include "Token.h"
#include <memory>

template <class R>
class Expr
{
public:
  virtual ~Expr() = default;

  class Binary;
  class Grouping;
  class Literal;
  class Unary;
  class Conditional;
  class Variable;

  struct Visitor
  {
    virtual R visitBinaryExpr(const typename Expr<R>::Binary& expr) = 0;
    virtual R visitGroupingExpr(const typename Expr<R>::Grouping& expr) = 0;
    virtual R visitLiteralExpr(const typename Expr<R>::Literal& expr) = 0;
    virtual R visitUnaryExpr(const typename Expr<R>::Unary& expr) = 0;
    virtual R visitConditionalExpr(const typename Expr<R>::Conditional& expr) = 0;
    virtual R visitVariableExpr(const typename Expr<R>::Variable& expr) = 0;
  };

  virtual R accept(Visitor& visitor) const = 0;
};

template <class R>
class Expr<R>::Binary : public Expr<R>
{
public:
  Binary(std::shared_ptr<const Expr<R>> left, const Token& oper, std::shared_ptr<const Expr<R>> right):
    left(left), oper(oper), right(right) {}

  R accept(Expr<R>::Visitor& visitor) const override
  {
    return visitor.visitBinaryExpr(*this);
  }

  std::shared_ptr<const Expr<R>> left;
  const Token oper;
  std::shared_ptr<const Expr<R>> right;
};

template <class R>
class Expr<R>::Grouping : public Expr<R>
{
public:
  Grouping(std::shared_ptr<const Expr<R>> expression):
    expression(expression) {}

  R accept(Expr<R>::Visitor& visitor) const override
  {
    return visitor.visitGroupingExpr(*this);
  }

  std::shared_ptr<const Expr<R>> expression;
};

template <class R>
class Expr<R>::Literal : public Expr<R>
{
public:
  Literal(const LiteralValue& value):
    value(value) {}

  R accept(Expr<R>::Visitor& visitor) const override
  {
    return visitor.visitLiteralExpr(*this);
  }

  const LiteralValue value;
};

template <class R>
class Expr<R>::Unary : public Expr<R>
{
public:
  Unary(const Token& oper, std::shared_ptr<const Expr<R>> right):
    oper(oper), right(right) {}

  R accept(Expr<R>::Visitor& visitor) const override
  {
    return visitor.visitUnaryExpr(*this);
  }

  const Token oper;
  std::shared_ptr<const Expr<R>> right;
};

template <class R>
class Expr<R>::Conditional : public Expr<R>
{
public:
  Conditional(std::shared_ptr<const Expr<R>> condition, std::shared_ptr<const Expr<R>> then_branch, std::shared_ptr<const Expr<R>> else_branch):
    condition(condition), then_branch(then_branch), else_branch(else_branch) {}

  R accept(Expr<R>::Visitor& visitor) const override
  {
    return visitor.visitConditionalExpr(*this);
  }

  std::shared_ptr<const Expr<R>> condition;
  std::shared_ptr<const Expr<R>> then_branch;
  std::shared_ptr<const Expr<R>> else_branch;
};

template <class R>
class Expr<R>::Variable : public Expr<R>
{
public:
  Variable(const Token& name):
    name(name) {}

  R accept(Expr<R>::Visitor& visitor) const override
  {
    return visitor.visitVariableExpr(*this);
  }

  const Token name;
};
