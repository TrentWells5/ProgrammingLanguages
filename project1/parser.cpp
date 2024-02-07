#include "parser.hpp"

Parser::Parser(const vector<Token>& tokens) : tokens(tokens) {}

void Parser::parse() {
    try {
        program();
    } catch (const runtime_error& error) {
        cerr << error.what() << endl;
        // Handle parsing error (e.g., log it, halt execution, etc.)
    }
}

void Parser::program() {
    consume(TokenType::Begin, "Expected 'begin' at the start of the program.");
    while (!check(TokenType::End) && !isAtEnd()) {
        statement();
    }
    consume(TokenType::End, "Expected 'end' after statements.");
    consume(TokenType::Dot, "Expected '.' after 'end'");
}

void Parser::statement() {
    if (check(TokenType::Identifier)) {
        assignment();
    } else {
        error(peek(), "Expected statement.");
        synchronize();
    }
}

void Parser::assignment() {
    Token identifier = consume(TokenType::Identifier, "Expected identifier.");
    consume(TokenType::Assign, "Expected '=' after identifier.");
    expression();
    consume(TokenType::Semicolon, "Expected ';' after expression.");
}

void Parser::expression() {
    // Placeholder for expression parsing
    // This method would be much more complex in a full implementation,
    // handling arithmetic and logical expressions according to the language's grammar
    advance(); // Naively advance past the expression for now
}

// Utility methods for parsing

bool Parser::isAtEnd() {
    return peek().type == TokenType::Eof;
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

Token Parser::peek() {
    return tokens[current];
}

Token Parser::previous() {
    return tokens[current - 1];
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().type == type;
}

Token Parser::consume(TokenType type, const string& errorMessage) {
    if (check(type)) return advance();
    throw runtime_error(errorMessage);
}

void Parser::error(const Token& token, const string& message) {
    // Placeholder for error handling
    cerr << "Parse error at line " << token.line << ": " << message << endl;
    throw runtime_error(message);
}

void Parser::synchronize() {
    advance();
    while (!isAtEnd()) {
        if (previous().type == TokenType::Semicolon) return;

        switch (peek().type) {
            case TokenType::Begin:
            case TokenType::End:
                return;
            default:
                advance();
        }
    }
}
