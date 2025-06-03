#include "lexer.h"
#include <unordered_map>

using namespace std;

Lexer::Lexer(const string& input)
    : input(input), position(0), line(1), column(1), errorOccurred(false) {
    indentationStack.push(0);  // Start with 0 indentation
}

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return input[position];
}

char Lexer::advance() {
    if (!isAtEnd()) {
        char current = input[position++];
        if (current == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
        return current;
    }
    return '\0';
}

bool Lexer::isAtEnd() const {
    return position >= input.length();
}

void Lexer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        if (c == ' ' || c == '\r' || c == '\t') {
            advance();
        } else {
            break;
        }
    }
}

bool Lexer::match(char expected) {
    if (isAtEnd() || input[position] != expected) return false;
    position++;
    column++;
    return true;
}

bool Lexer::isDigit(char c) const {
    return c >= '0' && c <= '9';
}

bool Lexer::isAlpha(char c) const {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Lexer::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}

void Lexer::setError(const string& message) {
    errorOccurred = true;
    errorMessage = "Line " + to_string(line) + ", Column " + to_string(column) + ": " + message;
}

TokenType Lexer::checkKeyword(const string& identifier) const {
    static const unordered_map<string, TokenType> keywords = {
        {"def", TokenType::DEF},
        {"if", TokenType::IF},
        {"elif", TokenType::ELIF},
        {"else", TokenType::ELSE},
        {"while", TokenType::WHILE},
        {"for", TokenType::FOR},
        {"in", TokenType::IN},
        {"return", TokenType::RETURN},
        {"pass", TokenType::PASS},
        {"break", TokenType::BREAK},
        {"continue", TokenType::CONTINUE}
    };

    auto it = keywords.find(identifier);
    if (it != keywords.end()) {
        return it->second;
    }
    return TokenType::IDENTIFIER;
}

Token Lexer::handleIdentifier() {
    string identifier;
    int startColumn = column;
    
    while (!isAtEnd() && isAlphaNumeric(peek())) {
        identifier += advance();
    }
    
    TokenType type = checkKeyword(identifier);
    return Token(type, identifier, line, startColumn);
}

Token Lexer::handleNumber() {
    string number;
    bool isFloat = false;
    int startColumn = column;
    
    while (!isAtEnd() && (isDigit(peek()) || peek() == '.')) {
        char c = peek();
        if (c == '.') {
            if (isFloat) {
                setError("Invalid number format: multiple decimal points");
                return Token(TokenType::ERROR, number, line, startColumn);
            }
            isFloat = true;
        }
        number += advance();
    }
    
    return Token(isFloat ? TokenType::FLOAT : TokenType::INTEGER, number, line, startColumn);
}

Token Lexer::handleString() {
    string str;
    int startColumn = column;
    char quote = advance(); // Skip the opening quote
    
    while (!isAtEnd() && peek() != quote) {
        if (peek() == '\n') {
            setError("Unterminated string literal");
            return Token(TokenType::ERROR, str, line, startColumn);
        }
        str += advance();
    }
    
    if (isAtEnd()) {
        setError("Unterminated string literal");
        return Token(TokenType::ERROR, str, line, startColumn);
    }
    
    advance(); // Skip the closing quote
    return Token(TokenType::STRING, str, line, startColumn);
}

Token Lexer::handleIndentation() {
    int spaces = 0;
    int startColumn = column;
    
    while (peek() == ' ' || peek() == '\t') {
        if (peek() == ' ') spaces++;
        else spaces += 4; // Convert tab to 4 spaces
        advance();
    }
    
    int currentIndent = indentationStack.top();
    
    if (spaces > currentIndent) {
        indentationStack.push(spaces);
        return Token(TokenType::INDENT, "", line, startColumn);
    } else if (spaces < currentIndent) {
        indentationStack.pop();
        return Token(TokenType::DEDENT, "", line, startColumn);
    }
    
    return getNextToken(); // Skip this token and get the next one
}

void Lexer::handleComment() {
    while (!isAtEnd() && peek() != '\n') {
        advance();
    }
}

Token Lexer::getNextToken() {
    skipWhitespace();
    
    if (isAtEnd()) {
        return Token(TokenType::END_OF_FILE, "", line, column);
    }
    
    char c = peek();
    int startColumn = column;
    
    // Handle indentation at the start of a line
    if (column == 1 && c != '\n') {
        return handleIndentation();
    }
    
    // Handle different token types
    if (isAlpha(c)) {
        return handleIdentifier();
    }
    
    if (isDigit(c)) {
        return handleNumber();
    }
    
    switch (c) {
        case '\n':
            advance();
            return Token(TokenType::NEWLINE, "\\n", line-1, startColumn);
            
        case '#':
            handleComment();
            return getNextToken();
            
        case '"':
        case '\'':
            return handleString();
            
        case '+': advance(); return Token(TokenType::PLUS, "+", line, startColumn);
        case '-': advance(); return Token(TokenType::MINUS, "-", line, startColumn);
        case '*': advance(); return Token(TokenType::MULTIPLY, "*", line, startColumn);
        case '/': advance(); return Token(TokenType::DIVIDE, "/", line, startColumn);
        
        case '=':
            advance();
            if (match('=')) return Token(TokenType::EQUALS, "==", line, startColumn);
            return Token(TokenType::ASSIGN, "=", line, startColumn);
            
        case '!':
            advance();
            if (match('=')) return Token(TokenType::NOT_EQUALS, "!=", line, startColumn);
            setError("Expected '=' after '!'");
            return Token(TokenType::ERROR, "!", line, startColumn);
            
        case '<':
            advance();
            if (match('=')) return Token(TokenType::LESS_EQUAL, "<=", line, startColumn);
            return Token(TokenType::LESS_THAN, "<", line, startColumn);
            
        case '>':
            advance();
            if (match('=')) return Token(TokenType::GREATER_EQUAL, ">=", line, startColumn);
            return Token(TokenType::GREATER_THAN, ">", line, startColumn);
            
        case '(': advance(); return Token(TokenType::LPAREN, "(", line, startColumn);
        case ')': advance(); return Token(TokenType::RPAREN, ")", line, startColumn);
        case '{': advance(); return Token(TokenType::LBRACE, "{", line, startColumn);
        case '}': advance(); return Token(TokenType::RBRACE, "}", line, startColumn);
        case '[': advance(); return Token(TokenType::LBRACKET, "[", line, startColumn);
        case ']': advance(); return Token(TokenType::RBRACKET, "]", line, startColumn);
        case ':': advance(); return Token(TokenType::COLON, ":", line, startColumn);
        case ',': advance(); return Token(TokenType::COMMA, ",", line, startColumn);
        case '.': advance(); return Token(TokenType::DOT, ".", line, startColumn);
    }
    
    setError("Unexpected character: " + string(1, c));
    advance();
    return Token(TokenType::ERROR, string(1, c), line, startColumn);
} 