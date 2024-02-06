#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <vector>
#include <string>
#include <cctype>
#include <iostream>

using namespace std;


enum class TokenType {
    Begin, End, Identifier, Number, Assign, Semicolon, Plus, Minus, Multiply, Divide, LeftParen, RightParen, Dot, Comment, Unknown, Eof
};

struct Token {
    TokenType type;
    string value;
    int line;
};

class Scanner {
public:
    Scanner(const string& source);
    vector<Token> scanTokens();
    string tokenTypeToString(TokenType type);

private:
    string source;
    vector<Token> tokens;
    unsigned int start = 0;
    unsigned int current = 0;
    int line = 1;

    void scanToken();
    bool isAtEnd();
    void addToken(TokenType type);
    void addToken(TokenType type, string value);
    char advance();
    bool match(char expected);
    void number();
    void identifier();
    void skipWhitespace();
    void comment();
    char peek();
    char peekNext();
    bool isAlpha(char c);
    bool isAlphaNumeric(char c);
    bool isDigit(char c);
    
};

#endif
