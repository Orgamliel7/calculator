
#include<iostream>
#include<string>
#include<map>
#include<algorithm>
#include <regex>
#include<stdexcept>

using namespace std;

enum Tokens
        {
    NUM = 20,
    ID = 21,
    INT = 22,
    EOL = 23,
    IF = 24,
    ELSE = 25
};

class Token{
public:
    char kind;
    int value;
    string name;


    explicit Token(char ch) : Token(ch, 0, "") { }
    Token(char ch, int val) : Token(ch, val, "") { }
    Token(char ch, string n) : Token(ch, 0, move(n)) { }
    Token(char ch, int val, string n) : kind(ch), value(val), name(move(n)) {}
};

class Token_stream {
    bool full;
    Token buffer;
public:

    Token_stream() : full(false), buffer(0) {}
    Token get();
    void putback(Token t);
};

int statement();
int declaration();
int condition();
int expression();
int term();
int primary();

int logicialOr();
int logicialAnd();
int logiciakBiggerOrSmaller();
