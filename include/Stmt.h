#pragma once

#include "Token.h"

template <class R>
class Stmt
{
public:
  virtual ~Stmt() = default;

  class Block;
  class Expression;
  class If;
  class Function;
  class Print;
  class Var;
  class While;
  class Jump;

  struct Visitor
  {
    virtual R visitBlockStmt(const Stmt<R>::Block& stmt) = 0;
    virtual R visitExpressionStmt(const Stmt<R>::Expression& stmt) = 0;
    virtual R visitIfStmt(const Stmt<R>::If& stmt) = 0;
    virtual R visitFunctionStmt(const Stmt<R>::Function& stmt) = 0;
    virtual R visitPrintStmt(const Stmt<R>::Print& stmt) = 0;
    virtual R visitVarStmt(const Stmt<R>::Var& stmt) = 0;
    virtual R visitWhileStmt(const Stmt<R>::While& stmt) = 0;
    virtual R visitJumpStmt(const Stmt<R>::Jump& stmt) = 0;
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
class Stmt<R>::If : public Stmt<R>
{
public:
  If(const std::shared_ptr<const Expr<R>>& condition, const std::shared_ptr<const Stmt<R>>& then_branch, const std::shared_ptr<const Stmt<R>>& else_branch):
    condition(condition), then_branch(then_branch), else_branch(else_branch) {}

  R accept(Stmt<R>::Visitor& visitor) const override
  {
    return visitor.visitIfStmt(*this);
  }

  const std::shared_ptr<const Expr<R>> condition;
  const std::shared_ptr<const Stmt<R>> then_branch;
  const std::shared_ptr<const Stmt<R>> else_branch;
};

template <class R>
class Stmt<R>::Function : public Stmt<R>
{
public:
  Function(const Token& name, const std::vector<Token>& params, const std::vector<std::shared_ptr<const Stmt<R>>>& body):
    name(name), params(params), body(body) {}

  R accept(Stmt<R>::Visitor& visitor) const override
  {
    return visitor.visitFunctionStmt(*this);
  }

  const Token name;
  const std::vector<Token> params;
  const std::vector<std::shared_ptr<const Stmt<R>>> body;
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

template <class R>
class Stmt<R>::While : public Stmt<R>
{
public:
  While(const std::shared_ptr<const Expr<R>>& condition, const std::shared_ptr<const Stmt<R>>& body):
    condition(condition), body(body) {}

  R accept(Stmt<R>::Visitor& visitor) const override
  {
    return visitor.visitWhileStmt(*this);
  }

  const std::shared_ptr<const Expr<R>> condition;
  const std::shared_ptr<const Stmt<R>> body;
};

template <class R>
class Stmt<R>::Jump : public Stmt<R>
{
public:
  Jump(const Token& keyword):
    keyword(keyword) {}

  R accept(Stmt<R>::Visitor& visitor) const override
  {
    return visitor.visitJumpStmt(*this);
  }

  const Token keyword;
};
