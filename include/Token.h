#pragma once

#include <iostream>

/**
 * @enum TokenType
 * @brief Enumerates the types of tokens that can be encountered.
 */
enum TokenType 
{
  // Single-character tokens.
  LEFT_PAREN, /**< Token for '(' */
  RIGHT_PAREN, /**< Token for ')' */
  LEFT_BRACE, /**< Token for '{' */
  RIGHT_BRACE, /**< Token for '}' */
  COMMA, /**< Token for ',' */
  DOT, /**< Token for '.' */
  MINUS, /**< Token for '-' */
  PLUS, /**< Token for '+' */
  SEMICOLON, /**< Token for ';' */
  SLASH, /**< Token for '/' */
  STAR, /**< Token for '*' */

  // One or two character tokens.
  BANG, /**< Token for '!' */
  BANG_EQUAL, /**< Token for '!=' */
  EQUAL, /**< Token for '=' */
  EQUAL_EQUAL, /**< Token for '==' */
  GREATER, /**< Token for '>' */
  GREATER_EQUAL, /**< Token for '>=' */
  LESS, /**< Token for '<' */
  LESS_EQUAL, /**< Token for '<=' */

  // Literals.
  IDENTIFIER, /**< Token for identifiers */
  STRING, /**< Token for string literals */
  NUMBER, /**< Token for numeric literals */

  // Keywords.
  AND, /**< Token for keyword 'and' */
  CLASS, /**< Token for keyword 'class' */
  ELSE, /**< Token for keyword 'else' */
  FALSE, /**< Token for keyword 'false' */
  FUN, /**< Token for keyword 'fun' */
  FOR, /**< Token for keyword 'for' */
  IF, /**< Token for keyword 'if' */
  NIL, /**< Token for keyword 'nil' */
  OR, /**< Token for keyword 'or' */
  PRINT, /**< Token for keyword 'print' */
  RETURN, /**< Token for keyword 'return' */
  SUPER, /**< Token for keyword 'super' */
  THIS, /**< Token for keyword 'this' */
  TRUE, /**< Token for keyword 'true' */
  VAR, /**< Token for keyword 'var' */
  WHILE, /**< Token for keyword 'while' */

  END /**< Token to signify the end of the file */
};

/**
 * @class BaseToken
 * @brief The base class for all token types.
 */
class BaseToken
{
public:
  /**
   * @brief Virtual destructor to allow derived classes to clean up.
   */
  virtual ~BaseToken() = default;
};

// Forward declaration of the Token class template.
template <typename Literal_Type>
class Token;

// Forward declaration of the stream insertion operator.
template <typename Literal_Type>
std::ostream& operator<<(std::ostream& os, const Token<Literal_Type>& token);

/**
 * @class Token
 * @brief Template class for a token.
 *
 * @tparam Literal_Type The type of the literal value of the token.
 */
template <typename Literal_Type>
class Token : public BaseToken
{
public:
  const TokenType type; /**< Type of the token. */
  const std::string lexeme; /**< Lexeme of the token. */
  const Literal_Type literal; /**< Literal value of the token, if applicable. */
  const int line; /**< Line number in the source where the token was found. */

  /**
   * @brief Constructor for the Token class.
   * @param type The type of the token.
   * @param lexeme The lexeme of the token.
   * @param literal The literal value of the token.
   * @param line The line number where the token was found.
   */
  Token(const TokenType& type, const std::string& lexeme, 
        const Literal_Type& literal, const int& line):
    type(type), lexeme(lexeme), literal(literal), line(line) {}
  
  /**
   * @brief Stream insertion operator for the Token class.
   * @param os The output stream.
   * @param token The token to be inserted into the stream.
   * @return The output stream.
   */
  friend std::ostream& operator<< <>(std::ostream& os, const Token<Literal_Type>& token);
};

/**
 * @brief Stream insertion operator for the Token class.
 * @param os The output stream.
 * @param token The token to be inserted into the stream.
 * @return The output stream.
 */
template <typename Literal_Type>
std::ostream& operator<<(std::ostream& os, const Token<Literal_Type>& token)
{
  os << token.type << " " << token.lexeme << " " << token.literal << " " << token.line;
  return os;
}
