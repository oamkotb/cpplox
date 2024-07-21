#include "utils.h"
#include "Parser.h"

/**
 * @brief Parses the tokens into an expression.
 * @tparam R The type of the expression that will be parsed.
 * @return A pointer to the parsed expression or nullptr if an error occurred.
 */
template <class R>
Expr<R>* Parser<R>::parse()
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
 * @tparam R The type of the expression that will be parsed.
 * @return A pointer to the parsed expression.
 */
template <class R>
Expr<R>* Parser<R>::expression()
{
  return conditional();
}

/**
 * @brief Parses a conditional expression.
 * @tparam R The type of the expression that will be parsed.
 * @return A pointer to the parsed conditional expression.
 */
template <class R>
Expr<R>* Parser<R>::conditional()
{
  Expr<R>* expr = comma();

  if (match({QUESTION_MARK}))
  {
    Expr<R>* then_branch = expression();
    consume(COLON, "Expect ':' after then branch of conditional expression.");
    Expr<R>* else_branch = conditional();
    expr = new Expr<R>::Conditional(expr, then_branch, else_branch);
    registerExpr(expr);
  }

  return expr;
}

/**
 * @brief Parses a comma expression.
 * @tparam R The type of the expression that will be parsed.
 * @return A pointer to the parsed comma expression.
 */
template <class R>
Expr<R>* Parser<R>::comma()
{
  Expr<R>* expr = equality();

  while (match({COMMA}))
  {
    Token oper = previous();
    Expr<R>* right = equality();
    expr = new Expr<R>::Binary(expr, oper, right);
    registerExpr(expr);
  }

  return expr;
}

/**
 * @brief Parses an equality expression.
 * @tparam R The type of the expression that will be parsed.
 * @return A pointer to the parsed equality expression.
 */
template <class R>
Expr<R>* Parser<R>::equality()
{
  Expr<R>* expr = comparison();

  while (match({ BANG_EQUAL, EQUAL_EQUAL }))
  {
    Token oper = previous();
    Expr<R>* right = comparison();
    expr = new Expr<R>::Binary(expr, oper, right);
    registerExpr(expr);
  }

  return expr;
}

/**
 * @brief Parses a comparison expression.
 * @tparam R The type of the expression that will be parsed.
 * @return A pointer to the parsed comparison expression.
 */
template <class R>
Expr<R>* Parser<R>::comparison()
{
  Expr<R>* expr = term();

  while (match({ GREATER, GREATER_EQUAL, LESS, LESS_EQUAL }))
  {
    Token oper = previous();
    Expr<R>* right = term();
    expr = new Expr<R>::Binary(expr, oper, right);
    registerExpr(expr);
  }

  return expr;
}

/**
 * @brief Parses a term expression.
 * @tparam R The type of the expression that will be parsed.
 * @return A pointer to the parsed term expression.
 */
template <class R>
Expr<R>* Parser<R>::term()
{
  Expr<R>* expr = factor();

  while (match({ MINUS, PLUS }))
  {
    Token oper = previous();
    Expr<R>* right = factor();
    expr = new Expr<R>::Binary(expr, oper, right);
    registerExpr(expr);
  }

  return expr;
}

/**
 * @brief Parses a factor expression.
 * @tparam R The type of the expression that will be parsed.
 * @return A pointer to the parsed factor expression.
 */
template <class R>
Expr<R>* Parser<R>::factor()
{
  Expr<R>* expr = unary();

  while (match({ SLASH, STAR }))
  {
    Token oper =  previous();
    Expr<R>* right = unary();
    expr = new Expr<R>::Binary(expr, oper, right);
    registerExpr(expr);
  }

  return expr;
}

/**
 * @brief Parses a unary expression.
 * @tparam R The type of the expression that will be parsed.
 * @return A pointer to the parsed unary expression.
 */
template <class R>
Expr<R>* Parser<R>::unary()
{
  if (match({ BANG, MINUS }))
  {
    Token oper = previous();
    Expr<R>* right = unary();
    Expr<R>* expr = new Expr<R>::Unary(oper, right);
    registerExpr(expr);
    return expr;
  }

  return primary();
}

/**
 * @brief Parses a primary expression.
 * @tparam R The type of the expression that will be parsed.
 * @return A pointer to the parsed primary expression.
 */
template <class R>
Expr<R>* Parser<R>::primary()
{
  if (match(FALSE)) return new Expr<R>::Literal(false);
  if (match(TRUE)) return new Expr<R>::Literal(true);
  if (match(NIL)) return new Expr<R>::Literal(std::monostate()); // NULL

  if (match({ NUMBER, STRING }))
  {
    Expr<R>* expr =  new Expr<R>::Literal(previous().literal);
    registerExpr(expr);
    return expr;
  }
  
  if (match(LEFT_PAREN))
  {
    Expr<R>* expr = expression();
    consume(RIGHT_PAREN, "Expect ')' after expression.");
    Expr<R>* grouping = new Expr<R>::Grouping(expr);
    registerExpr(expr);
    return grouping;
  }

  throw error(peek(), "Expect expression.");
}

/**
 * @brief Checks if the current token matches the given type and advances if it does.
 * @tparam R The type of the expression that will be parsed.
 * @param type The token type to match.
 * @return True if the token matches, otherwise false.
 */
template <class R>
bool Parser<R>::match(const TokenType& type)
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
 * @tparam R The type of the expression that will be parsed.
 * @param types The list of token types to match.
 * @return True if the token matches any of the types, otherwise false.
 */
template <class R>
bool Parser<R>::match(const std::vector<TokenType>& types)
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
 * @tparam R The type of the expression that will be parsed.
 * @param type The token type to check.
 * @return True if the token matches, otherwise false.
 */
template <class R>
bool Parser<R>::check(const TokenType& type)
{
  if (isAtEnd()) return false;
  return peek().type == type;
}

/**
 * @brief Checks if the parser has reached the end of the token list.
 * @tparam R The type of the expression that will be parsed.
 * @return True if at the end, otherwise false.
 */
template <class R>
bool Parser<R>::isAtEnd()
{
  return peek().type == END;
}

/**
 * @brief Consumes the current token if it matches the given type, otherwise throws an error.
 * @tparam R The type of the expression that will be parsed.
 * @param type The token type to consume.
 * @param message The error message if the token doesn't match.
 * @return The consumed token.
 */
template <class R>
Token Parser<R>::consume(const TokenType& type, const std::string& message)
{
  if (check(type)) return advance();

  throw error(peek(), message);
}

/**
 * @brief Peeks at the current token.
 * @tparam R The type of the expression that will be parsed.
 * @return The current token.
 */
template <class R>
Token Parser<R>::peek()
{
  return _tokens[_current];
}

/**
 * @brief Advances to the next token and returns the previous token.
 * @tparam R The type of the expression that will be parsed.
 * @return The previous token.
 */
template <class R>
Token Parser<R>::advance()
{
  if (!isAtEnd()) ++_current;
  return previous();
}

/**
 * @brief Returns the previous token.
 * @tparam R The type of the expression that will be parsed.
 * @return The previous token.
 */
template <class R>
Token Parser<R>::previous()
{
  return _tokens[_current - 1];
}

/**
 * @brief Creates a parse error, logs it, and returns a ParseError object.
 * @tparam R The type of the expression that will be parsed.
 * @param token The token where the error occurred.
 * @param message The error message.
 * @return A ParseError object.
 */
template <class R>
ParseError Parser<R>::error(const Token& token, const std::string& message)
{
  Lox::error(token, message);
  cleanUpExprs();
  return ParseError(message);
}

/**
 * @brief Synchronizes the parser state after an error.
 * @tparam R The type of the expression that will be parsed.
 */
template <class R>
void Parser<R>::synchronize()
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
 * @tparam R The type of the expression that will be parsed.
 * @param expr The expression to register.
 */
template <class R>
void Parser<R>::registerExpr(Expr<R>* expr)
{
  _allocated_exprs.push_back(expr);
}

/**
 * @brief Cleans up all allocated expressions.
 * @tparam R The type of the expression that will be parsed.
 */
template <class R>
void Parser<R>::cleanUpExprs()
{
  for (Expr<R>* expr : _allocated_exprs)
  {
    delete expr;
  }
  _allocated_exprs.clear();
}
