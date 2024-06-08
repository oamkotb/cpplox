#include <unordered_map>
#include <string>
#include <vector>
#include "Token.h"

class Scanner {
private:
    std::unordered_map<std::string, TokenType> keywords;
    std::string source;
    std::vector<Token> tokens;

    int start = 0;
    int current = 0;
    int line = 1;

    // Initialize keywords in a private method
    void initializeKeywords() {
        keywords["and"] = AND;
        keywords["class"] = CLASS;
        keywords["else"] = ELSE;
        keywords["false"] = FALSE;
        keywords["for"] = FOR;
        keywords["fun"] = FUN;
        keywords["if"] = IF;
        keywords["nil"] = NIL;
        keywords["or"] = OR;
        keywords["print"] = PRINT;
        keywords["super"] = SUPER;
        keywords["this"] = THIS;
        keywords["true"] = TRUE;
        keywords["var"] = VAR;
        keywords["while"] = WHILE;
    }

public:
    // Constructor
    Scanner(std::string source) : source(source) {
        initializeKeywords();
    }
};
int main() {
    // Example usage
    Scanner scanner("source code here");
    return 0;
}
