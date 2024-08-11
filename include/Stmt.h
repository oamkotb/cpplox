#pragma once

#include "Token.h"
#include <memory>

template <class R>
class Stmt
{
public:
  virtual ~Stmt() = default;

  class Expression;
  class Print;
  class Var;

  struct Visitor
  {
    virtual R visitExpressionStmt(const typename Stmt<R>::Expression& stmt) = 0;
    virtual R visitPrintStmt(const typename Stmt<R>::Print& stmt) = 0;
    virtual R visitVarStmt(const typename Stmt<R>::Var& stmt) = 0;
  };

  virtual R accept(Visitor& visitor) const = 0;
};

template <class R>
class Stmt<R>::Expression : public Stmt<R>
{
public:
  Expression(std::shared_ptr<const Expr<R>> expression):
    expression(expression) {}

  R accept(Stmt<R>::Visitor& visitor) const override
  {
    return visitor.visitExpressionStmt(*this);
  }

  std::shared_ptr<const Expr<R>> expression;
};

template <class R>
class Stmt<R>::Print : public Stmt<R>
{
public:
  Print(std::shared_ptr<const Expr<R>> expression):
    expression(expression) {}

  R accept(Stmt<R>::Visitor& visitor) const override
  {
    return visitor.visitPrintStmt(*this);
  }

  std::shared_ptr<const Expr<R>> expression;
};

template <class R>
class Stmt<R>::Var : public Stmt<R>
{
public:
  Var(const Token& name, std::shared_ptr<const Expr<R>> initializer):
    name(name), initializer(initializer) {}

  R accept(Stmt<R>::Visitor& visitor) const override
  {
    return visitor.visitVarStmt(*this);
  }

  const Token name;
  std::shared_ptr<const Expr<R>> initializer;
};
