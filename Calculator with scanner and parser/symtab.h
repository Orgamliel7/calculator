#include<iostream>
#include<string>
#include<map>
#include<algorithm>
#include <regex>
#include<stdexcept>

using namespace std;

class Attributes
        {
public:
    int value;
    explicit Attributes(int nv) : value(nv) { }
};

class Symbol_table
        {
public:
    Attributes& get(string name);                    // return the attributes of name
    void set(string name, int nv);                // set the attribute of value
    bool is_declared(string name);                   // is name already in symtab?
    void declare(string name, int nv);            // add name to symtab
    map<string, Attributes> sym_tab;
    
};

