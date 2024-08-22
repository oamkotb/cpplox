#pragma once

#include <stdexcept>
#include <vector>
#include <memory>
#include "Expr.h"
#include "Stmt.h"
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
    : _tokens(std::move(tokens)) {}
  
  /**
   * @brief Parses the tokens into a list of statements.
   * @return A vector of smart pointers to the parsed statements.
   */
  std::vector<std::shared_ptr<Stmt<R>>> parse();

private:
  std::vector<Token> _tokens; ///< The list of tokens to parse.
  unsigned int _current = 0; ///< The current position in the token list.

  std::vector<std::shared_ptr<Expr<R>>> _allocated_exprs; ///< List of allocated expressions for cleanup.
  std::vector<std::shared_ptr<Stmt<R>>> _allocated_stmts; ///< List of allocated statements for cleanup.

  /**
   * @brief Parses an expression.
   * @return A smart pointer to the parsed expression.
   */
  std::shared_ptr<Expr<R>> expression();

  /**
   * @brief Parses a block of statements enclosed in braces.
   * @tparam R The type of the expression that will be parsed.
   * @return A vector of smart pointers to the parsed block statements.
   */
  std::vector<std::shared_ptr<const Stmt<R>>> block();

  /**
   * @brief Parses a declaration statement.
   * @return A smart pointer to the parsed declaration statement.
   */
  std::shared_ptr<Stmt<R>> declaration();

  /**
   * @brief Parses a variable declaration statement.
   * @return A smart pointer to the parsed variable declaration statement.
   */
  std::shared_ptr<Stmt<R>> varDeclaration();

  /**
   * @brief Parses a statement.
   * @return A smart pointer to the parsed statement.
   */
  std::shared_ptr<Stmt<R>> statement();

  /**
   * CHANGE THIS COMMENT
   */
  std::shared_ptr<Stmt<R>> forStatement();

  /**
   * @brief Parses an if statement.
   * @tparam R The type of the expression that will be parsed.
   * @return A smart pointer to the parsed if statement.
   */
  std::shared_ptr<Stmt<R>> ifStatement();

  /**
   * @brief Parses a print statement.
   * @return A smart pointer to the parsed print statement.
   */
  std::shared_ptr<Stmt<R>> printStatement();
  
  /**
   * @brief Parses a while statement.
   * @tparam R The type of the expression that will be parsed.
   * @return A smart pointer to the parsed while statement.
   */
  std::shared_ptr<Stmt<R>> whileStatement();

  /**
   * @brief Parses an expression statement.
   * @return A smart pointer to the parsed expression statement.
   */
  std::shared_ptr<Stmt<R>> expressionStatement();

  /**
   * @brief Parses an assignment expression.
   * @tparam R The type of the expression that will be parsed.
   * @return A smart pointer to the parsed assignment expression.
   */
  std::shared_ptr<Expr<R>> assignment();
  
  /**
   * @brief Parses a ternary expression.
   * @return A smart pointer to the parsed ternary expression.
   */
  std::shared_ptr<Expr<R>> ternary();

  /**
   * @brief Parses a logical OR expression. 
   * @tparam R The type of the expression that will be parsed.
   * @return A smart pointer to the parsed logical OR expression.
   */
  std::shared_ptr<Expr<R>> logicalOr();

  /**
   * @brief Parses a logical AND expression.
   * @tparam R The type of the expression that will be parsed.
   * @return A smart pointer to the parsed logical AND expression.
   */
  std::shared_ptr<Expr<R>> logicalAnd();

  /**
   * @brief Parses a comma expression.
   * @return A smart pointer to the parsed comma expression.
   */
  std::shared_ptr<Expr<R>> comma();

  /**
   * @brief Parses an equality expression.
   * @return A smart pointer to the parsed equality expression.
   */
  std::shared_ptr<Expr<R>> equality();

  /**
   * @brief Parses a comparison expression.
   * @return A smart pointer to the parsed comparison expression.
   */
  std::shared_ptr<Expr<R>> comparison();

  /**
   * @brief Parses a term expression.
   * @return A smart pointer to the parsed term expression.
   */
  std::shared_ptr<Expr<R>> term();

  /**
   * @brief Parses a factor expression.
   * @return A smart pointer to the parsed factor expression.
   */
  std::shared_ptr<Expr<R>> factor();

  /**
   * @brief Parses a unary expression.
   * @return A smart pointer to the parsed unary expression.
   */
  std::shared_ptr<Expr<R>> unary();

  /**
   * @brief Parses a primary expression.
   * @return A smart pointer to the parsed primary expression.
   */
  std::shared_ptr<Expr<R>> primary();
  
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
  ParseError error(const Token& token, const std::string& message);

  /**
   * @brief Synchronizes the parser state after an error.
   */
  void synchronize();
};

#include "Parser_impl.h"
