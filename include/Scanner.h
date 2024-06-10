/**
 * @file Scanner.h
 * @brief Header file for the Scanner class which is responsible for 
 *        scanning the source code and generating a list of tokens.
 *
 * This file contains the declaration of the Scanner class, which 
 * includes methods for scanning and tokenizing source code for a 
 * programming language. The Scanner class breaks down the input source 
 * code into a sequence of tokens that can be used by a parser or 
 * interpreter.
 */

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Token.h"
#include "utils.h"

/**
 * @brief The Scanner class is responsible for scanning the source code
 * and generating a list of tokens.
 */
class Scanner
{
public: 
    /**
     * @brief Constructs a Scanner object with the given source code.
     * @param source The source code to scan.
     */
    Scanner(const std::string& source);

    /**
     * @brief Scans the source code and returns a vector of tokens.
     * @return A vector containing all the tokens found in the source code.
     */
    std::vector<Token> scanTokens();

private: 
    /// The source code to be scanned.
    std::string _source;
    
    /// Start position of the current lexeme being scanned.
    size_t _start = 0;

    /// Current position in the source code.
    size_t _current = 0;

    /// Current line number in the source code.
    size_t _line = 1;

    /// Vector to store the scanned tokens.
    std::vector<Token> _tokens;

    /// Unordered map of keywords and their corresponding token types.
    const std::unordered_map<std::string, TokenType> _keywords;

    /**
     * @brief Scans the next token from the source code.
     */
    void scanToken();

    /**
     * @brief Adds a token of the given type to the tokens vector.
     * @param type The type of the token to be added.
     */
    void addToken(const TokenType& type);

    /**
     * @brief Adds a token of the given type and literal value to the tokens vector.
     * @param type The type of the token to be added.
     * @param literal The literal value of the token.
     */
    void addToken(const TokenType& type, const Token::Literal& literal);

    /**
     * @brief Checks if the scanner has reached the end of the source code.
     * @return True if the scanner is at the end of the source code, false otherwise.
     */
    bool isAtEnd();

    /**
     * @brief Checks if the given character is a digit.
     * @param c The character to check.
     * @return True if the character is a digit, false otherwise.
     */
    bool isDigit(const char& c);

    /**
     * @brief Checks if the given character is an alphabetic character.
     * @param c The character to check.
     * @return True if the character is alphabetic, false otherwise.
     */
    bool isAlpha(const char& c);

    /**
     * @brief Checks if the given character is an alphanumeric character.
     * @param c The character to check.
     * @return True if the character is alphanumeric, false otherwise.
     */
    bool isAlphaNumeric(const char& c);

    /**
     * @brief Scans a number token from the source code.
     */
    void number();

    /**
     * @brief Scans a string token from the source code.
     */
    void string();

    /**
     * @brief Scans an identifier token from the source code.
     */
    void identifier();

    /**
     * @brief Scans a block comment from the source code.
     */
    void blockComment();

    /**
     * @brief Advances the current position in the source code and returns the current character.
     * @return The current character in the source code.
     */
    char advance();

    /**
     * @brief Checks if the next character in the source code matches the expected character.
     * @param expected The expected character to match.
     * @return True if the next character matches the expected character, false otherwise.
     */
    bool match(const char& expected);

    /**
     * @brief Returns the current character in the source code without advancing the current position.
     * @return The current character in the source code.
     */
    char peek();

    /**
     * @brief Returns the next character in the source code without advancing the current position.
     * @return The next character in the source code.
     */
    char peekNext();
};
