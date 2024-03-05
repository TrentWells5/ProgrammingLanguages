#ifndef PARSER_HPP
#define PARSER_HPP

#include "scanner.hpp"
#include <vector>
#include <string>
#include <set>
#include <fstream>
#include <stdexcept> 

struct RPNInstruction {
    string operation;
    string operand;
    RPNInstruction(const std::string& op, const std::string& opnd) : operation(op), operand(opnd) {}
};

class Parser {
public:
    Parser(const vector<Token>& tokens);
    bool parse();
    void outputRPNInstructions(const std::string& filename);

private:
    const vector<Token>& tokens;
    int current = 0;
    set<string> declaredVariables;
    vector<RPNInstruction> rpnInstructions; // Vector to store RPN instructions

    // Utility methods for parsing
    bool isAtEnd();
    Token advance();
    Token peek();
    Token previous();
    bool check(TokenType type);
    Token consume(TokenType type, const string& errorMessage);
    void synchronize();
    void validateIdentifier(const Token& token);
    bool match(TokenType type);
    
    // Declaration and statement parsing methods
    void declaration();

    // Core program structure methods
    void program();
    void statement();

    // Expression parsing methods
    void expression();
    void term();
    void factor();
    void assignment();

    // RPN Generation helper
    void addRPNInstruction(const std::string& operation, const std::string& operand = "");

    // Error handling
    void error(const Token& token, const string& message);
};

#endif
