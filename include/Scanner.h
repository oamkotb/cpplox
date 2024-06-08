#pragma once

#include <string>
#include <vector>
#include "Token.h"

class Scanner{
    private: 
        const std:: string source;
        std::vector<Token> tokens;
        int start = 0;
        int current = 0;
        int line = 1;

        bool isAtEnd();
        void scanToken();
        char advance();
        void addToken(TokenType type);
        void addToken(TokenType type, const Token::Literal& literal);

    public: 
        Scanner(const std::string& source);
        std::vector<Token> scanTokens();
};
