#ifndef TOKEN_H
#define TOKEN_H

#include <string>

using namespace std;

enum class TokenType {
    // Keywords
    DEF,
    IF,
    ELIF,
    ELSE,
    WHILE,
    FOR,
    IN,
    RETURN,
    PASS,
    BREAK,
    CONTINUE,
    
    // Operators
    PLUS,           // +
    MINUS,          // -
    MULTIPLY,       // *
    DIVIDE,         // /
    ASSIGN,         // =
    EQUALS,         // ==
    NOT_EQUALS,     // !=
    LESS_THAN,      // <
    GREATER_THAN,   // >
    LESS_EQUAL,     // <=
    GREATER_EQUAL,  // >=
    
    // Delimiters
    LPAREN,         // (
    RPAREN,         // )
    LBRACE,         // {
    RBRACE,         // }
    LBRACKET,       // [
    RBRACKET,       // ]
    COLON,          // :
    COMMA,          // ,
    DOT,            // .
    
    // Other
    IDENTIFIER,     // Variable names, function names, etc.
    INTEGER,        // Integer literals
    FLOAT,          // Float literals
    STRING,         // String literals
    INDENT,         // Python indentation
    DEDENT,         // Python dedentation
    NEWLINE,        // End of line
    COMMENT,        // Python comments
    END_OF_FILE,    // End of input
    ERROR           // Invalid token
};

struct Token {
    TokenType type;
    string value;
    int line;
    int column;
    
    Token(TokenType t, const string& v, int l, int c)
        : type(t), value(v), line(l), column(c) {}
};

#endif // TOKEN_H 