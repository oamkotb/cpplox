#pragma once

#include "Token.h"

template <class R>
class Expr
{
public:
  virtual ~Expr() = default;

  class Assign;
  class Binary;
  class Call;
  class Grouping;
  class Literal;
  class Logical;
  class Unary;
  class Ternary;
  class Variable;

  struct Visitor
  {
    virtual R visitAssignExpr(const Expr<R>::Assign& expr) = 0;
    virtual R visitBinaryExpr(const Expr<R>::Binary& expr) = 0;
    virtual R visitCallExpr(const Expr<R>::Call& expr) = 0;
    virtual R visitGroupingExpr(const Expr<R>::Grouping& expr) = 0;
    virtual R visitLiteralExpr(const Expr<R>::Literal& expr) = 0;
    virtual R visitLogicalExpr(const Expr<R>::Logical& expr) = 0;
    virtual R visitUnaryExpr(const Expr<R>::Unary& expr) = 0;
    virtual R visitTernaryExpr(const Expr<R>::Ternary& expr) = 0;
    virtual R visitVariableExpr(const Expr<R>::Variable& expr) = 0;
  };

  virtual R accept(Visitor& visitor) const = 0;
};

template <class R>
class Expr<R>::Assign : public Expr<R>
{
public:
  Assign(const Token& name, const std::shared_ptr<const Expr<R>>& value):
    name(name), value(value) {}

  R accept(Expr<R>::Visitor& visitor) const override
  {
    return visitor.visitAssignExpr(*this);
  }

  const Token name;
  const std::shared_ptr<const Expr<R>> value;
};

template <class R>
class Expr<R>::Binary : public Expr<R>
{
public:
  Binary(const std::shared_ptr<const Expr<R>>& left, const Token& oper, const std::shared_ptr<const Expr<R>>& right):
    left(left), oper(oper), right(right) {}

  R accept(Expr<R>::Visitor& visitor) const override
  {
    return visitor.visitBinaryExpr(*this);
  }

  const std::shared_ptr<const Expr<R>> left;
  const Token oper;
  const std::shared_ptr<const Expr<R>> right;
};

template <class R>
class Expr<R>::Call : public Expr<R>
{
public:
  Call(const std::shared_ptr<const Expr<R>>& callee, const Token& paren, std::vector<std::shared_ptr<const Expr<R>>>& arguments):
    callee(callee), paren(paren), arguments(arguments) {}

  R accept(Expr<R>::Visitor& visitor) const override
  {
    return visitor.visitCallExpr(*this);
  }

  const std::shared_ptr<const Expr<R>> callee;
  const Token paren;
  std::vector<std::shared_ptr<const Expr<R>>> arguments;
};

template <class R>
class Expr<R>::Grouping : public Expr<R>
{
public:
  Grouping(const std::shared_ptr<const Expr<R>>& expression):
    expression(expression) {}

  R accept(Expr<R>::Visitor& visitor) const override
  {
    return visitor.visitGroupingExpr(*this);
  }

  const std::shared_ptr<const Expr<R>> expression;
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
class Expr<R>::Logical : public Expr<R>
{
public:
  Logical(const std::shared_ptr<const Expr<R>>& left, const Token& oper, const std::shared_ptr<const Expr<R>>& right):
    left(left), oper(oper), right(right) {}

  R accept(Expr<R>::Visitor& visitor) const override
  {
    return visitor.visitLogicalExpr(*this);
  }

  const std::shared_ptr<const Expr<R>> left;
  const Token oper;
  const std::shared_ptr<const Expr<R>> right;
};

template <class R>
class Expr<R>::Unary : public Expr<R>
{
public:
  Unary(const Token& oper, const std::shared_ptr<const Expr<R>>& right):
    oper(oper), right(right) {}

  R accept(Expr<R>::Visitor& visitor) const override
  {
    return visitor.visitUnaryExpr(*this);
  }

  const Token oper;
  const std::shared_ptr<const Expr<R>> right;
};

template <class R>
class Expr<R>::Ternary : public Expr<R>
{
public:
  Ternary(const std::shared_ptr<const Expr<R>>& condition, const std::shared_ptr<const Expr<R>>& then_branch, const std::shared_ptr<const Expr<R>>& else_branch):
    condition(condition), then_branch(then_branch), else_branch(else_branch) {}

  R accept(Expr<R>::Visitor& visitor) const override
  {
    return visitor.visitTernaryExpr(*this);
  }

  const std::shared_ptr<const Expr<R>> condition;
  const std::shared_ptr<const Expr<R>> then_branch;
  const std::shared_ptr<const Expr<R>> else_branch;
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
