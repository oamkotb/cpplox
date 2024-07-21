#include "Scanner.h"

/**
 * @brief Constructor for Scanner class.
 * @param source The source code to be scanned.
 */
Scanner::Scanner(const std::string& source):
  _source(source),
  _keywords({
    {"and", AND},
    {"class", CLASS},
    {"else", ELSE},
    {"false", FALSE},
    {"for", FOR},
    {"fun", FUN},
    {"if", IF},
    {"nil", NIL},
    {"or", OR},
    {"print", PRINT},
    {"return", RETURN},
    {"super", SUPER},
    {"this", THIS},
    {"true", TRUE},
    {"var", VAR},
    {"while", WHILE}
  }) {}

/**
 * @brief Scans the source code and returns a vector of tokens.
 * @return A vector containing all the tokens found in the source code.
 */
std::vector<Token> Scanner::scanTokens()
{
  while (!isAtEnd())
  {
    // We are at the beginning of the next lexeme.
    _start = _current;
    scanToken();
  }

  _tokens.emplace_back(END, "", std::monostate(), _line);
  return _tokens;
}

/**
 * @brief Scans the next token from the source code.
 */
void Scanner::scanToken()
{
  char c = advance();
  switch(c)
  {
    case '(': addToken(LEFT_PAREN); break;
    case ')': addToken(RIGHT_PAREN); break;
    case '{': addToken(LEFT_BRACE); break;
    case '}': addToken(RIGHT_BRACE); break;
    case ',': addToken(COMMA); break;
    case '.': addToken(DOT); break;
    case '-': addToken(MINUS); break;
    case '+': addToken(PLUS); break;
    case ';': addToken(SEMICOLON); break;
    case '*': addToken(STAR); break;
    case ':': addToken(COLON); break;
    case '?': addToken(QUESTION_MARK); break;
    case '!':
      addToken(match('=') ? BANG_EQUAL : BANG);
      break;
    case '=':
      addToken(match('=') ? EQUAL_EQUAL : EQUAL);
      break;
    case '<':
      addToken(match('=') ? LESS_EQUAL : LESS);
      break;
    case '>':
      addToken(match('=') ? GREATER_EQUAL : GREATER);
      break;
    case '/':
      if (match('/'))
        while (peek() != '\n' && !isAtEnd()) advance();
      else if (match('*'))
        blockComment();
      else
        addToken(SLASH);
      break;
    case ' ':
    case '\r':
    case '\t':
      // Ignore whitespace.
      break;
    case '\n':
      ++_line;
      break;
    case '"': string(); break;
    default:
      if (isDigit(c)) number();
      else if (isAlpha(c)) identifier();
      else Lox::error(_line, "Unexpected character.");
    
      break;
  }
}

/**
 * @brief Adds a token of the given type to the tokens vector.
 * @param type The type of the token to be added.
 */
void Scanner::addToken(const TokenType& type)
{
  addToken(type, std::monostate());
}

/**
 * @brief Adds a token of the given type and literal value to the tokens vector.
 * @param type The type of the token to be added.
 * @param literal The literal value of the token.
 */
void Scanner::addToken(const TokenType& type, const LiteralValue& literal)
{
  std::string text = _source.substr(_start, _current - _start);
  _tokens.emplace_back(type, text, literal, _line);
}

/**
 * @brief Checks if the scanner has reached the end of the source code.
 * @return True if the scanner is at the end of the source code, false otherwise.
 */
bool Scanner::isAtEnd()
{
  return _current >= _source.length();
}

/**
 * @brief Checks if the given character is a digit.
 * @param c The character to check.
 * @return True if the character is a digit, false otherwise.
 */
bool Scanner::isDigit(const char& c)
{
  return (c >= '0' && c <= '9');
}

/**
 * @brief Checks if the given character is an alphabetic character.
 * @param c The character to check.
 * @return True if the character is alphabetic, false otherwise.
 */
bool Scanner::isAlpha(const char& c)
{
  return (c >= 'a' && c <= 'z') ||
         (c >= 'A' && c <= 'Z') ||
          c == '_';
}

/**
 * @brief Checks if the given character is an alphanumeric character.
 * @param c The character to check.
 * @return True if the character is alphanumeric, false otherwise.
 */
bool Scanner::isAlphaNumeric(const char& c)
{
  return isAlpha(c) || isDigit(c);
}

/**
 * @brief Scans a number token from the source code.
 */
void Scanner::number()
{
  while (isDigit(peek())) advance();

  // Look for a fractional part
  if (peek() == '.' && isDigit(peekNext()))
  {
    // Consume the "."
    advance();
    while (isDigit(peek())) advance();
  }

  addToken(NUMBER, std::stod(_source.substr(_start, _current - _start)));
}

/**
 * @brief Scans a string token from the source code.
 */
void Scanner::string()
{
  while (peek() != '"' && !isAtEnd())
  {
    if (peek() == '\n') _line++;
    advance();
  }

  if (isAtEnd())
  {
    Lox::error(_line, "Unterminated string.");
    return;
  }

  advance();

  std::string value = _source.substr(_start + 1, _current - _start - 2);
  addToken(STRING, value);
}

/**
 * @brief Scans an identifier token from the source code.
 */
void Scanner::identifier()
{
  while (isAlphaNumeric(peek())) advance();
  addToken(IDENTIFIER);
}

/**
 * @brief Scans a block comment from the source code.
 */
void Scanner::blockComment()
{
  while (true)
  {
    if (isAtEnd())
    {
      Lox::error(_line, "Undetermined block comment.");
      return;
    }

    if (peek() == '*' && peekNext() == '/')
    {
      advance(); // consume the '*'
      advance(); // consume the '/'
      break;
    }

    if (peek() == '\n')
      ++_line;

    advance();
  }
}

/**
 * @brief Advances the current position in the source code and returns the current character.
 * @return The current character in the source code.
 */
char Scanner::advance()
{
  return _source.at(_current++);
}

/**
 * @brief Checks if the next character in the source code matches the expected character.
 * @param expected The expected character to match.
 * @return True if the next character matches the expected character, false otherwise.
 */
bool Scanner::match(const char& expected)
{
  if (isAtEnd() || _source.at(_current) != expected) return false; 

  _current++;
  return true;
}

/**
 * @brief Returns the current character in the source code without advancing the current position.
 * @return The current character in the source code.
 */
char Scanner::peek()
{
  if (isAtEnd()) return '\0';
  return _source.at(_current);
}

/**
 * @brief Returns the next character in the source code without advancing the current position.
 * @return The next character in the source code.
 */
char Scanner::peekNext()
{
  if (_current + 1 >= _source.length()) return '\0';
  return _source.at(_current + 1);
}