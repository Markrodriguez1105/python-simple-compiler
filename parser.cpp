#include "parser.h"
#include <cctype>
#include <stack>
#include <iomanip>
#include <iostream>

using namespace std;

string Parser::tokenTypeToString(TokenType type) const {
    switch (type) {
        case TokenType::DEF: return "KEYWORD_DEF";
        case TokenType::IF: return "KEYWORD_IF";
        case TokenType::ELIF: return "KEYWORD_ELIF";
        case TokenType::ELSE: return "KEYWORD_ELSE";
        case TokenType::WHILE: return "KEYWORD_WHILE";
        case TokenType::FOR: return "KEYWORD_FOR";
        case TokenType::IN: return "KEYWORD_IN";
        case TokenType::RETURN: return "KEYWORD_RETURN";
        case TokenType::PASS: return "KEYWORD_PASS";
        case TokenType::BREAK: return "KEYWORD_BREAK";
        case TokenType::CONTINUE: return "KEYWORD_CONTINUE";
        case TokenType::PLUS: return "OPERATOR_PLUS";
        case TokenType::MINUS: return "OPERATOR_MINUS";
        case TokenType::MULTIPLY: return "OPERATOR_MULTIPLY";
        case TokenType::DIVIDE: return "OPERATOR_DIVIDE";
        case TokenType::ASSIGN: return "OPERATOR_ASSIGN";
        case TokenType::EQUALS: return "OPERATOR_EQUALS";
        case TokenType::NOT_EQUALS: return "OPERATOR_NOT_EQUALS";
        case TokenType::LESS_THAN: return "OPERATOR_LESS_THAN";
        case TokenType::GREATER_THAN: return "OPERATOR_GREATER_THAN";
        case TokenType::LESS_EQUAL: return "OPERATOR_LESS_EQUAL";
        case TokenType::GREATER_EQUAL: return "OPERATOR_GREATER_EQUAL";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::INTEGER: return "LITERAL_INTEGER";
        case TokenType::FLOAT: return "LITERAL_FLOAT";
        case TokenType::STRING: return "LITERAL_STRING";
        default: return "OTHER";
    }
}

void Parser::addSymbol(const string& name, const string& type, const string& dataType) {
    SymbolInfo info;
    info.type = type;
    info.dataType = dataType;
    info.lineNumber = currentLine;
    info.scope = currentScope;
    symbolTable[name] = info;
}

void Parser::addToken(const string& lexeme, const string& tokenType) {
    TokenInfo info;
    info.lexeme = lexeme;
    info.tokenType = tokenType;
    info.lineNumber = currentLine;
    info.column = currentPos - lexeme.length() + 1;
    tokenTable.push_back(info);
}

void Parser::printSymbolTable() const {
    cout << "\nSymbol Table:\n";
    cout << setw(20) << left << "Name"
         << setw(15) << left << "Type"
         << setw(15) << left << "Data Type"
         << setw(10) << left << "Line"
         << setw(10) << left << "Scope" << endl;
    cout << string(70, '-') << endl;

    for (const auto& entry : symbolTable) {
        cout << setw(20) << left << entry.first
             << setw(15) << left << entry.second.type
             << setw(15) << left << entry.second.dataType
             << setw(10) << left << entry.second.lineNumber
             << setw(10) << left << entry.second.scope << endl;
    }
    cout << endl;
}

void Parser::printTokenTable() const {
    cout << "\nLexemes and Tokens Table:\n";
    cout << setw(20) << left << "Lexeme"
         << setw(25) << left << "Token Type"
         << setw(10) << left << "Line"
         << setw(10) << left << "Column" << endl;
    cout << string(65, '-') << endl;

    for (const auto& token : tokenTable) {
        cout << setw(20) << left << token.lexeme
             << setw(25) << left << token.tokenType
             << setw(10) << left << token.lineNumber
             << setw(10) << left << token.column << endl;
    }
    cout << endl;
}

void Parser::skipWhitespace()
{
    while (currentPos < code.length() && isspace(code[currentPos]))
    {
        if (code[currentPos] == '\n')
        {
            currentLine++;
        }
        currentPos++;
    }
}

bool Parser::isIdentifierChar(char c)
{
    return isalnum(c) || c == '_';
}

bool Parser::isNumber(char c)
{
    return isdigit(c);
}

void Parser::parseIndentation()
{
    int spaces = 0;
    while (currentPos < code.length() && code[currentPos] == ' ')
    {
        spaces++;
        currentPos++;
    }

    // Skip empty lines
    size_t tempPos = currentPos;
    while (tempPos < code.length() && code[tempPos] != '\n')
    {
        if (!isspace(code[tempPos]) && code[tempPos] != '#')
            break;
        tempPos++;
    }
    if (tempPos < code.length() && (code[tempPos] == '\n' || code[tempPos] == '#'))
        return;

    if (spaces % 4 != 0)
    {
        error = true;
        errorOccurred = true;
        errorMessage = "Indentation must be a multiple of 4 spaces at line " + to_string(currentLine);
        return;
    }

    int newLevel = spaces / 4;
    
    // Check if we're inside a block that requires indentation
    if (requiresIndent && newLevel == 0)
    {
        error = true;
        errorOccurred = true;
        errorMessage = "Expected indented block at line " + to_string(currentLine);
        return;
    }
    
    if (newLevel > indentLevel + 1)
    {
        error = true;
        errorOccurred = true;
        errorMessage = "Too many indentation levels at line " + to_string(currentLine);
        return;
    }

    indentLevel = newLevel;
    requiresIndent = false;
}

void Parser::parseStatement()
{
    skipWhitespace();

    if (currentPos >= code.length())
    {
        return;
    }

    // Check for basic syntax elements
    if (code[currentPos] == '#')
    {
        string comment;
        while (currentPos < code.length() && code[currentPos] != '\n')
        {
            comment += code[currentPos];
            currentPos++;
        }
        addToken(comment, "COMMENT");
        return;
    }

    // Store the start of the line for checking
    size_t lineStart = currentPos;
    size_t lineEnd = code.find('\n', currentPos);
    if (lineEnd == string::npos)
    {
        lineEnd = code.length();
    }

    string line = code.substr(lineStart, lineEnd - lineStart);
    
    // Check parentheses matching
    stack<char> brackets;
    for (size_t i = 0; i < line.length(); i++)
    {
        char c = line[i];
        if (c == '(' || c == '[' || c == '{')
        {
            brackets.push(c);
            addToken(string(1, c), "DELIMITER");
        }
        else if (c == ')' || c == ']' || c == '}')
        {
            if (brackets.empty())
            {
                error = true;
                errorOccurred = true;
                errorMessage = "Unmatched closing bracket at line " + to_string(currentLine);
                return;
            }
            char open = brackets.top();
            brackets.pop();
            addToken(string(1, c), "DELIMITER");
            if ((c == ')' && open != '(') ||
                (c == ']' && open != '[') ||
                (c == '}' && open != '{'))
            {
                error = true;
                errorOccurred = true;
                errorMessage = "Mismatched brackets at line " + to_string(currentLine);
                return;
            }
        }
    }
    if (!brackets.empty())
    {
        error = true;
        errorOccurred = true;
        errorMessage = "Unclosed bracket at line " + to_string(currentLine);
        return;
    }
    
    // Trim trailing comments from the line
    size_t commentPos = line.find('#');
    if (commentPos != string::npos)
    {
        string comment = line.substr(commentPos);
        addToken(comment, "COMMENT");
        line = line.substr(0, commentPos);
    }

    // Check for statements that require colons
    if (line.find("def ") == 0)
    {
        size_t nameStart = 4; // after "def "
        size_t nameEnd = line.find('(');
        if (nameEnd != string::npos)
        {
            string funcName = line.substr(nameStart, nameEnd - nameStart);
            addSymbol(funcName, "Function", "void");
            addToken("def", "KEYWORD");
            addToken(funcName, "IDENTIFIER");
        }
    }
    else if (line.find("if ") == 0 || 
             line.find("elif ") == 0 || 
             line.find("else") == 0 ||
             line.find("while ") == 0 ||
             line.find("for ") == 0 ||
             line.find("class ") == 0)
    {
        string keyword = line.substr(0, line.find(' '));
        addToken(keyword, "KEYWORD");
    }

    // Check for variable assignments
    size_t assignPos = line.find('=');
    if (assignPos != string::npos && line[assignPos + 1] != '=')
    {
        string varName = line.substr(0, assignPos);
        // Trim whitespace
        varName.erase(0, varName.find_first_not_of(" \t"));
        varName.erase(varName.find_last_not_of(" \t") + 1);
        if (!varName.empty())
        {
            addSymbol(varName, "Variable", "unknown");
            addToken(varName, "IDENTIFIER");
            addToken("=", "OPERATOR");
        }
    }

    // Move to next line
    currentPos = lineEnd;
    if (currentPos < code.length())
    {
        currentPos++;
        currentLine++;
    }
}

void Parser::parse()
{
    while (currentPos < code.length() && !error)
    {
        parseIndentation();
        if (error)
            return;

        parseStatement();
        if (error)
            return;
    }
    
    // After successful parsing, print the tables
    if (!error)
    {
        printSymbolTable();
        printTokenTable();
    }
}