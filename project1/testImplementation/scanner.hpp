#ifndef SCANNER_HPP
#define SCANNER_HPP
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <string>

enum class TokenType {
    BEGIN, END, IDENTIFIER, NUMBER, ASSIGN, SEMICOLON, LPAREN, RPAREN, PLUS, MINUS, MULTIPLY, DIVIDE, COMMENT, END_OF_FILE, INVALID
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
};

class Scanner {
private:
    std::ifstream inputStream;
    std::unordered_set<std::string> symbolTable;
    int line = 1;

    char peekChar() {
        return inputStream.peek();
    }

    char getChar() {
        char c = inputStream.get();
        if (c == '\n') ++line;
        return c;
    }

    bool isAlpha(char c) {
        return std::isalpha(static_cast<unsigned char>(c));
    }

    bool isDigit(char c) {
        return std::isdigit(static_cast<unsigned char>(c));
    }

    void skipWhitespaceAndComments() {
        char c;
        while (isspace(peekChar()) || peekChar() == '~') {
            if (peekChar() == '~') {
                while (peekChar() != '\n' && !inputStream.eof()) getChar();
            } else {
                getChar();
            }
        }
    }

public:
    Scanner(const std::string& filename) : inputStream(filename) {
        if (!inputStream) {
            throw std::runtime_error("File not found: " + filename);
        }
    }

    Token getNextToken() {
        skipWhitespaceAndComments();
        if (inputStream.eof()) return {TokenType::END_OF_FILE, "", line};

        char c = getChar();

        if (isAlpha(c)) {
            std::string lexeme(1, c);
            while (isAlpha(peekChar()) || isDigit(peekChar()) || peekChar() == '_') {
                c = getChar();
                if (c == '_' && (peekChar() == '_' || !isAlpha(peekChar()) && !isDigit(peekChar()))) {
                    return {TokenType::INVALID, lexeme, line};
                }
                lexeme += c;
            }
            if (lexeme == "begin") return {TokenType::BEGIN, lexeme, line};
            if (lexeme == "end") return {TokenType::END, lexeme, line};
            symbolTable.insert(lexeme);
            return {TokenType::IDENTIFIER, lexeme, line};
        }

        if (isDigit(c)) {
            std::string lexeme(1, c);
            while (isDigit(peekChar())) {
                lexeme += getChar();
            }
            return {TokenType::NUMBER, lexeme, line};
        }

        switch (c) {
            case '=': return {TokenType::ASSIGN, std::string(1, c), line};
            case ';': return {TokenType::SEMICOLON, std::string(1, c), line};
            case '(': return {TokenType::LPAREN, std::string(1, c), line};
            case ')': return {TokenType::RPAREN, std::string(1, c), line};
            case '+': return {TokenType::PLUS, std::string(1, c), line};
            case '-': return {TokenType::MINUS, std::string(1, c), line};
            case '*': return {TokenType::MULTIPLY, std::string(1, c), line};
            case '/': return {TokenType::DIVIDE, std::string(1, c), line};
            default: return {TokenType::INVALID, std::string(1, c), line};
        }
    }

    void printSymbolTable() {
        std::cout << "Symbol Table:" << std::endl;
        for (const auto& symbol : symbolTable) {
            std::cout << symbol << std::endl;
        }
    }

    ~Scanner() {
        if (inputStream.is_open()) {
            inputStream.close();
        }
    }
};
#endif