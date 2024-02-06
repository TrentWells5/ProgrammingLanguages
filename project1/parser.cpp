#include "parser.hpp"

Parser::Parser(const string& source) : scanner(source) {
    scan(); // Initialize the first token
}

void Parser::error(const string& message) {
    cerr << "Error: " << message << endl;
    assert(false); // Use C++'s assert for error handling
}

void Parser::scan() {
    tie(currentToken, currentValue) = scanner.nextToken();
}

void Parser::expect(const initializer_list<string>& symbolSet) {
    if (find(symbolSet.begin(), symbolSet.end(), currentToken) != symbolSet.end()) {
        if (currentToken != "eoI") scan();
    } else {
        error(currentToken + " expected, found " + currentToken);
    }
}

void Parser::parse() {
    try {
        Stmts();
        expect({"eoI"});
        cout << "Success!" << endl;
    } catch (...) {
        cout << "Aborted!" << endl;
    }
}

// Inside Parser.cpp

// Stmt = Assign | Cond | Loop
void Parser::Stmt() {
    if (currentToken == "identifier") {
        Assign();
    } else if (currentToken == "ifSym") {
        Cond();
    } else if (currentToken == "whileSym") {
        Loop();
    } else {
        error("Statement (id, if, while) expected");
    }
}

// Assign = LVal = AExpr
void Parser::Assign() {
    LVal();
    expect({"assignSym"});
    AExpr();
}

// Cond = if ( AExpr ) Stmt
void Parser::Cond() {
    scan(); // Move past 'ifSym'
    expect({"lParen"});
    AExpr();
    expect({"rParen"});
    Stmt();
}

// Loop = while ( AExpr ) Stmt
void Parser::Loop() {
    scan(); // Move past 'whileSym'
    expect({"lParen"});
    AExpr();
    expect({"rParen"});
    Stmt();
}

// LVal = id
void Parser::LVal() {
    expect({"identifier"});
}

// AExpr = Term (+ | - Term)*
void Parser::AExpr() {
    Term();
    while (currentToken == "plusSym" || currentToken == "minusSym") {
        scan(); // Move past the operator
        Term();
    }
}

// Term = Factor (* | / Factor)*
void Parser::Term() {
    Factor();
    while (currentToken == "timesSym" || currentToken == "divSym") {
        scan(); // Move past the operator
        Factor();
    }
}

// Factor = (AExpr) | id | num
void Parser::Factor() {
    if (currentToken == "lParen") {
        scan(); // Move past '('
        AExpr();
        expect({"rParen"}); // Ensure we have a closing ')'
    } else if (currentToken == "identifier" || currentToken == "numConstant") {
        scan(); // Accept the token and move on
    } else {
        error("Factor (id, num, or expression) expected");
    }
}


// Example implementation for Stmts
void Parser::Stmts() {
    Stmt();
    while (currentToken == "semicolon") {
        scan();
        Stmt();
    }
}

// The rest of the methods (Stmt, Assign, Cond, Loop, LVal, AExpr, Term, Factor) need to be implemented 
// following the logic provided in the Python version.
