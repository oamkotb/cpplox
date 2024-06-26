#include <iostream>
#include <vector>

#include "Expr.h"

class AstPrinter : public Expr<std::string>::Visitor
{
public:
  std::string print(Expr<std::string>& expr)
  {
    return expr.accept(*this);
  }

  std::string visitBinaryExpr(const Expr<std::string>::Binary& expr) override
  {
    return paranthesize(expr.oper.lexeme, { expr.left, expr.right });
  }

  std::string visitGroupingExpr(const Expr<std::string>::Grouping& expr) override
  {
    return paranthesize("group", { expr.expression });
  }

  std::string visitLiteralExpr(const Expr<std::string>::Literal& expr) override
  {
    if (std::holds_alternative<std::monostate>(expr.value)) return "nil";
    
  }

  std::string visitUnaryExpr(const Expr<std::string>::Unary& expr) override
  {
    
  }
private:
  std::string paranthesize(const std::string& name, const Expr<std::string>& expr);
  std::string paranthesize(const std::string& name, const std::vector<const Expr<std::string>*>& exprs);
};