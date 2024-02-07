#ifndef PARSER_HPP
#define PARSER_HPP

#include "scanner.hpp" // Include the scanner header for Token and TokenType
#include <vector>
#include <string>
#include <stdexcept> // For std::runtime_error

class Parser {
public:
    explicit Parser(const vector<Token>& tokens);
    void parse(); // Main method to start parsing

private:
    const vector<Token>& tokens;
    int current = 0;

    bool isAtEnd();
    Token advance();
    Token peek();
    Token previous();
    bool check(TokenType type);
    Token consume(TokenType type, const string& errorMessage);
    void synchronize();

    // Parsing methods
    void program();
    void statement();
    void expression();
    void assignment();

    // Utility methods
    void error(const Token& token, const string& message);
};

#endif // PARSER_HPP
