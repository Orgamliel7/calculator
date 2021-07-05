
#include<iostream>
#include<string>
#include<map>
#include<algorithm>
using namespace std;

class Attributes {
public:
    int value;
    explicit Attributes(int nv) : value(nv) { }
};

class Symbol_table {
public:
    Attributes& get(const string & name);
    void set(const string & name, int nv);
    bool is_declared(const string & name);
    void declare(const string & name, int nv);
private:
    map<string, Attributes> sym_tab;
};
