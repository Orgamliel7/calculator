
#include "scanner.h"


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
