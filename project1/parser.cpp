#include "parser.hpp"

Parser::Parser(const vector<Token>& tokens) : tokens(tokens) {}

void Parser::parse() {
    try {
        program();
    } catch (const runtime_error& error) {
        //cerr << error.what() << endl;
        // Handle parsing error (e.g., log it, halt execution, etc.)
    }
}

void Parser::program() {
    consume(TokenType::Begin, "Expected 'begin' at the start of the program.");
    while (!check(TokenType::End) && !isAtEnd()) {
        statement();
    }
    consume(TokenType::End, "Expected 'end' after statements.");
    // No need to consume a semicolon after 'end.'
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
    // Temporary, simplistic expression parsing
    do {
        advance();  // Advance past the current token
        // Check if the next token is an operator that could be part of an expression
    } while (!isAtEnd() && !check(TokenType::Semicolon) && !check(TokenType::RightParen));
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
    Token errToken = peek(); // Get the current token where the error occurred
    string fullErrorMessage = errorMessage + " at line " + to_string(errToken.line);
    error(errToken, fullErrorMessage); // Use the error function to report the error
    throw runtime_error(fullErrorMessage); // Optionally re-throw if you want to halt parsing
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
