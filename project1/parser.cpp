#include "parser.hpp"

Parser::Parser(const vector<Token>& tokens) : tokens(tokens) {}

bool Parser::parse() {
    try {
        program();
        // If no exceptions were thrown, parsing is considered successful.
        return true;
    } catch (const runtime_error& error) {
        cerr << error.what() << endl;
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
    consume(TokenType::Identifier, "Expected identifier.");
    consume(TokenType::Assign, "Expected '=' after identifier.");
    expression(); // This should process through the expression and stop before the semicolon.
    
    // After 'expression()' returns, we're expecting a semicolon as the next token.
    if (check(TokenType::Semicolon)) {
        advance();  // Consume the semicolon
    } else {
        error(peek(), "Expected ';' after expression.");
    }
}



void Parser::expression() {
    int parenDepth = 0;
    while (!isAtEnd()) {
        if (check(TokenType::LeftParen)) {
            parenDepth++;
            advance(); // Consume the '('
        } else if (check(TokenType::RightParen)) {
            if (parenDepth == 0) {
                // If there are no open parentheses to match, this is an error or the end of the current expression.
                break;
            }
            parenDepth--;
            advance(); // Consume the ')'
        } else if (check(TokenType::Semicolon) && parenDepth == 0) {
            // Only consider a semicolon as ending the expression if not within parentheses.
            break;
        } else {
            advance(); // Advance through other tokens within the expression.
        }
    }

    if (parenDepth != 0) {
        error(peek(), "Unmatched parentheses in expression.");
    }
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
