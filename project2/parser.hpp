#ifndef PARSER_HPP
#define PARSER_HPP

#include "scanner.hpp"
#include <vector>
#include <string>
#include <set>
#include <stdexcept> 

class Parser {
public:
    Parser(const vector<Token>& tokens);
    bool parse();

private:
    const vector<Token>& tokens;
    int current = 0;
    set<string> declaredVariables;

    bool isAtEnd();
    Token advance();
    Token peek();
    Token previous();
    bool check(TokenType type);
    Token consume(TokenType type, const string& errorMessage);
    void synchronize();
    void validateIdentifier(const Token& token);
    bool match(TokenType type);
    void declaration();

    void program();
    void statement();
    void expression();
    void assignment();

    void error(const Token& token, const string& message);
};

#endif
