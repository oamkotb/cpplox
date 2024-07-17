#pragma once

#include <stdexcept>
#include <vector>

#include "Expr.h"
#include "Token.h"

class ParseError : public std::runtime_error
{
public:
  ParseError(const std::string& msg) 
    : std::runtime_error(msg) {}
};

class Parser
{
public:
  Parser(std::vector<Token> tokens)
    : _tokens(tokens) {}

  Expr<std::string>* parse();

private:
  std::vector<Token> _tokens;
  unsigned int _current = 0;

  std::vector<Expr<std::string>*> _allocated_exprs;

  Expr<std::string>* expression();
  Expr<std::string>* conditional();
  Expr<std::string>* comma();
  Expr<std::string>* equality();
  Expr<std::string>* comparison();
  Expr<std::string>* term();
  Expr<std::string>* factor();
  Expr<std::string>* unary();
  Expr<std::string>* primary();
  
  bool match(const TokenType& type);
  bool match(const std::vector<TokenType>& types);
  bool check(const TokenType& type);
  bool isAtEnd();
  
  Token consume(const TokenType& type, const std::string& message);
  Token peek();
  Token advance();
  Token previous();

  ParseError error (const Token& token, const std::string& message);
  void synchronize();

  void registerExpr(Expr<std::string>* expr);
  void cleanUpExprs();
};