#include "Token.h"

/**
 * @brief Stream insertion operator for the Token class.
 * @param os The output stream.
 * @param token The token to be inserted into the stream.
 * @return The output stream.
 */
std::string tokenTypeToString(TokenType type)
{
  switch (type) {
    case LEFT_PAREN: return "LEFT_PAREN";
    case RIGHT_PAREN: return "RIGHT_PAREN";
    case LEFT_BRACE: return "LEFT_BRACE";
    case RIGHT_BRACE: return "RIGHT_BRACE";
    case COMMA: return "COMMA";
    case DOT: return "DOT";
    case MINUS: return "MINUS";
    case PLUS: return "PLUS";
    case SEMICOLON: return "SEMICOLON";
    case SLASH: return "SLASH";
    case STAR: return "STAR";
    case BANG: return "BANG";
    case BANG_EQUAL: return "BANG_EQUAL";
    case EQUAL: return "EQUAL";
    case EQUAL_EQUAL: return "EQUAL_EQUAL";
    case GREATER: return "GREATER";
    case GREATER_EQUAL: return "GREATER_EQUAL";
    case LESS: return "LESS";
    case LESS_EQUAL: return "LESS_EQUAL";
    case IDENTIFIER: return "IDENTIFIER";
    case STRING: return "STRING";
    case NUMBER: return "NUMBER";
    case AND: return "AND";
    case CLASS: return "CLASS";
    case ELSE: return "ELSE";
    case FALSE: return "FALSE";
    case FUN: return "FUN";
    case FOR: return "FOR";
    case IF: return "IF";
    case NIL: return "NIL";
    case OR: return "OR";
    case PRINT: return "PRINT";
    case RETURN: return "RETURN";
    case SUPER: return "SUPER";
    case THIS: return "THIS";
    case TRUE: return "TRUE";
    case VAR: return "VAR";
    case WHILE: return "WHILE";
    case END: return "EOF"; // To make match with jlox
    default: return "UNKNOWN";
  }
}

std::ostream& operator<<(std::ostream& os, const Token& token)
{
  os << tokenTypeToString(token.type) << " " << token.lexeme << " ";
  std::visit([&os](auto&& arg) 
  { 
    if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, std::monostate>)
      os << "null";
    else
      // Print string literals without enclosing quotes
      os << arg;
  }, token.literal);
  
  return os;
}
