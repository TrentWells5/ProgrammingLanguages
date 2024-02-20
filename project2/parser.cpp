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
        if (match(TokenType::Var)) {
            declaration();
        } else {
            statement();
        }
    }
    consume(TokenType::End, "Expected 'end' after statements.");
    consume(TokenType::Dot, "Expected '.' after 'end'");
}

void Parser::declaration() {
    do {
        Token varName = consume(TokenType::Identifier, "Expected variable name.");
        if (declaredVariables.find(varName.value) != declaredVariables.end()) {
            error(varName, "Illegal redefinition " + varName.value);
        }
        declaredVariables.insert(varName.value);
        // Optionally handle initialization or just move to the next variable/semicolon
    } while (match(TokenType::Comma)); // Keep looking for more variables separated by commas
    consume(TokenType::Semicolon, "Expected ';' after variable declaration.");
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
    if (!token.value.empty() && token.value.back() == '_') {
        error(token, "Identifier cannot end with an underscore.");
    }
    if (token.value.find("__") != std::string::npos) {
        error(token, "Identifier cannot contain consecutive underscores.");
    }
    if (declaredVariables.find(token.value) == declaredVariables.end()) {
        error(token, "Undefined variable " + token.value);
    }
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
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
    string fullErrorMessage = errorMessage;
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