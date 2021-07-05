#include "scanner.h"
#include "symtab.h"
#include<iostream>
#include<string>
#include<map>
#include<algorithm>
#include <iomanip>

// returns a number from 0 up to, but excluding x
const int getrandom0 (const int x)
{
    if (x <= 0)
        return 0;

    // r will be between 0 and 1 (but below 1 as we are dividing by RAND_MAX+1)
    double r = static_cast<double> (std::rand () % RAND_MAX) / (static_cast<double> (RAND_MAX) + 1.0);
    return (r * x);

}   // end of getrandom

const int roll (const int howmany, const int die)
{
    int count;
    int total = 0;

    for (count = 0; count < howmany; ++count)
        total += getrandom0 (count) + 1;

    return total;

} // end of roll


// returns true if a x% probability exists
// eg. percent (80) will be true 80% of the time
const bool percent0 (const int prob)
{
    if (prob <= 0)
        return false;
    if (prob >= 100)
        return true;

    return getrandom0 (100) > (100 - prob);

}

static int initRandom0 ()
{
    srand   (time (NULL));
#ifndef WIN32
    srand48 (time (NULL));
#endif
    return 0;
}



//////////////  empleado


int m; //Total number of rows
int n; //Total number columns

double w; //The size of each tile in inches
double h; //The size of each tile in inches

int x; // X location on the board
int y; // Y location on the board

double vx; // Travel speed of moter x
double vy; // Travel speed of moter y

double distanceX(int xs){
    double distance;
    distance = abs(xs - x) * w;
    return distance;
}


double distanceY(int ys){
    double distance;
    distance = abs(ys - y) * h;
    return distance;
}

// This going to calculate the cost of the node
// Dont need this but im going to keep it
double nodeDistance(int xs, int ys){
    double cost = 0;

    cost = (distanceX(xs) * distanceX(xs)) + (distanceY(ys)* distanceY(ys));

    return cost;
}

double maxVelocity(int xs, int ys){
    double tx = distanceX(xs) / vx;
    double ty =  distanceY(ys) / vy;
    return max(tx, ty);
}



























// גלובלים
Token_stream ts;
Symbol_table st;

int statement()
{
    Token t = ts.get();
    switch (t.kind) {
        case INT:
            return declaration();
        case IF:
            return condition();
        default:
            ts.putback(t);
            return logicialOr();
    }
}

int declaration()
{
    Token t = ts.get();
    if (t.kind != ID) throw runtime_error("name expected in declaration");
    string name = t.name;
    st.declare(name, 0);
    return 0;
}

int condition()
{
    int stipulation = logicialOr(); //Read condition value
    int do_statement = logicialOr();
    Token t = ts.get();
    int do_not_statement = logicialOr();

    if(t.kind != ELSE){
        throw runtime_error("if without else");
    }
    else if(stipulation != 0){ //i.e. condition is true
        return do_statement;
    }
    else{
        return do_not_statement;
    }
}

// + and -
int expression()
{
    int left = term();
    Token t = ts.get();

    while (true) {
        switch (t.kind) {
            case '+':
                left += term();
                t = ts.get();
                break;
            case '-':
                left -= term();
                t = ts.get();
                break;
            default:
                ts.putback(t);
                return left;
        }
    }
}

// * and /
int term()
{
    int left = primary();
    Token t = ts.get();

    while (true)
    {
        switch (t.kind)
        {
            case '*':
                left *= primary();
                t = ts.get();
                break;
            case '/':
            {
                int d = primary();
                if (d == 0) throw runtime_error("divide by zero");
                left /= d;
                t = ts.get();
                break;
            }
            default:
                ts.putback(t);
                return left;
        }
    }
}

int primary()
{
    Token t = ts.get();

    switch (t.kind) {
        case '(':    // handle '(' expression ')'
        {
            int d = logicialOr();
            t = ts.get();
            if (t.kind != ')'){
                throw runtime_error("')' expected");
            }else{
                return d;
            }
        }
        case '-':
            return - primary();
        case '+':
            return primary();
        case '!':
            return !primary();  //! has higher precedence then * and /
        case NUM:
            return t.value;  // return the number value
        case ID:
        {
            string n = t.name;
            Token next = ts.get();
            if (next.kind == '=') {	// name = expression
                int d = logicialOr();
                st.set(n, d);
                return d; // return the assignment value
            }
            else {
                ts.putback(next);		// not an assignment
                return st.get(t.name).value;  // return the id value
            }
        }
        default:
            throw runtime_error("primary expected");
    }
}

int logicialOr()
{
    //לוגם עדיפות גבוהה מאו
    int left = logicialAnd();
    Token t = ts.get();

    while(true) {
        if (t.kind == '|') {
            int right = logicialAnd();
            left = (left || right);
            t = ts.get();
        } else {
            ts.putback(t);
            return left;
        }
    }

}
int logicialAnd()
{
    int left = logiciakBiggerOrSmaller();
    Token t = ts.get();

    while(true) {
        if (t.kind == '&') {
            int right = logiciakBiggerOrSmaller();
            left = (left && right);
            t = ts.get();
        } else {
            ts.putback(t);
            return left;
        }
    }
}
int logiciakBiggerOrSmaller()
{
    int left = expression();
    Token t = ts.get();

    while (true) {
        switch(t.kind){
            case '<':
                left = (left < expression());
                t = ts.get();
                break;
            case '>':
                left = (left > expression());
                t = ts.get();
                break;
            default:
                ts.putback(t);
                return left;
        }
    }
}