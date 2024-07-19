#include "utils.h"
#include "Parser.h"

/**
 * @brief Parses the tokens into an expression.
 * @return A pointer to the parsed expression or nullptr if an error occurred.
 */
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

/**
 * @brief Parses an expression.
 * @return A pointer to the parsed expression.
 */
Expr<std::string>* Parser::expression()
{
  return conditional();
}

/**
 * @brief Parses a conditional expression.
 * @return A pointer to the parsed conditional expression.
 */
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

/**
 * @brief Parses a comma expression.
 * @return A pointer to the parsed comma expression.
 */
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

/**
 * @brief Parses an equality expression.
 * @return A pointer to the parsed equality expression.
 */
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

/**
 * @brief Parses a comparison expression.
 * @return A pointer to the parsed comparison expression.
 */
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

/**
 * @brief Parses a term expression.
 * @return A pointer to the parsed term expression.
 */
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

/**
 * @brief Parses a factor expression.
 * @return A pointer to the parsed factor expression.
 */
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

/**
 * @brief Parses a unary expression.
 * @return A pointer to the parsed unary expression.
 */
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

/**
 * @brief Parses a primary expression.
 * @return A pointer to the parsed primary expression.
 */
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

/**
 * @brief Checks if the current token matches the given type and advances if it does.
 * @param type The token type to match.
 * @return True if the token matches, otherwise false.
 */
bool Parser::match(const TokenType& type)
{
  if (check(type))
  {
    advance();
    return true;
  }
  return false;
}

/**
 * @brief Checks if the current token matches any of the given types and advances if it does.
 * @param types The list of token types to match.
 * @return True if the token matches any of the types, otherwise false.
 */
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

/**
 * @brief Checks if the current token matches the given type.
 * @param type The token type to check.
 * @return True if the token matches, otherwise false.
 */
bool Parser::check(const TokenType& type)
{
  if (isAtEnd()) return false;
  return peek().type == type;
}

/**
 * @brief Checks if the parser has reached the end of the token list.
 * @return True if at the end, otherwise false.
 */
bool Parser::isAtEnd()
{
  return peek().type == END;
}

/**
 * @brief Consumes the current token if it matches the given type, otherwise throws an error.
 * @param type The token type to consume.
 * @param message The error message if the token doesn't match.
 * @return The consumed token.
 */
Token Parser::consume(const TokenType& type, const std::string& message)
{
  if (check(type)) return advance();

  throw error(peek(), message);
}

/**
 * @brief Peeks at the current token.
 * @return The current token.
 */
Token Parser::peek()
{
  return _tokens[_current];
}

/**
 * @brief Advances to the next token and returns the previous token.
 * @return The previous token.
 */
Token Parser::advance()
{
  if (!isAtEnd()) ++_current;
  return previous();
}

/**
 * @brief Returns the previous token.
 * @return The previous token.
 */
Token Parser::previous()
{
  return _tokens[_current - 1];
}

/**
 * @brief Creates a parse error, logs it, and returns a ParseError object.
 * @param token The token where the error occurred.
 * @param message The error message.
 * @return A ParseError object.
 */
ParseError Parser::error (const Token& token, const std::string& message)
{
  Lox::error(token, message);
  cleanUpExprs();
  return ParseError(message);
}

/**
 * @brief Synchronizes the parser state after an error.
 */
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

/**
 * @brief Registers an allocated expression for cleanup.
 * @param expr The expression to register.
 */
void Parser::registerExpr(Expr<std::string>* expr)
{
  _allocated_exprs.push_back(expr);
}

/**
 * @brief Cleans up all allocated expressions.
 */
void Parser::cleanUpExprs()
{
  for (Expr<std::string>* expr : _allocated_exprs)
  {
    delete expr;
  }
  _allocated_exprs.clear();
}