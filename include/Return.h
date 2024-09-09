#pragma once

#include <stdexcept>

#include "Token.h"

class Return : public std::exception
{
public:
  Return(const LiteralValue& value)
    : value(value) {}

  const LiteralValue value;
};