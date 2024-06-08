#include "Scanner.h"

Scanner::Scanner(const std::string& source) : source(source){}

std::vector<Token> Scanner::scanTokens(){
    while (!isAtEnd()){
        // We are the beginning of the next lexeme.
        start = current; 
        scanToken();
    }

    tokens.emplace_back(END, "", std::monostate(), line);
    return tokens;
}

void Scanner::scanToken(){
    char c = advance();
    switch(c) {
        case '(': addToken(LEFT_PAREN); break;
        case ')': addToken(RIGHT_PAREN); break;
        case '{': addToken(LEFT_BRACE); break;
        case '}': addToken(RIGHT_BRACE); break;
        case ',': addToken(COMMA); break;
        case '.': addToken(DOT); break;
        case '-': addToken(MINUS); break;
        case '+': addToken(PLUS); break;
        case ';': addToken(SEMICOLON); break;
        case '*': addToken(STAR); break;

        default:
                  break;
    }
}
bool Scanner::isAtEnd(){
    return current >= source.length();
}

char Scanner::advance(){
    return source.at(current++);
} 

void Scanner::addToken(TokenType type){
    addToken(type, std::monostate());
}

void Scanner::addToken(TokenType type, const Token::Literal& literal){
    std::string text = source.substr(start, current-start);
    tokens.emplace_back(type, text, literal, line);
}
