#include "parser.hpp"

Parser::Parser(const string& source) : scanner(source) {
    scan(); // Initialize the first token
}

void Parser::error(const string& message) {
    cerr << "Error: " << message << endl;
    assert(false);
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

void Parser::Assign() {
    LVal();
    expect({"assignSym"});
    AExpr();
}

void Parser::Cond() {
    scan(); 
    expect({"lParen"});
    AExpr();
    expect({"rParen"});
    Stmt();
}

void Parser::Loop() {
    scan(); 
    expect({"lParen"});
    AExpr();
    expect({"rParen"});
    Stmt();
}

void Parser::LVal() {
    expect({"identifier"});
}

void Parser::AExpr() {
    Term();
    while (currentToken == "plusSym" || currentToken == "minusSym") {
        scan(); 
        Term();
    }
}

void Parser::Term() {
    Factor();
    while (currentToken == "timesSym" || currentToken == "divSym") {
        scan(); 
        Factor();
    }
}

void Parser::Factor() {
    if (currentToken == "lParen") {
        scan(); 
        AExpr();
        expect({"rParen"});
    } else if (currentToken == "identifier" || currentToken == "numConstant") {
        scan(); 
    } else {
        error("Factor (id, num, or expression) expected");
    }
}

void Parser::Stmts() {
    Stmt();
    while (currentToken == "semicolon") {
        scan();
        Stmt();
    }
}