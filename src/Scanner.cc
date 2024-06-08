#include "Scanner.h"

Scanner::Scanner(const std::string& source)
    : source(source),
      keywords({
        {"and", AND},
        {"class", CLASS},
        {"else", ELSE},
        {"false", FALSE},
        {"for", FOR},
        {"fun", FUN},
        {"if", IF},
        {"nil", NIL},
        {"or", OR},
        {"print", PRINT},
        {"return", RETURN},
        {"super", SUPER},
        {"this", THIS},
        {"true", TRUE},
        {"var", VAR},
        {"while", WHILE}
      }) {}

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
        case '!':
                  addToken(match('=') ? BANG_EQUAL : BANG);
                  break;
        case '<':
                  addToken(match('=') ? EQUAL_EQUAL : EQUAL);
                  break;
        case '>':
                  addToken(match('=') ? LESS_EQUAL : EQUAL);
                  break;
        case '/':
                  if (match('/')){
                    // A comment goes until the end of the line.
                    while (peek() != '\n' && !isAtEnd()) advance();
                  } else {
                    addToken(SLASH);
                  }
                  break;
        case ' ':
        case '\r':
        case '\t':
                  // Ignore white space.
                  break;
        case '\n':
                  line++;
                  break;
        case '"': string(); break;
        case 'o':
                  if (match('r')){
                    addToken(OR);
                  }
                  break;
        default:
                  if (isDigit(c)){
                    number();
                  } else if (isAlpha(c)) {
                    identifier();
                  } else {
                  Lox::error(line, "Unexpected character.");
                  }
                  break;
    }
}

void Scanner::identifier(){
    while (isAlphaNumeric(peek())) advance();
    addToken(IDENTIFIER);
}

bool Scanner::isAlpha(char c){
    return (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') ||
        c == '_';
}

bool Scanner::isAlphaNumeric(char c){
    return isAlpha(c) || isDigit(c);
}

bool Scanner::isDigit(char c){
    return c >= '0' && c <= '9';
}

void Scanner::number(){
    while (isDigit(peek())) advance();

    // Look for a fractional part
    if (peek() == '.' && isDigit(peekNext())){
        // Consume the "."
        advance();

        while (isDigit(peek())) advance();
    }

    addToken(NUMBER,
            std::stod(source.substr(start, current-start)));
}

void Scanner::string(){
    while(peek() != '"' && !isAtEnd()){
        if (peek() == '\n') line++;
        advance();
    }

    if (isAtEnd()){
        Lox::error(line, "Undetermined string.");
        return;
    }

    // The closing ".
    advance();

    std::string value = source.substr(start+1, current - start- 2);
    addToken(STRING, value);
}

char Scanner::peek(){
    if (isAtEnd()) return '\0';
    return source.at(current);
}

char Scanner::peekNext(){
    if (current + 1 >= source.length()) return '\0';
    return source.at(current+1);
}

bool Scanner::match(char expected){
    if (isAtEnd()) return false; 
    if (source.at(current) != expected) return false;

    current++;
    return true;
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
