#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <stack>
#include "token.h"

using namespace std;

class Lexer {
public:
    Lexer(const string& input);
    Token getNextToken();
    bool hasError() const { return errorOccurred; }
    const string& getErrorMessage() const { return errorMessage; }

private:
    string input;
    size_t position;
    int line;
    int column;
    stack<int> indentationStack;
    bool errorOccurred;
    string errorMessage;

    // Helper methods
    char peek() const;
    char advance();
    bool isAtEnd() const;
    void skipWhitespace();
    bool match(char expected);
    
    // Token processing methods
    Token handleIdentifier();
    Token handleNumber();
    Token handleString();
    Token handleOperator();
    Token handleIndentation();
    void handleComment();
    
    // Error handling
    void setError(const string& message);
    
    // Character classification helpers
    bool isDigit(char c) const;
    bool isAlpha(char c) const;
    bool isAlphaNumeric(char c) const;
    
    // Keyword checking
    TokenType checkKeyword(const string& identifier) const;
};

#endif // LEXER_H 