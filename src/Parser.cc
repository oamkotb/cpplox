#include "utils.h"
#include "Parser.h"

Expr<std::string>* Parser::parse()
{
  try
  {
    return expression();
  }
  catch (const ParseError& error)
  {
    cleanUpExprs();
    return nullptr;
  }
}

Expr<std::string>* Parser::expression()
{
  return conditional();
}

Expr<std::string>* Parser::conditional()
{
  Expr<std::string>* expr = comma();

  if (match({QUESTION_MARK}))
  {
    Expr<std::string>* then_branch = expression();
    consume(COLON, "Expect ':' after then branch of conditional expression.");
    Expr<std::string>* else_branch = conditional();
    expr = new Expr<std::string>::Conditional(expr, then_branch, else_branch);
    registerExpr(expr);
  }

  return expr;
}

Expr<std::string>* Parser::comma()
{
  Expr<std::string>* expr = equality();

  while (match({COMMA}))
  {
    Token oper = previous();
    Expr<std::string>* right = equality();
    expr = new Expr<std::string>::Binary(expr, oper, right);
    registerExpr(expr);
  }

  return expr;
}

Expr<std::string>* Parser::equality()
{
  Expr<std::string>* expr = comparison();

  while (match({ BANG_EQUAL, EQUAL_EQUAL }))
  {
    Token oper = previous();
    Expr<std::string>* right = comparison();
    expr = new Expr<std::string>::Binary(expr, oper, right);
    registerExpr(expr);
  }

  return expr;
}

Expr<std::string>* Parser::comparison()
{
  Expr<std::string>* expr = term();

  while (match({ GREATER, GREATER_EQUAL, LESS, LESS_EQUAL }))
  {
    Token oper = previous();
    Expr<std::string>* right = term();
    expr = new Expr<std::string>::Binary(expr, oper, right);
    registerExpr(expr);
  }

  return expr;
}

Expr<std::string>* Parser::term()
{
  Expr<std::string>* expr = factor();

  while (match({ MINUS, PLUS }))
  {
    Token oper = previous();
    Expr<std::string>* right = factor();
    expr = new Expr<std::string>::Binary(expr, oper, right);
    registerExpr(expr);
  }

  return expr;
}

Expr<std::string>* Parser::factor()
{
  Expr<std::string>* expr = unary();

  while (match({ SLASH, STAR }))
  {
    Token oper =  previous();
    Expr<std::string>* right = unary();
    expr = new Expr<std::string>::Binary(expr, oper, right);
    registerExpr(expr);
  }

  return expr;
}

Expr<std::string>* Parser::unary()
{
  if (match({ BANG, MINUS }))
  {
    Token oper = previous();
    Expr<std::string>* right = unary();
    Expr<std::string>* expr = new Expr<std::string>::Unary(oper, right);
    registerExpr(expr);
    return expr;
  }

  return primary();
}

Expr<std::string>* Parser::primary()
{
  if (match(FALSE)) return new Expr<std::string>::Literal(0.0);
  if (match(TRUE)) return new Expr<std::string>::Literal(1.0);
  if (match(NIL)) return new Expr<std::string>::Literal(std::monostate()); // NULL

  if (match({ NUMBER, STRING }))
  {
    Expr<std::string>* expr =  new Expr<std::string>::Literal(previous().literal);
    registerExpr(expr);
  }
  
  if (match(LEFT_PAREN))
  {
    Expr<std::string>* expr = expression();
    consume(RIGHT_PAREN, "Expect ')' after expression.");
    Expr<std::string>* grouping = new Expr<std::string>::Grouping(expr);
    registerExpr(expr);
    return grouping;
  }

  throw error(peek(), "Expect expression.");
}

bool Parser::match(const TokenType& type)
{
  if (check(type))
  {
    advance();
    return true;
  }
  return false;
}

bool Parser::match(const std::vector<TokenType>& types)
{
  for (TokenType type : types)
    if (check(type))
    {
      advance();
      return true;
    }
  return false;
}

bool Parser::check(const TokenType& type)
{
  if (isAtEnd()) return false;
  return peek().type == type;
}

bool Parser::isAtEnd()
{
  return peek().type == END;
}

Token Parser::consume(const TokenType& type, const std::string& message)
{
  if (check(type)) return advance();

  throw error(peek(), message);
}

Token Parser::peek()
{
  return _tokens[_current];
}

Token Parser::advance()
{
  if (!isAtEnd()) ++_current;
  return previous();
}

Token Parser::previous()
{
  return _tokens[_current - 1];
}


ParseError Parser::error (const Token& token, const std::string& message)
{
  Lox::error(token, message);
  cleanUpExprs();
  return ParseError(message);
}

void Parser::synchronize()
{
  advance();
 
  while (!isAtEnd())
  {
    if (previous().type == SEMICOLON) return;

    switch (peek().type)
    {
      case CLASS:
      case FUN:
      case VAR:
      case FOR:
      case IF:
      case WHILE:
      case PRINT:
      case RETURN:
        return;

      default:
        break;
    }
    advance();
  }
}

void Parser::registerExpr(Expr<std::string>* expr)
{
  _allocated_exprs.push_back(expr);
}

void Parser::cleanUpExprs()
{
  for (Expr<std::string>* expr : _allocated_exprs)
  {
    delete expr;
  }
  _allocated_exprs.clear();
}