#pragma once

#include "Token.h"

template <class R>
class Stmt
{
public:
  virtual ~Stmt() = default;

  class Block;
  class Expression;
  class Print;
  class Var;

  struct Visitor
  {
    virtual R visitBlockStmt(const Stmt<R>::Block& stmt) = 0;
    virtual R visitExpressionStmt(const Stmt<R>::Expression& stmt) = 0;
    virtual R visitPrintStmt(const Stmt<R>::Print& stmt) = 0;
    virtual R visitVarStmt(const Stmt<R>::Var& stmt) = 0;
  };

  virtual R accept(Visitor& visitor) const = 0;
};

template <class R>
class Stmt<R>::Block : public Stmt<R>
{
public:
  Block(const std::vector<std::shared_ptr<const Stmt<R>>>& statements):
    statements(statements) {}

  R accept(Stmt<R>::Visitor& visitor) const override
  {
    return visitor.visitBlockStmt(*this);
  }

  const std::vector<std::shared_ptr<const Stmt<R>>> statements;
};

template <class R>
class Stmt<R>::Expression : public Stmt<R>
{
public:
  Expression(const std::shared_ptr<const Expr<R>>& expression):
    expression(expression) {}

  R accept(Stmt<R>::Visitor& visitor) const override
  {
    return visitor.visitExpressionStmt(*this);
  }

  const std::shared_ptr<const Expr<R>> expression;
};

template <class R>
class Stmt<R>::Print : public Stmt<R>
{
public:
  Print(const std::shared_ptr<const Expr<R>>& expression):
    expression(expression) {}

  R accept(Stmt<R>::Visitor& visitor) const override
  {
    return visitor.visitPrintStmt(*this);
  }

  const std::shared_ptr<const Expr<R>> expression;
};

template <class R>
class Stmt<R>::Var : public Stmt<R>
{
public:
  Var(const Token& name, const std::shared_ptr<const Expr<R>>& initializer):
    name(name), initializer(initializer) {}

  R accept(Stmt<R>::Visitor& visitor) const override
  {
    return visitor.visitVarStmt(*this);
  }

  const Token name;
  const std::shared_ptr<const Expr<R>> initializer;
};
