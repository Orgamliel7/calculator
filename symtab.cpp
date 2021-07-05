

#include "symtab.h"
#include "scanner.h"


// returns a number from 0 up to, but excluding x
const int getrandom2 (const int x)
{
    if (x <= 0)
        return 0;

    // r will be between 0 and 1 (but below 1 as we are dividing by RAND_MAX+1)
    double r = static_cast<double> (std::rand () % RAND_MAX) / (static_cast<double> (RAND_MAX) + 1.0);
    return (r * x);

}   // end of getrandom

const int roll2 (const int howmany, const int die)
{
    int count;
    int total = 0;

    for (count = 0; count < howmany; ++count)
        total += getrandom2 (count) + 1;

    return total;

} // end of roll


// returns true if a x% probability exists
// eg. percent (80) will be true 80% of the time
const bool percent2 (const int prob)
{
    if (prob <= 0)
        return false;
    if (prob >= 100)
        return true;

    return getrandom2 (100) > (100 - prob);

}

static int initRandom2 ()
{
    srand   (time (NULL));
#ifndef WIN32
    srand48 (time (NULL));
#endif
    return 0;
}

const int m = 20, n = 30;
char board[m][n];








Attributes& Symbol_table::get(const string & name)
{
    auto it = sym_tab.find(name);
    if(it != sym_tab.end()){
        return it->second;
    }
    else {
        throw runtime_error("undefined variable");
    }
}

void Symbol_table::set(const string & name, int nv)
{
    Attributes& attr =  get(name);
    attr.value = nv;
}

bool Symbol_table::is_declared(const string & name)
{
    if(sym_tab.find(name) != sym_tab.end())
        return true;
    return false;
}

void Symbol_table::declare(const string & name, int nv)
{
    if (is_declared(name)) throw runtime_error("declared twice");
    sym_tab.insert(make_pair(name,Attributes(nv)));
}