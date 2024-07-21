#pragma once

#include <stdexcept>
#include <vector>
#include "Expr.h"
#include "Token.h"

/**
 * @class ParseError
 * @brief Custom exception class for parse errors.
 */
class ParseError : public std::runtime_error
{
public:
  /**
   * @brief Constructs a new ParseError with the given message.
   * @param msg The error message.
   */
  ParseError(const std::string& msg) 
    : std::runtime_error(msg) {}
};

/**
 * @class Parser
 * @brief Parses tokens into an abstract syntax tree (AST) of expressions.
 * @tparam R The type of the expression that will be parsed.
 */
template <class R>
class Parser
{
public:
  /**
   * @brief Constructs a new Parser with the given tokens.
   * @param tokens The list of tokens to parse.
   */
  Parser(std::vector<Token> tokens)
    : _tokens(tokens) {}

  /**
   * @brief Parses the tokens into an expression.
   * @return A pointer to the parsed expression or nullptr if an error occurred.
   */
  Expr<R>* parse();

private:
  std::vector<Token> _tokens; ///< The list of tokens to parse.
  unsigned int _current = 0; ///< The current position in the token list.

  std::vector<Expr<R>*> _allocated_exprs; ///< List of allocated expressions for cleanup.

  /**
   * @brief Parses an expression.
   * @return A pointer to the parsed expression.
   */
  Expr<R>* expression();

  /**
   * @brief Parses a conditional expression.
   * @return A pointer to the parsed conditional expression.
   */
  Expr<R>* conditional();

  /**
   * @brief Parses a comma expression.
   * @return A pointer to the parsed comma expression.
   */
  Expr<R>* comma();

  /**
   * @brief Parses an equality expression.
   * @return A pointer to the parsed equality expression.
   */
  Expr<R>* equality();

  /**
   * @brief Parses a comparison expression.
   * @return A pointer to the parsed comparison expression.
   */
  Expr<R>* comparison();

  /**
   * @brief Parses a term expression.
   * @return A pointer to the parsed term expression.
   */
  Expr<R>* term();

  /**
   * @brief Parses a factor expression.
   * @return A pointer to the parsed factor expression.
   */
  Expr<R>* factor();

  /**
   * @brief Parses a unary expression.
   * @return A pointer to the parsed unary expression.
   */
  Expr<R>* unary();

  /**
   * @brief Parses a primary expression.
   * @return A pointer to the parsed primary expression.
   */
  Expr<R>* primary();
  
  /**
   * @brief Checks if the current token matches the given type and advances if it does.
   * @param type The token type to match.
   * @return True if the token matches, otherwise false.
   */
  bool match(const TokenType& type);

  /**
   * @brief Checks if the current token matches any of the given types and advances if it does.
   * @param types The list of token types to match.
   * @return True if the token matches any of the types, otherwise false.
   */
  bool match(const std::vector<TokenType>& types);

  /**
   * @brief Checks if the current token matches the given type.
   * @param type The token type to check.
   * @return True if the token matches, otherwise false.
   */
  bool check(const TokenType& type);

  /**
   * @brief Checks if the parser has reached the end of the token list.
   * @return True if at the end, otherwise false.
   */
  bool isAtEnd();

  /**
   * @brief Consumes the current token if it matches the given type, otherwise throws an error.
   * @param type The token type to consume.
   * @param message The error message if the token doesn't match.
   * @return The consumed token.
   */
  Token consume(const TokenType& type, const std::string& message);

  /**
   * @brief Peeks at the current token.
   * @return The current token.
   */
  Token peek();

  /**
   * @brief Advances to the next token and returns the previous token.
   * @return The previous token.
   */
  Token advance();

  /**
   * @brief Returns the previous token.
   * @return The previous token.
   */
  Token previous();

  /**
   * @brief Creates a parse error, logs it, and returns a ParseError object.
   * @param token The token where the error occurred.
   * @param message The error message.
   * @return A ParseError object.
   */
  ParseError error (const Token& token, const std::string& message);

  /**
   * @brief Synchronizes the parser state after an error.
   */
  void synchronize();

  /**
   * @brief Registers an allocated expression for cleanup.
   * @param expr The expression to register.
   */
  void registerExpr(Expr<R>* expr);

  /**
   * @brief Cleans up all allocated expressions.
   */
  void cleanUpExprs();
};

#include "Parser_impl.h"