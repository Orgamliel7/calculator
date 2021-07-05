#include "scanner.h"
#include "symtab.h"

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