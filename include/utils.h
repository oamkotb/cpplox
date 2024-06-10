/** 
 * @file utils.h
 * @brief Utility functions for error reporting in the Lox interpreter.
 */

#pragma once

#include <iostream>

namespace Lox
{
  /**
   * @brief Flag to indicate if an error has occurred.
   */
  extern bool had_error;

  /**
   * @brief Reports an error with the given message and location.
   * @param line The line number where the error occurred.
   * @param where The context or location of the error.
   * @param message The error message.
   */
  void report(int line, std::string where, std::string message);

  /**
   * @brief Reports a general error with the given message and location.
   * @param line The line number where the error occurred.
   * @param message The error message.
   */
  void error(int line, std::string message);
}
