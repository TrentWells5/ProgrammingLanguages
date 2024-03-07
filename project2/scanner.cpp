#include "scanner.hpp"

Scanner::Scanner(const string& source) : source(source) {}

bool Scanner::isAtEnd() {
    return current >= source.length();
}

void Scanner::addToken(TokenType type) {
    addToken(type, "");
}

string Scanner::tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::Identifier: return "Identifier";
        case TokenType::Number: return "Number";
        case TokenType::Assign: return "Assign";
        case TokenType::Semicolon: return "Semicolon";
        case TokenType::Plus: return "Plus";
        case TokenType::Minus: return "Minus";
        case TokenType::Multiply: return "Multiply";
        case TokenType::Divide: return "Divide";
        case TokenType::LeftParen: return "LeftParen";
        case TokenType::RightParen: return "RightParen";
        case TokenType::Begin: return "Begin";
        case TokenType::End: return "End";
        case TokenType::Dot: return "Dot";
        case TokenType::Eof: return "Eof";
        case TokenType::Unknown: return "Unknown";
        default: return "NotImplemented";
    }
}


void Scanner::addToken(TokenType type, string value) {
    if (value.empty()) {
        value = "N/A";
    }
    Token token = {type, value, line};
    tokens.push_back(token);
}


char Scanner::advance() {
    current++;
    return source[current - 1];
}

bool Scanner::match(char expected) {
    if (isAtEnd()) return false;
    if (source[current] != expected) return false;

    current++;
    return true;
}

char Scanner::peek() {
    if (isAtEnd()) return '\0';
    return source[current];
}

char Scanner::peekNext() {
    if (current + 1 >= source.length()) return '\0';
    return source[current + 1];
}

void Scanner::scanToken() {
    char c = advance();
    switch (c) {
        case '(': addToken(TokenType::LeftParen); break;
        case ')': addToken(TokenType::RightParen); break;
        case ';': addToken(TokenType::Semicolon); break;
        case '.': addToken(TokenType::Dot); break;
        case '=': addToken(TokenType::Assign); break;
        case '+': addToken(TokenType::Plus); break;
        case '-': addToken(TokenType::Minus); break;
        case '*': addToken(TokenType::Multiply); break;
        case '/': addToken(TokenType::Divide); break;
        case ',': addToken(TokenType::Comma); break;
        case '~': comment(); break;
        default:
            if (isDigit(c)) {
                number();
            } else if (isAlpha(c)) {
                identifier();
            } else {
                cerr << "Unexpected character: '" << c << "' at line " << line << endl; 
            }
            break;
    }
}

void Scanner::number() {
    while (isDigit(peek())) advance();
    addToken(TokenType::Number, source.substr(start, current - start));
}


void Scanner::identifier() {
    while (isAlphaNumeric(peek()) || peek() == '_') {
        advance();
    }
    string text = source.substr(start, current - start);

    TokenType type = TokenType::Identifier;
    if (text == "begin") type = TokenType::Begin;
    else if (text == "end") type = TokenType::End;
    else if (text == "var") type = TokenType::Var;
    addToken(type, text);
}




void Scanner::comment() {
    while (peek() != '\n' && !isAtEnd()) advance();
}

void Scanner::skipWhitespace() {
    while (true) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                line++;
                advance();
                break;
            default:
                return;
        }
    }
}

bool Scanner::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Scanner::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

bool Scanner::isDigit(char c) {
    return c >= '0' && c <= '9';
}

vector<Token> Scanner::scanTokens() {
    while (!isAtEnd()) {
        start = current;
        scanToken();
        skipWhitespace();
    }

    tokens.push_back({TokenType::Eof, "N/A", line});
    return tokens;
}