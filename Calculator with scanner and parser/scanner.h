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
    ELSE = 25,
    EXIT = 26,
    NOLINE = 27,
};

class Token
        {
public:
    char kind;        	    // מה סוג הטוקן
    int value;     	    // לתת ערך מספרי למשתני ספרות
    string name;     	    // למשתני שם
    Token(char ch) :kind(ch), value(0) { }
    Token(char ch, int val) :kind(ch), value(val) { }
    Token(char ch, string n) :kind{ch}, name{n} { }
};

class Token_stream
        {
public:
    Token_stream() :full(false), buffer(0) {}
    Token get();            // get a Token
    void putback(Token t);  // put a Token back

private:
    bool full;              // האם יש טוקן בבאפר אם לאו
    Token buffer;           // שמור על טוקן ע"י המתודה putback()
};

// הצהרות לתקשורת חוצה פונקציות
int expression();
int term();
int primary();
int statement();
int declaration();


// אופרציות לוגיות
int logicialOr();
int logicialAnd();
int logiciakBiggerOrSmaller();
