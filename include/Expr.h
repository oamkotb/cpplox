#pragma once

#include "Token.h"

/**
 * @brief Represents an abstract expression in the language.
 * 
 * @tparam R The return type of the expression when evaluated or processed.
 */
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
  class Conditional;

  /**
   * @brief The Visitor interface for the expression.
   */
  struct Visitor
  {
    /**
     * @brief Visit a binary expression.
     * 
     * @param expr The binary expression to visit.
     * @return The result of visiting the expression.
     */
    virtual R visitBinaryExpr(const Expr<R>::Binary& expr) = 0;

    /**
     * @brief Visit a grouping expression.
     * 
     * @param expr The grouping expression to visit.
     * @return The result of visiting the expression.
     */
    virtual R visitGroupingExpr(const Expr<R>::Grouping& expr) = 0;

    /**
     * @brief Visit a literal expression.
     * 
     * @param expr The literal expression to visit.
     * @return The result of visiting the expression.
     */
    virtual R visitLiteralExpr(const Expr<R>::Literal& expr) = 0;

    /**
     * @brief Visit a unary expression.
     * 
     * @param expr The unary expression to visit.
     * @return The result of visiting the expression.
     */
    virtual R visitUnaryExpr(const Expr<R>::Unary& expr) = 0;

    /**
     * @brief Visit a conditional expression.
     * 
     * @param expr The conditional expression to visit.
     * @return The result of visiting the expression.
     */
    virtual R visitConditionalExpr(const Expr<R>::Conditional& expr) = 0;
  };

  /**
   * @brief Accept a visitor to process this expression.
   * 
   * @param visitor The visitor to accept.
   * @return The result of the visitor processing this expression.
   */
  virtual R accept(Visitor& visitor) const = 0;
};

/**
 * @brief Represents a binary expression.
 * 
 * @tparam R The return type of the expression when evaluated or processed.
 */
template <class R>
class Expr<R>::Binary : public Expr<R>
{
public:
  /**
   * @brief Construct a new Binary expression.
   * 
   * @param left The left operand of the binary expression.
   * @param oper The operator token of the binary expression.
   * @param right The right operand of the binary expression.
   */
  Binary(const Expr<R>* left, const Token& oper, const Expr<R>* right)
      : left(left), oper(oper), right(right) {}

  /// Destructor to clean up the operands.
  ~Binary()
  {
    delete left;
    delete right;
  }

  /**
   * @brief Accept a visitor to process this binary expression.
   * 
   * @param visitor The visitor to accept.
   * @return The result of the visitor processing this expression.
   */
  R accept(Expr<R>::Visitor& visitor) const override
  {
    return visitor.visitBinaryExpr(*this);
  }

  const Expr<R>* left; ///< The left operand.
  const Token oper; ///< The operator token.
  const Expr<R>* right; ///< The right operand.
};

/**
 * @brief Represents a grouping expression.
 * 
 * @tparam R The return type of the expression when evaluated or processed.
 */
template <class R>
class Expr<R>::Grouping : public Expr<R>
{
public:
  /**
   * @brief Construct a new Grouping expression.
   * 
   * @param expression The expression inside the grouping.
   */
  Grouping(const Expr<R>* expression)
    : expression(expression) {}

  /// Destructor to clean up the inner expression.
  ~Grouping()
  {
    delete expression;
  }

  /**
   * @brief Accept a visitor to process this grouping expression.
   * 
   * @param visitor The visitor to accept.
   * @return The result of the visitor processing this expression.
   */
  R accept(Expr<R>::Visitor& visitor) const override
  {
    return visitor.visitGroupingExpr(*this);
  }

  const Expr<R>* expression; ///< The expression inside the grouping.
};

/**
 * @brief Represents a literal expression.
 * 
 * @tparam R The return type of the expression when evaluated or processed.
 */
template <class R>
class Expr<R>::Literal : public Expr<R>
{
public:
  /**
   * @brief Construct a new Literal expression.
   * 
   * @param value The value of the literal expression.
   */
  Literal(const LiteralValue& value)
    : value(value) {}

  /**
   * @brief Accept a visitor to process this literal expression.
   * 
   * @param visitor The visitor to accept.
   * @return The result of the visitor processing this expression.
   */
  R accept(Expr<R>::Visitor& visitor) const override
  {
    return visitor.visitLiteralExpr(*this);
  }

  const LiteralValue value; ///< The value of the literal expression.
};

/**
 * @brief Represents a unary expression.
 * 
 * @tparam R The return type of the expression when evaluated or processed.
 */
template <class R>
class Expr<R>::Unary : public Expr<R>
{
public:
  /**
   * @brief Construct a new Unary expression.
   * 
   * @param oper The operator token of the unary expression.
   * @param right The operand of the unary expression.
   */
  Unary(const Token& oper, const Expr<R>* right)
    : oper(oper), right(right) {}

  /// Destructor to clean up the operand.
  ~Unary()
  {
    delete right;
  }

  /**
   * @brief Accept a visitor to process this unary expression.
   * 
   * @param visitor The visitor to accept.
   * @return The result of the visitor processing this expression.
   */
  R accept(Expr<R>::Visitor& visitor) const override
  {
    return visitor.visitUnaryExpr(*this);
  }

  const Token oper; ///< The operator token.
  const Expr<R>* right; ///< The operand of the unary expression.
};

/**
 * @brief Represents a conditional (ternary) expression.
 * 
 * @tparam R The return type of the expression when evaluated or processed.
 */
template <class R>
class Expr<R>::Conditional : public Expr<R>
{
public:
  /**
   * @brief Construct a new Conditional expression.
   * 
   * @param condition The condition expression.
   * @param then_branch The expression to evaluate if the condition is true.
   * @param else_branch The expression to evaluate if the condition is false.
   */
  Conditional(Expr<R>* condition, Expr<R>* then_branch, Expr<R>* else_branch)
    : condition(condition), then_branch(then_branch), else_branch(else_branch) {}

  /// Destructor to clean up the sub-expressions.
  ~Conditional()
  {
    delete condition;
    delete then_branch;
    delete else_branch;
  }

  /**
   * @brief Accept a visitor to process this conditional expression.
   * 
   * @param visitor The visitor to accept.
   * @return The result of the visitor processing this expression.
   */
  R accept(Expr<R>::Visitor& visitor) const override
  {
    return visitor.visitConditionalExpr(*this);
  }

  const Expr<R>* condition; ///< The condition expression.
  const Expr<R>* then_branch; ///< The expression to evaluate if the condition is true.
  const Expr<R>* else_branch; ///< The expression to evaluate if the condition is false.
};