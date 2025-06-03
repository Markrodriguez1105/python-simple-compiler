# Python Parser Technical Documentation

## 1. Overview
This parser implements a subset of Python syntax analysis, including lexical analysis and syntax checking. The implementation follows a recursive descent parsing approach and includes symbol table management and token tracking.

## 2. Grammar Specification
The parser implements the following simplified Python grammar in EBNF notation:

```ebnf
program     ::= statement*
statement   ::= simple_stmt | compound_stmt
simple_stmt ::= assignment | expression | return_stmt
compound_stmt ::= if_stmt | while_stmt | for_stmt | function_def

function_def ::= 'def' IDENTIFIER '(' parameter_list? ')' ':' block
parameter_list ::= IDENTIFIER (',' IDENTIFIER)*

if_stmt     ::= 'if' expression ':' block ('elif' expression ':' block)* ('else' ':' block)?
while_stmt  ::= 'while' expression ':' block
for_stmt    ::= 'for' IDENTIFIER 'in' expression ':' block
block       ::= INDENT statement+ DEDENT

assignment  ::= IDENTIFIER '=' expression
expression  ::= comparison
comparison  ::= term (('>='|'<='|'>'|'<'|'=='|'!=') term)*
term        ::= factor ('+'|'-' factor)*
factor      ::= primary ('*'|'/' primary)*
primary     ::= IDENTIFIER | NUMBER | STRING | '(' expression ')' | function_call
function_call ::= IDENTIFIER '(' (expression (',' expression)*)? ')'
```

## 3. Implementation Details

### 3.1 Lexical Analysis
The lexical analyzer (lexer.cpp) implements:
- Token recognition for:
  - Keywords (def, if, elif, else, while, for, in, return)
  - Operators (+, -, *, /, =, ==, !=, <, >, <=, >=)
  - Delimiters (parentheses, brackets, colons)
  - Identifiers
  - Literals (integers, floats, strings)
  - Comments
- Indentation handling for Python's block structure
- Line and column tracking for error reporting

### 3.2 Syntax Analysis
The parser (parser.cpp) implements:
- Recursive descent parsing following the grammar rules
- Error detection and reporting for:
  - Missing colons
  - Incorrect indentation
  - Unmatched parentheses
  - Invalid syntax structures
- Symbol table management
- Token tracking and categorization

### 3.3 Symbol Table
The symbol table maintains:
- Identifier information (variables and functions)
- Type information
- Scope tracking
- Line number references

Structure:
```cpp
struct SymbolInfo {
    string type;      // Variable, Function, Class
    string dataType;  // int, float, string, etc.
    int lineNumber;
    int scope;
};
```

### 3.4 Token Table
Tracks all tokens with:
- Lexeme (actual text)
- Token type
- Line and column numbers
- Categorization of tokens

Structure:
```cpp
struct TokenInfo {
    string lexeme;
    string tokenType;
    int lineNumber;
    int column;
};
```

## 4. Error Handling
The parser implements error detection for:
- Lexical errors:
  - Invalid characters
  - Malformed numbers
  - Unterminated strings
- Syntax errors:
  - Missing colons in compound statements
  - Incorrect indentation
  - Unmatched parentheses/brackets
  - Invalid statement structure

## 5. Indentation Handling
Python's significant whitespace is handled by:
- Tracking indentation levels
- Converting tabs to spaces (4-space equivalent)
- Validating consistent indentation
- Generating INDENT/DEDENT tokens

## 6. Limitations
Current implementation limitations:
- Limited data type inference
- No semantic analysis
- Basic error recovery
- Subset of Python syntax supported
- No optimization features

## 7. Usage Example
```python
def example(x, y):
    if x > y:
        return x
    else:
        return y

result = example(10, 20)
```

This code demonstrates:
- Function definition parsing
- Parameter handling
- Conditional statement parsing
- Indentation processing
- Expression evaluation structure
- Symbol table population
- Token tracking

## 8. Future Improvements
Potential enhancements:
- Full Python grammar support
- Type checking and inference
- Advanced error recovery
- AST generation
- Code optimization
- Import statement handling
- Class definition support 