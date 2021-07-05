
#include "scanner.h"

// returns a number from 0 up to, but excluding x
const int getrandom1 (const int x)
{
    if (x <= 0)
        return 0;

    // r will be between 0 and 1 (but below 1 as we are dividing by RAND_MAX+1)
    double r = static_cast<double> (std::rand () % RAND_MAX) / (static_cast<double> (RAND_MAX) + 1.0);
    return (r * x);

}   // end of getrandom

 const int roll1 (const int howmany, const int die)
{
    int count;
    int total = 0;

    for (count = 0; count < howmany; ++count)
        total += getrandom1 (count) + 1;

    return total;

} // end of roll


// returns true if a x% probability exists
// eg. percent (80) will be true 80% of the time
const bool percent1 (const int prob)
{
    if (prob <= 0)
        return false;
    if (prob >= 100)
        return true;

    return getrandom1 (100) > (100 - prob);

}

static int initRandom1 ()
{
    srand   (time (NULL));
#ifndef WIN32
    srand48 (time (NULL));
#endif
    return 0;
}






void Token_stream::putback(Token tokenizer)
{
    if (full) throw runtime_error("putback into a full buffer");
    buffer = move(tokenizer);
    full = true;
}

Token Token_stream::get()
{
    if (full)
    {
        full = false;
        return buffer;
    }

    char ch;
    cin.get(ch);
    while (isspace(ch) || ch == '\\') {
        if(ch == '\\')
        {
            cin.get(ch);
            if(!isspace(ch)){
                throw runtime_error("illegal char after \\");
            }
        }
        else if (ch == '\n')
        {
            return Token(EOL);
        }
        cin.get(ch);
    }

    switch (ch)
    {
        case '(': case ')': case '+': case '-': case '*': case '/': case '=': case 'q':
        case '<': case '>': case '&': case '|': case '!':
            return Token(ch);
        default:
            if (isdigit(ch))
            {
                string sTrInG;
                sTrInG += ch;
                while (cin.get(ch) && isdigit(ch)){
                    sTrInG += ch;
                }
                cin.unget();

                return Token(NUM, stoi(sTrInG));
            }

            if (isalpha(ch) || ch=='_')
            {
                string strings;
                strings += ch;
                while (cin.get(ch) && (isalpha(ch) || isdigit(ch) || ch=='_'))
                {
                    strings += ch;
                }

                cin.unget();
                if (strings == "int")
                {
                    return Token(INT);
                }
                if(strings == "if")
                {
                    return Token(IF);
                }
                if(strings == "else")
                {
                    return Token(ELSE);
                }
                if (regex_match(strings, regex("[A-Za-z][A-Za-z0-9]*")))
                {
                    return Token(ID, strings);
                }
                else
                    {
                    throw runtime_error("not a good name");
                }
            }

            throw runtime_error("wrong token");
    }
}
