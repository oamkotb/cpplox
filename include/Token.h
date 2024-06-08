#pragma once

#include <iostream>

enum TokenType 
{
  // Single-character tokens
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

  // One or two character tokens
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,

  // Literals
  IDENTIFIER, STRING, NUMBER,

  // Keywords
  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
  PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

  END
};

class BaseToken
{
public:
  virtual ~BaseToken() = default;
};

template <typename Literal_Type>
class Token
{
public:
  const TokenType type;
  const std::string lexeme;
  const Literal_Type literal;
  const int line;

  Token(const TokenType& type, const std::string& lexeme, 
        const Literal_Type& literal, const int& line):
    type(type), lexeme(lexeme), literal(literal), line(line) {}
  
  // friend std::ostream& operator<<(std::ostream& os, const Token<Literal_Type>& token);
};

// template <typename Literal_Type>
// std::ostream& operator<<(std::ostream& os, const Token<Literal_Type>& token)
// {
//   os << token.type << " " << token.lexeme << " " << token.literal << std::endl;
//   return os;
// }