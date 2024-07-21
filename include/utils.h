/** 
 * @file utils.h
 * @brief Utility functions for error reporting in the Lox interpreter.
 */

#pragma once

#include <iostream>

#include "RuntimeError.h"
#include "Token.h"

namespace Lox
{
  /**
   * @brief Flag to indicate if an error has occurred.
   */
  extern bool had_error;

  /**
   * @brief Flag to indicate if a runtime error has occured.
   */
  extern bool had_runtime_error;

  /**
   * @brief Reports an error with the given message and location.
   * @param line The line number where the error occurred.
   * @param where The context or location of the error.
   * @param message The error message.
   */
  void report(const int& line, const std::string& where, const std::string& message);

  /**
   * @brief Reports a general error with the given message and location.
   * @param line The line number where the error occurred.
   * @param message The error message.
   */
  void error(const int& line, const std::string& message);

  /**
   * @brief Reports an error related to a specific token.
   * @param token The token that caused the error.
   * @param message The error message to be displayed.
   */
  void error(const Token& token, const std::string& message);

  /**
   * @brief Reports a specific runtime error.
   * @param error The RuntimeError object that contains the token and message for the error.
   */
  void runtimeError(const RuntimeError& error);

}
