#include <iostream>
#include "AstPrinter.h"
#include "Expr.h"
#include "Token.h"

int main()
{
  Expr<std::string>::Binary* binaryExpr = new Expr<std::string>::Binary(
    new Expr<std::string>::Unary(
      Token(TokenType::MINUS, "-", 1),
      new Expr<std::string>::Literal(123.0)),
    Token(TokenType::STAR, "*", 1),
    new Expr<std::string>::Grouping(
      new Expr<std::string>::Literal(45.67))
  );

  AstPrinter printer;
  std::string result = printer.print(*binaryExpr);
  std::cout << result << std::endl;

  delete binaryExpr;
  return 0;
}