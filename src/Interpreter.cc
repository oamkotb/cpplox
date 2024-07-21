#include <Interpreter.h>

void Interpreter::interpret(const Expr<LiteralValue>* expr)
{
  try
  {
    LiteralValue value = evaluate(expr);
    std::cout << stringify(value) << std::endl;
  }
  catch(const RuntimeError& error)
  {
    Lox::runtimeError(error);
  }
  
}

LiteralValue Interpreter::visitBinaryExpr(const Expr<LiteralValue>::Binary& expr)
{
  LiteralValue left = evaluate(expr.left);
  LiteralValue right = evaluate(expr.right);

  switch (expr.oper.type)
  {
  case GREATER:
    checkNumberOperands(expr.oper, left, right);
    return std::get<double>(left) > std::get<double>(right);

  case GREATER_EQUAL:
    checkNumberOperands(expr.oper, left, right);
    return std::get<double>(left) >= std::get<double>(right);
  
  case LESS:
    checkNumberOperands(expr.oper, left, right);
    return std::get<double>(left) < std::get<double>(right);
  
  case LESS_EQUAL:
    checkNumberOperands(expr.oper, left, right);
    return std::get<double>(left) <= std::get<double>(right);
  
  case EQUAL_EQUAL:
    return isEqual(left, right);

  case BANG_EQUAL:
    return !isEqual(left, right);

  case MINUS:
    checkNumberOperands(expr.oper, left, right);
    return std::get<double>(left) - std::get<double>(right);
  
  case PLUS:
    if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right))
      return std::get<std::string>(left) + std::get<std::string>(right);
    if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right))
      return std::get<double>(left) + std::get<double>(right);
    
    throw RuntimeError(expr.oper, "Operands must be two numbers or two strings.");

  case STAR:
    checkNumberOperands(expr.oper, left, right);
    return std::get<double>(left) * std::get<double>(right);

  case SLASH:
    checkNumberOperands(expr.oper, left, right);
    return std::get<double>(left) / std::get<double>(right);

  default:
    return std::monostate();
  }
}

LiteralValue Interpreter::visitLiteralExpr(const Expr<LiteralValue>::Literal& expr)
{
  return expr.value;
}

LiteralValue Interpreter::visitGroupingExpr(const Expr<LiteralValue>::Grouping& expr)
{
  return evaluate(expr.expression);
}

LiteralValue Interpreter::visitUnaryExpr(const Expr<LiteralValue>::Unary& expr)
{
  LiteralValue right = evaluate(expr.right);

  switch (expr.oper.type)
  {
    case BANG:
      return !isTruthy(right);
    case MINUS:
      checkNumberOperand(expr.oper, right);
      return right;

    default:
      return std::monostate();
  }
}

LiteralValue Interpreter::visitConditionalExpr(const Expr<LiteralValue>::Conditional& expr)
{
  LiteralValue conditon = evaluate(expr.condition);

  if (isTruthy(conditon))
    return evaluate(expr.then_branch);
  
  return evaluate(expr.else_branch);
}

LiteralValue Interpreter::evaluate(const Expr<LiteralValue>* expr)
{
  return expr->accept(*this);
}

std::string Interpreter::stringify(const LiteralValue& value)
{  
  if (std::holds_alternative<double>(value))
  {
    std::string text = std::to_string(std::get<double>(value));
    if (text.size() > 2 && text.substr(text.size() - 2) == ".0")
      text = text.substr(0, text.size() - 2);
    
    return text;
  }
  
  if (std::holds_alternative<std::string>(value))
    return std::get<std::string>(value);

  if (std::holds_alternative<bool>(value))
  {
    if (std::get<bool>(value) == true)
      return "True";
    else
      return "False";
  }

  return "nil";
}

bool Interpreter::isTruthy(const LiteralValue& value)
{
  if (std::holds_alternative<std::monostate>(value)) return false;
  if (std::holds_alternative<bool>(value)) return std::get<bool>(value);

  return true;
}

bool Interpreter::isEqual(const LiteralValue& a, const LiteralValue& b)
{
  if (std::holds_alternative<std::monostate>(a) && std::holds_alternative<std::monostate>(b)) return true;
  if (a.index() == b.index()) return a == b;

  return false;
}

void Interpreter::checkNumberOperand(const Token& oper, const LiteralValue& operand)
{
  if (std::holds_alternative<double>(operand)) return;

  throw RuntimeError(oper, "Operands must be numbers.");
}

void Interpreter::checkNumberOperands(const Token& oper, const LiteralValue& left, const LiteralValue& right)
{
  if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) return;

  throw RuntimeError(oper, "Operands must be numbers.");
}