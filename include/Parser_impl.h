#include "utils.h"
#include "Parser.h"

/**
 * @brief Parses the tokens into a list of statements.
 * @tparam R The type of the expression that will be parsed.
 * @return A vector of smart pointers to the parsed statements.
 */
template <class R>
std::vector<std::shared_ptr<Stmt<R>>> Parser<R>::parse()
{
  std::vector<std::shared_ptr<Stmt<R>>> statements;

  while (!isAtEnd())
  {
    std::shared_ptr<Stmt<R>> stmt = declaration();
    if (stmt != nullptr)
      statements.push_back(stmt);
  }

  return statements;
}

/**
 * @brief Parses an expression.
 * @tparam R The type of the expression that will be parsed.
 * @return A smart pointer to the parsed expression.
 */
template <class R>
std::shared_ptr<Expr<R>> Parser<R>::expression()
{
  return comma();
}

/**
 * @brief Parses a declaration statement.
 * @tparam R The type of the expression that will be parsed.
 * @return A smart pointer to the parsed declaration statement.
 */
template <class R>
std::shared_ptr<Stmt<R>> Parser<R>::declaration()
{
  try
  {
    if (match(VAR))
      return varDeclaration();

    return statement();
  }
  catch (const ParseError& error)
  {
    synchronize();
    return nullptr;
  }
}

/**
 * @brief Parses a statement.
 * @tparam R The type of the expression that will be parsed.
 * @return A smart pointer to the parsed statement.
 */
template <class R>
std::shared_ptr<Stmt<R>> Parser<R>::statement()
{
  if (match(PRINT)) return printStatement();
  if (match(LEFT_BRACE)) return std::make_shared<typename Stmt<R>::Block>(block());

  return expressionStatement();
}

/**
 * @brief Parses a block of statements enclosed in braces.
 * @tparam R The type of the expression that will be parsed.
 * @return A vector of smart pointers to the parsed block statements.
 */
template <class R>
std::vector<std::shared_ptr<const Stmt<R>>> Parser<R>::block()
{
  std::vector<std::shared_ptr<const Stmt<R>>> statements; 

  while (!check(RIGHT_BRACE) && !isAtEnd())
    statements.push_back(declaration());

  consume(RIGHT_BRACE, "Expect '}' after block.");
  return statements;
}

/**
 * @brief Parses a variable declaration statement.
 * @tparam R The type of the expression that will be parsed.
 * @return A smart pointer to the parsed variable declaration statement.
 */
template <class R>
std::shared_ptr<Stmt<R>> Parser<R>::varDeclaration()
{
  Token name = consume(IDENTIFIER, "Expect variable name.");
  std::shared_ptr<Expr<R>> initializer = nullptr;

  if (match(EQUAL))
    initializer = expression();

  consume(SEMICOLON, "Expect ';' after variable declaration.");
  return std::make_shared<typename Stmt<R>::Var>(name, initializer);
}

/**
 * @brief Parses a print statement.
 * @tparam R The type of the expression that will be parsed.
 * @return A smart pointer to the parsed print statement.
 */
template <class R>
std::shared_ptr<Stmt<R>> Parser<R>::printStatement()
{
  std::shared_ptr<Expr<R>> value = expression();
  consume(SEMICOLON, "Expect ';' after value.");

  return std::make_shared<typename Stmt<R>::Print>(value);
}

/**
 * @brief Parses an expression statement.
 * @tparam R The type of the expression that will be parsed.
 * @return A smart pointer to the parsed expression statement.
 */
template <class R>
std::shared_ptr<Stmt<R>> Parser<R>::expressionStatement()
{
  std::shared_ptr<Expr<R>> expr = expression();
  consume(SEMICOLON, "Expect ';' after expression.");

  return std::make_shared<typename Stmt<R>::Expression>(expr);
}

/**
 * @brief Parses an assignment expression.
 * @tparam R The type of the expression that will be parsed.
 * @return A smart pointer to the parsed assignment expression.
 */
template <class R>
std::shared_ptr<Expr<R>> Parser<R>::assignment()
{
  std::shared_ptr<Expr<R>> expr = conditional();

  if (match(EQUAL))
  {
    Token equals = previous();
    std::shared_ptr<Expr<R>> value = assignment();
    
    // Attempt to cast expr to a std::shared_ptr of Expr<R>::Variable
    auto variableExpr = std::dynamic_pointer_cast<typename Expr<R>::Variable>(expr);
    if (variableExpr) {
        Token name = variableExpr->name;
        return std::make_shared<typename Expr<R>::Assign>(name, value);
    }

    error(equals, "Invalid assignment target.");
  }

  
  return expr;
}

/**
 * @brief Parses a conditional expression.
 * @tparam R The type of the expression that will be parsed.
 * @return A smart pointer to the parsed conditional expression.
 */
template <class R>
std::shared_ptr<Expr<R>> Parser<R>::conditional()
{
  std::shared_ptr<Expr<R>> expr = equality();

  if (match(QUESTION_MARK))
  {
    std::shared_ptr<Expr<R>> then_branch = expression();
    consume(COLON, "Expect ':' after then branch of conditional expression.");
    std::shared_ptr<Expr<R>> else_branch = conditional();
    expr = std::make_shared<typename Expr<R>::Conditional>(expr, then_branch, else_branch);
  }

  return expr;
}

/**
 * @brief Parses a comma expression.
 * @tparam R The type of the expression that will be parsed.
 * @return A smart pointer to the parsed comma expression.
 */
template <class R>
std::shared_ptr<Expr<R>> Parser<R>::comma()
{
  std::shared_ptr<Expr<R>> expr = assignment();

  while (match(COMMA))
  {
    Token oper = previous();
    std::shared_ptr<Expr<R>> right = assignment();
    expr = std::make_shared<typename Expr<R>::Binary>(expr, oper, right);
  }

  return expr;
}

/**
 * @brief Parses an equality expression.
 * @tparam R The type of the expression that will be parsed.
 * @return A smart pointer to the parsed equality expression.
 */
template <class R>
std::shared_ptr<Expr<R>> Parser<R>::equality()
{
  std::shared_ptr<Expr<R>> expr = comparison();

  while (match({ BANG_EQUAL, EQUAL_EQUAL }))
  {
    Token oper = previous();
    std::shared_ptr<Expr<R>> right = comparison();
    expr = std::make_shared<typename Expr<R>::Binary>(expr, oper, right);
  }

  return expr;
}

/**
 * @brief Parses a comparison expression.
 * @tparam R The type of the expression that will be parsed.
 * @return A smart pointer to the parsed comparison expression.
 */
template <class R>
std::shared_ptr<Expr<R>> Parser<R>::comparison()
{
  std::shared_ptr<Expr<R>> expr = term();

  while (match({ GREATER, GREATER_EQUAL, LESS, LESS_EQUAL }))
  {
    Token oper = previous();
    std::shared_ptr<Expr<R>> right = term();
    expr = std::make_shared<typename Expr<R>::Binary>(expr, oper, right);
  }

  return expr;
}

/**
 * @brief Parses a term expression.
 * @tparam R The type of the expression that will be parsed.
 * @return A smart pointer to the parsed term expression.
 */
template <class R>
std::shared_ptr<Expr<R>> Parser<R>::term()
{
  std::shared_ptr<Expr<R>> expr = factor();

  while (match({ MINUS, PLUS }))
  {
    Token oper = previous();
    std::shared_ptr<Expr<R>> right = factor();
    expr = std::make_shared<typename Expr<R>::Binary>(expr, oper, right);
  }

  return expr;
}

/**
 * @brief Parses a factor expression.
 * @tparam R The type of the expression that will be parsed.
 * @return A smart pointer to the parsed factor expression.
 */
template <class R>
std::shared_ptr<Expr<R>> Parser<R>::factor()
{
  std::shared_ptr<Expr<R>> expr = unary();

  while (match({ SLASH, STAR }))
  {
    Token oper = previous();
    std::shared_ptr<Expr<R>> right = unary();
    expr = std::make_shared<typename Expr<R>::Binary>(expr, oper, right);
  }

  return expr;
}

/**
 * @brief Parses a unary expression.
 * @tparam R The type of the expression that will be parsed.
 * @return A smart pointer to the parsed unary expression.
 */
template <class R>
std::shared_ptr<Expr<R>> Parser<R>::unary()
{
  if (match({ BANG, MINUS }))
  {
    Token oper = previous();
    std::shared_ptr<Expr<R>> right = unary();
    return std::make_shared<typename Expr<R>::Unary>(oper, right);
  }

  return primary();
}

/**
 * @brief Parses a primary expression.
 * @tparam R The type of the expression that will be parsed.
 * @return A smart pointer to the parsed primary expression.
 */
template <class R>
std::shared_ptr<Expr<R>> Parser<R>::primary()
{
  if (match(FALSE))
    return std::make_shared<typename Expr<R>::Literal>(false);

  if (match(TRUE))
    return std::make_shared<typename Expr<R>::Literal>(true);

  if (match(NIL))
    return std::make_shared<typename Expr<R>::Literal>(std::monostate()); // NULL

  if (match({ NUMBER, STRING }))
    return std::make_shared<typename Expr<R>::Literal>(previous().literal);

  if (match(IDENTIFIER))
    return std::make_shared<typename Expr<R>::Variable>(previous());

  if (match(LEFT_PAREN))
  {
    std::shared_ptr<Expr<R>> expr = expression();
    consume(RIGHT_PAREN, "Expect ')' after expression.");
    return std::make_shared<typename Expr<R>::Grouping>(expr);
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