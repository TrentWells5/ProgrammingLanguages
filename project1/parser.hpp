#ifndef PARSER_HPP
#define PARSER_HPP

#include "scanner.hpp" // Adjust the include path according to your project structure
#include <string>
#include <iostream>
#include <cassert>
using namespace std;

class Parser {
public:
    Parser(const string& source);
    void parse();

private:
    Scanner scanner;
    string currentToken;
    string currentValue;

    void error(const string& message);
    void scan();
    void expect(const initializer_list<string>& symbolSet);

    // Grammar rules
    void Stmts();
    void Stmt();
    void Assign();
    void Cond();
    void Loop();
    void LVal();
    void AExpr();
    void Term();
    void Factor();
};

#endif
