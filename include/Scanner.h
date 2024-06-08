#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "Token.h"

class Scanner{
    private: 
        const std:: string source;
        std::vector<Token> tokens;
        const std::unordered_map<std::string, TokenType> keywords;

        int start = 0;
        int current = 0;
        int line = 1;
        

        bool isAtEnd();
        bool isDigit(char c);
        bool isAlpha(char c);
        bool isAlphaNumeric(char c);
        void number();
        void string();
        void identifier();
        char peekNext();
        void scanToken();
        bool match(char expected);
        char advance();
        char peek();
        void addToken(TokenType type);
        void addToken(TokenType type, const Token::Literal& literal);

    public: 
        Scanner(const std::string& source);
        std::vector<Token> scanTokens();
};
