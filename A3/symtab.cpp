#include "symtab.hh"
#include <iostream>
using namespace std;

void SymTab::printgst()
{
    // cout<<"[\n";

    for(auto it = Entries.begin(); it!= Entries.end(); ++it)
    {
        //cout<<"[\n";
        // cout<<"\""<<it->first<<"\",";
        // cout<<"\""<<it->second.varfun<<"\",";
        // cout<<"\""<<it->second.scope<<"\",";
        // cout<<it->second.size<<",";
        if(it->second.varfun != "fun" && it->second.scope == "global"){
            // cout<<"\"-\""<<",";
        }
        else{
            // cout<<it->second.offset<<",";
        }
        // cout<<"\""<<it->second.returntype<<"\"";
        // cout<<"]\n";
        if(next(it,1) != Entries.end()){
            // cout<<","<<endl;
        }
    }
    // cout<<"]\n";
}

void SymTab::print()
{
    printgst();
}

SymTabEntry* SymTab::search(string s)
{
    return &Entries[s];
}

void SymTabEntry::print()
{
    // cout<<"[";
    // cout<<"\""<<varfun<<"\",";
    // cout<<"\""<<scope<<"\",";
    // cout<<size<<"";
    // cout<<offset<<"";
    // cout<<"\""<<returntype<<"\"";
    // cout<<"]\n";     
}