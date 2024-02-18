#include "parser.hpp"

Parser::Parser(const vector<Token>& tokens) : tokens(tokens) {}

bool Parser::parse() {
    try {
        program();
        return true;
    } catch (const runtime_error& error) {
        // Return false to indicate parsing was unsuccessful.
        return false;
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
    Token identifierToken = consume(TokenType::Identifier, "Expected identifier.");
    validateIdentifier(identifierToken); // Call to validate the identifier
    consume(TokenType::Assign, "Expected '=' after identifier.");
    expression();

    if (check(TokenType::Semicolon)) {
        advance();
    } else {
        error(peek(), "Expected ';' after expression.");
    }
}


void Parser::validateIdentifier(const Token& token) {
    // Assuming token.value contains the actual text of the identifier
    if (!token.value.empty() && token.value.back() == '_') {
        error(token, "Identifier cannot end with an underscore.");
    }

    // Check for consecutive underscores within the identifier
    if (token.value.find("__") != std::string::npos) {
        error(token, "Identifier cannot contain consecutive underscores.");
    }

    // Here, token.value is used in place of what was previously referred to as token.lexeme
    // Additional identifier validation rules can be implemented here as needed
}



void Parser::expression() {
    int parenDepth = 0;
    while (!isAtEnd()) {
        if (check(TokenType::LeftParen)) {
            parenDepth++;
            advance();
        } else if (check(TokenType::RightParen)) {
            if (parenDepth == 0) {
                break;
            }
            parenDepth--;
            advance();
        } else if (check(TokenType::Semicolon) && parenDepth == 0) {
            break;
        } else {
            advance();
        }
    }

    if (parenDepth != 0) {
        error(peek(), "Unmatched parentheses in expression.");
    }
}

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
    Token errToken = peek();
    string fullErrorMessage = errorMessage + " at line " + to_string(errToken.line);
    error(errToken, fullErrorMessage);
    throw runtime_error(fullErrorMessage);
}

void Parser::error(const Token& token, const string& message) {
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