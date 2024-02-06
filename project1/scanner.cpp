#include "scanner.hpp"

// Static member definitions
const char Scanner::START_COMMENT = '~';
const char Scanner::EQUAL = '=';

const unordered_set<char> Scanner::WHITESPACE = {' ', '\t', '\n'};
const unordered_set<char> Scanner::DIGITS = {'0','1','2','3','4','5','6','7','8','9'};
const unordered_set<char> Scanner::LETTERS = Scanner::initializeLetters();
const unordered_set<char> Scanner::LETTERS_OR_DIGITS = Scanner::initializeLettersOrDigits();

const unordered_map<char, string> Scanner::OP_TABLE = {
    {'(', "lParen"},
    {')', "rParen"}, 
    {'+', "plusSym"}, 
    {'-', "minusSym"},
    {'*', "timesSym"}, 
    {'/', "divSym"},
    {';', "semicolon"}, 
};

const unordered_map<string, string> Scanner::KEYWORD_TABLE = {
    {"begin", "beginSym"},
    {"end.", "endSym"},
};

unordered_set<char> Scanner::initializeLetters() {
    unordered_set<char> letters;
    for (char ch = 'a'; ch <= 'z'; ++ch) {
        letters.insert(ch);
    }
    for (char ch = 'A'; ch <= 'Z'; ++ch) {
        letters.insert(ch);
    }
    return letters;
}

unordered_set<char> Scanner::initializeLettersOrDigits() {
    unordered_set<char> lettersOrDigits = Scanner::initializeLetters();
    for (char ch = '0'; ch <= '9'; ++ch) {
        lettersOrDigits.insert(ch);
    }
    return lettersOrDigits;
}

Scanner::Scanner(const string& source) : source(source + EOI), position(0) {
    init(); // Assuming init() sets up the initial state for scanning
}

void Scanner::init() {
    position = 0;
    currentText = "";
    currentToken = "";
}

char Scanner::currentCh() {
    if (position < source.length()) {
        return source[position];
    } else {
        return EOI;
    }
}

// move
void Scanner::move() {
    if (position < source.length()) {
        position++;
    } else {
        error("Attempt to move beyond end of input");
    }
}


void Scanner::error(const string& message) {
    cout << ">>> Error: " << message << endl;
}

bool Scanner::atEOI() {
    return currentCh() == EOI;
}

void Scanner::eat() {
    if (atEOI()) {
        error("Cannot move beyond EOI!");
    } else {
        move();
    }
}

string Scanner::find(char x) {
    string result;
    while (currentCh() != x && !atEOI()) {
        result += currentCh();
        eat();
    }
    if (atEOI()) {
        error("EOI detected searching for " + string(1, x));
    }
    return result;
}

string Scanner::findStar(const unordered_set<char>& s) {
    string result;
    while (s.find(currentCh()) == s.end() && !atEOI()) {
        result += currentCh();
        eat();
    }
    if (atEOI()) {
        error("EOI detected searching for specified set");
    }
    return result;
}

string Scanner::skip(char x) {
    string result;
    while (currentCh() == x) {
        result += currentCh();
        eat();
    }
    return result;
}

string Scanner::skipStar(const unordered_set<char>& s) {
    string result;
    while (s.find(currentCh()) != s.end()) {
        result += currentCh();
        eat();
    }
    return result;
}

void Scanner::skipWS() {
    skipStar(WHITESPACE);
}

void Scanner::skipComment() {
    eat(); // Move past the start comment character
    while (currentCh() != END_COMMENT && !atEOI()) {
        eat();
    }
    if (!atEOI()) {
        eat(); // Move past the end comment character
    } else {
        error("Unterminated comment");
    }
}

void Scanner::jump() {
    if (currentCh() == START_COMMENT) {
        skipComment();
    } else if (WHITESPACE.find(currentCh()) != WHITESPACE.end()) {
        skipWS();
    }
}

void Scanner::jumpStar() {
    while (WHITESPACE.find(currentCh()) != WHITESPACE.end() || currentCh() == START_COMMENT) {
        jump();
    }
}

pair<string, string> Scanner::NUM() {
    string numLiteral;
    while (isdigit(currentCh())) {
        numLiteral += currentCh();
        eat();
    }
    return {"numConstant", numLiteral}; // Keep as string
}


pair<string, string> Scanner::ID() {
    string result = skipStar(LETTERS_OR_DIGITS);
    auto found = KEYWORD_TABLE.find(result);
    if (found != KEYWORD_TABLE.end()) {
        return {found->second, ""};
    } else {
        return {"identifier", result};
    }
}

pair<string, string> Scanner::STRI() {
    eat(); // Skip initial START_STRING
    string result = find(END_STRING);
    eat(); // Skip END_STRING
    return {"stringConstant", result};
}

pair<string, string> Scanner::twoCharSym(char secondCh, const string& firstToken, const string& secondToken) {
    eat(); // Move past the first character
    if (currentCh() == secondCh) {
        eat(); // Move past the second character
        return {secondToken, ""};
    } else {
        return {firstToken, ""};
    }
}

pair<string, string> Scanner::nextToken() {
    if (atEOI()) {
        return {eoIToken, ""}; // End of Input token
    }

    jumpStar(); // Skip whitespaces or comments

    char c = currentCh();

    // Handle numeric constants
    if (DIGITS.find(c) != DIGITS.end()) {
        return NUM();
    }

    // Handle string literals
    if (c == START_STRING) {
        return STRI();
    }

    // Handle identifiers and keywords
    if (LETTERS.find(c) != LETTERS.end()) {
        auto [tokenType, lexeme] = ID();
        if (KEYWORD_TABLE.find(lexeme) != KEYWORD_TABLE.end()) {
            return {KEYWORD_TABLE.at(lexeme), ""};
        }
        return {tokenType, lexeme};
    }

    // Handle two-character symbols
    switch (c) {
        case EQUAL: return twoCharSym(EQUAL, "assignSym", "equalSym");
        case NOT: return twoCharSym(EQUAL, "notSym", "notEqualSym");
        case GREATER: return twoCharSym(EQUAL, "greaterSym", "greaterEQSym");
        case LESS: return twoCharSym(EQUAL, "lessSym", "lessEQSym");
        default: break;
    }

    // Handle one-character operators
    if (OP_TABLE.find(c) != OP_TABLE.end()) {
        eat(); // Consume the operator
        return {OP_TABLE.at(c), ""};
    }

    // Unknown token or character
    eat(); // Move past unknown character
    return {"Unknown", string(1, c)};
}