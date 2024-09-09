#pragma once

#include <stdexcept>
#include "Token.h"

/**
 * @brief Exception to signal a return statement.
 *
 * @throw Return Thrown when a `return` statement is executed.
 */
class Return : public std::exception
{
public:
  /**
   * @brief Constructs a Return exception with a return value.
   * @param value The value to be returned by the function.
   */
  Return(const LiteralValue& value)
    : value(value) {}

  const LiteralValue value; ///< The value returned by the function.
};

/**
 * @brief Exception to signal a break statement.
 *
 * @throw Break Thrown to exit the current loop prematurely.
 */
class Break : public std::exception {};

/**
 * @brief Exception to signal a continue statement.
 *
 * @throw Continue Thrown to skip the current loop iteration and continue to the next.
 */
class Continue : public std::exception {};
