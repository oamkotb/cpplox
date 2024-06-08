#include "Token.h"

/**
 * @brief Stream insertion operator for the Token class.
 * @param os The output stream.
 * @param token The token to be inserted into the stream.
 * @return The output stream.
 */
std::ostream& operator<<(std::ostream& os, const Token& token)
{
    os << token.type << " " << token.lexeme << " ";
    // std::visit is used with a lambda that handles the different types within the
    // std::variant
    std::visit([&os](auto&& arg) { 
        if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, std::monostate>)
            os << "null";
        else
            os << arg;
    }, token.literal);
    os << token.line;
    return os;
}
