#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

struct SymbolInfo {
    string type;      // Variable, Function, Class, etc.
    string dataType;  // int, float, string, etc.
    int lineNumber;
    int scope;        // Scope level where symbol is defined
};

struct TokenInfo {
    string lexeme;
    string tokenType;
    int lineNumber;
    int column;
};

class Parser
{
public:
    Parser(const string &input) : 
        lexer(input), 
        currentToken(TokenType::ERROR, "", 0, 0),
        errorOccurred(false), 
        code(input), 
        currentPos(0), 
        currentLine(1), 
        indentLevel(0),
        requiresIndent(false),
        currentScope(0),
        error(false) {}
    void parse();
    bool hasError() const { return errorOccurred; }
    const string &getErrorMessage() const { return errorMessage; }
    
    // New methods for symbol and token tables
    void printSymbolTable() const;
    void printTokenTable() const;
    const unordered_map<string, SymbolInfo>& getSymbolTable() const { return symbolTable; }
    const vector<TokenInfo>& getTokenTable() const { return tokenTable; }

private:
    Lexer lexer;
    Token currentToken;
    bool errorOccurred;
    string errorMessage;
    
    // Parser state
    string code;
    size_t currentPos;
    int currentLine;
    int indentLevel;
    bool requiresIndent;
    int currentScope;
    bool error;

    // Symbol and token tables
    unordered_map<string, SymbolInfo> symbolTable;
    vector<TokenInfo> tokenTable;

    // Helper methods
    void skipWhitespace();
    bool isIdentifierChar(char c);
    bool isNumber(char c);
    void parseIndentation();
    void advance();
    bool match(TokenType type);
    void consume(TokenType type, const string &message);
    void setError(const string &message);

    // Symbol table methods
    void addSymbol(const string& name, const string& type, const string& dataType);
    void addToken(const string& lexeme, const string& tokenType);

    // Parsing methods
    void parseProgram();
    void parseStatement();
    void parseExpression();
    void parseFunctionDef();
    void parseIfStatement();
    void parseWhileStatement();
    void parseForStatement();
    void parseBlock();
    void parseAssignment();
    void parseReturnStatement();

    // Expression parsing
    void parseComparison();
    void parseTerm();
    void parseFactor();
    void parsePrimary();
    void parseCall();

    // Helper function to convert TokenType to string
    string tokenTypeToString(TokenType type) const;
};

#endif // PARSER_H