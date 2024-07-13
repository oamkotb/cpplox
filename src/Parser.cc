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
    return nullptr;
  }
}

Expr<std::string>* Parser::expression()
{
  return equality();
}

Expr<std::string>* Parser::equality()
{
  Expr<std::string>* expr = comparison();

  while (match({ BANG_EQUAL, EQUAL_EQUAL }))
  {
    Token oper = previous();
    Expr<std::string>* right = comparison();
    expr = new Expr<std::string>::Binary(expr, oper, right);
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
  }

  return expr;
}

Expr<std::string>* Parser::unary()
{
  if (match({ BANG, MINUS }))
  {
    Token oper = previous();
    Expr<std::string>* right = unary();
    return new Expr<std::string>::Unary(oper, right);
  }

  return primary();
}

Expr<std::string>* Parser::primary()
{
  if (match(FALSE)) return new Expr<std::string>::Literal(0.0);
  if (match(TRUE)) return new Expr<std::string>::Literal(1.0);
  if (match(NIL)) return new Expr<std::string>::Literal(std::monostate()); // NULL

  if (match({ NUMBER, STRING }))
    return new Expr<std::string>::Literal(previous().literal);
  
  if (match(LEFT_PAREN))
  {
    Expr<std::string>* expr = expression();
    consume(RIGHT_PAREN, "Expect ')' after expression.");
    return new Expr<std::string>::Grouping(expr);
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
  return _tokens[current];
}

Token Parser::advance()
{
  if (!isAtEnd()) ++current;
  return previous();
}

Token Parser::previous()
{
  return _tokens[current - 1];
}


ParseError Parser::error (const Token& token, const std::string& message)
{
  Lox::error(token, message);
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