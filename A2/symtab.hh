#ifndef SYMTAB_HH
#define SYMTAB_HH
#include <string>
#include <map>
#include <iostream>
using namespace std;

class SymTabEntry;

class SymTab
{
    public:
    int offset;
    std::map<std::string, SymTabEntry> Entries;
    void printgst();
    void print();
    SymTabEntry* search(string s);
};

class SymTabEntry
{
    public:
    std::string varfun;
    std::string scope;
    int size;
    int offset;
    std::string type;
    std::string returntype;
    SymTab* symtab;
    void print();
};
#endif