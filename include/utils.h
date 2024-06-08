#pragma once

#include <string>

/** 
 * @file utils.h
 * @brief Utility functions for error reporting in the Lox interpreter.
 */

namespace Lox
{
  /** Flag to indicate if an error has occurred. */
  static bool had_error = false;
  /**
   * @brief Prints an error message to standard error stream.
   * 
   * This function sets the had_error flag and prints an error message
   * formatted with the line number and location of the error.
   * 
   * @param line The line number where the error occurred.
   * @param where Additional information about the error location.
   * @param message The error message to be reported.
   */
  void report(int line, std::string where, std::string message);

  /**
   * @brief Reports an error with a given line number and message.
   * 
   * This function is an inline wrapper that calls the report function.
   * 
   * @param line The line number where the error occurred.
   * @param message The error message to be reported.
   */
  void error(int line, std::string message);

}
