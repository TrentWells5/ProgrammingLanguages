#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <string>
#include <unordered_set>
#include <cctype>
#include <iostream>
#include <unordered_map>
using namespace std;

class Scanner {
public:
    Scanner(const string& source);
    void init();
    pair<string, string> nextToken();
    static const string eoIToken;

private:
    string source;
    size_t position;
    string currentText;
    string currentToken;

    // Character classes
    static const char EOI;
    static const char START_COMMENT;
    static const char END_COMMENT;
    static const char START_STRING;
    static const char END_STRING;
    static const char EQUAL;
    static const char NOT;
    static const char GREATER;
    static const char LESS;

    static const unordered_set<char> WHITESPACE;
    static const unordered_set<char> DIGITS;
    static const unordered_set<char> LETTERS;
    static const unordered_set<char> LETTERS_OR_DIGITS;

    static const unordered_map<char, string> OP_TABLE;
    static const unordered_map<string, string> KEYWORD_TABLE;

    // Helper methods
    bool isDigit(char ch);
    void error(const string& message);
    char currentCh();
    void move();
    bool atEOI();
    void eat();
    string find(char x);
    string findStar(const unordered_set<char>& s);
    string skip(char x);
    string skipStar(const unordered_set<char>& s);
    void skipWS();
    void skipComment();
    void jump();
    void jumpStar();
    pair<string, string> NUM();
    pair<string, string> ID();
    pair<string, string> STRI();
    pair<string, string> twoCharSym(char secondCh, const string& firstToken, const string& secondToken);
    static unordered_set<char> initializeLetters();
    static unordered_set<char> initializeLettersOrDigits();
};

#endif