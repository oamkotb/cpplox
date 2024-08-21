#include "utils.h"

namespace Lox
{
  /** Flag to indicate if an error has occurred. */
  bool had_error = false;
  
  /** Flag to indicate if a runtime error has occured. */
  bool had_runtime_error = false;

  /**
   * @brief Reports an error with the given message and location.
   * @param line The line number where the error occurred.
   * @param where The context or location of the error.
   * @param message The error message.
   */
  void report(const int& line, const std::string& where, const std::string& message)
  {
    had_error = true;
    std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
  }

  /**
   * @brief Reports a general error with the given message and location.
   * @param line The line number where the error occurred.
   * @param message The error message.
   */
  void error(const int& line, const std::string& message)
  {
    report(line, "", message);
  }

  /**
   * @brief Reports an error related to a specific token.
   * @param token The token that caused the error.
   * @param message The error message to be displayed.
   */
  void error(const Token& token, const std::string& message)
  {
    if (token.type == TokenType::END)
      report(token.line, " at end", message);
    else
      report(token.line, " at '" + token.lexeme + "' ", message);
  }

  /**
   * @brief Reports a specific runtime error.
   * @param error The RuntimeError object that contains the token and message for the error.
   */
  void runtimeError(const RuntimeError& error)
  {
    std::cerr << error.what() << "\n[line " << error.token.line << "]" << std::endl;
    had_runtime_error = true;
  }
}
