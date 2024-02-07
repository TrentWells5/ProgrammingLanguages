#include "scanner.hpp"

// Static member definitions
const char Scanner::EOI = '$';
const char Scanner::START_COMMENT = '~';
const char Scanner::EQUAL = '=';
const string Scanner::eoIToken = "eoI";

const unordered_set<char> Scanner::WHITESPACE = {' ', '\t', '\n'};
const unordered_set<char> Scanner::DIGITS = {'0','1','2','3','4','5','6','7','8','9'};
const unordered_set<char> Scanner::LETTERS = Scanner::initializeLetters();
const unordered_set<char> Scanner::LETTERS_OR_DIGITS = Scanner::initializeLettersOrDigits();

const unordered_map<char, string> Scanner::OP_TABLE = {
    {'(', "lParen"},
    {')', "rParen"}, 
    {'{', "lCurly"}, 
    {'}', "rCurly"},
    {'+', "plusSym"}, 
    {'-', "minusSym"},
    {'*', "timesSym"}, 
    {'/', "divSym"},
    {';', "semicolon"}, 
    {',', "comma"}
};

const unordered_map<string, string> Scanner::KEYWORD_TABLE = {
    {"begin", "beginSym"}, 
    {"end", "endSym"}, 
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

bool Scanner::isLetter(char ch) {
    return LETTERS.find(ch) != LETTERS.end();
}

bool Scanner::isDigit(char ch) {
    return DIGITS.find(ch) != DIGITS.end();
}

bool Scanner::isUnderscore(char ch) {
    return ch == '_';
}

bool Scanner::isValidIdentifierChar(char ch) {
    return LETTERS_OR_DIGITS.find(ch) != LETTERS_OR_DIGITS.end() || isUnderscore(ch);
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
    while (currentCh() != '\n' && !atEOI()) {
        eat();
    }
    if (!atEOI()) {
        eat(); // Move past the end comment character
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
    while (isDigit(currentCh())) {
        numLiteral += currentCh();
        eat();
    }
    return {"numConstant", numLiteral}; // Keep as string
}


pair<string, string> Scanner::ID() {
    // string result = skipStar(LETTERS_OR_DIGITS);
    // auto found = KEYWORD_TABLE.find(result);
    // if (found != KEYWORD_TABLE.end()) {
    //     return {found->second, ""};
    // } else {
    //     return {"identifier", result};
    // }
    string result;
    char prevChar = '\0';

    // Ensure the first character is a letter
    if (isLetter(currentCh())) {
        result += currentCh();
        prevChar = currentCh();
        eat();
    } else {
        // Handle error for identifiers not starting with a letter
        return {"error", "Identifier does not start with a letter"};
    }

    // Process subsequent characters
    while (isValidIdentifierChar(currentCh())) {
        if (isUnderscore(currentCh()) && isUnderscore(prevChar)) {
            // Handle consecutive underscores error
            return {"error", "Identifier contains consecutive underscores"};
        }
        result += currentCh();
        prevChar = currentCh();
        eat();
    }

    // Check if the identifier ends with an underscore
    if (isUnderscore(prevChar)) {
        return {"error", "Identifier cannot end with an underscore"};
    }

    // Check if the result is a keyword
    auto found = KEYWORD_TABLE.find(result);
    if (found != KEYWORD_TABLE.end()) {
        return {found->second, ""};
    } else {
        return {"identifier", result};
    }
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