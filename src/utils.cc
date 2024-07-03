#include "utils.h"

namespace Lox
{
  /** Flag to indicate if an error has occurred. */
  bool had_error = false;

  /**
   * @brief Reports an error with the given message and location.
   * @param line The line number where the error occurred.
   * @param where The context or location of the error.
   * @param message The error message.
   */
  void report(int line, std::string where, std::string message)
  {
    had_error = true;
    std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
  }

  /**
   * @brief Reports a general error with the given message and location.
   * @param line The line number where the error occurred.
   * @param message The error message.
   */
  void error(int line, std::string message)
  {
    report(line, "", message);
  }
}
