#ifndef PARSER_HPP
#define PARSER_HPP
#include "scanner.hpp"
#include <stdexcept>
#include <iostream>

class Parser {
private:
    Scanner scanner;
    Token currentToken;

    void advance() {
        currentToken = scanner.getNextToken();
    }

    void eat(TokenType type) {
        if (currentToken.type == type) {
            advance();
        } else {
            throw std::runtime_error("Syntax error on line " + std::to_string(currentToken.line) + ": unexpected token '" + currentToken.lexeme + "'");
        }
    }

public:
    Parser(const std::string& filename) : scanner(filename) {
        advance(); // Initialize the first token
    }

    void parseProgram() {
        eat(TokenType::BEGIN);
        while (currentToken.type != TokenType::END && currentToken.type != TokenType::END_OF_FILE) {
            parseStatement();
        }
        eat(TokenType::END);
        eat(TokenType::SEMICOLON); // Assuming semicolon here for simplicity; adjust as needed for a period
        scanner.printSymbolTable();
    }

    void parseStatement() {
        // Assuming only assignment statements for simplification
        if (currentToken.type == TokenType::IDENTIFIER) {
            advance(); // Identifier
            eat(TokenType::ASSIGN);
            parseExpression();
            eat(TokenType::SEMICOLON);
        } else {
            throw std::runtime_error("Syntax error on line " + std::to_string(currentToken.line) + ": expected identifier");
        }
    }

    void parseExpression() {
        // Simplified expression parsing; real implementation should handle arithmetic expressions
        if (currentToken.type == TokenType::NUMBER || currentToken.type == TokenType::IDENTIFIER) {
            advance(); // Consume the NUMBER or IDENTIFIER
            // For a full implementation, you'd handle operators and multiple operands here
        } else {
            throw std::runtime_error("Syntax error on line " + std::to_string(currentToken.line) + ": expected expression");
        }
    }
};
#endif
