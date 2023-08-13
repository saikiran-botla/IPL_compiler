#ifndef AST_HH
#define AST_HH

#include <fstream>
#include <bits/stdc++.h>
using namespace std;

enum typeExp{
    Empty_astnode,
    Seq_astnode,
    AssignS_astnode,
    Return_astnode,
    If_astnode,
    While_astnode,
    For_astnode,
    Proccall_astnode,

    Identifier_astnode,
    Arrayref_astnode,
    Arrow_astnode,
    Member_astnode,

    Op_binary_astnode,
    Op_unary_astnode,
    AssignE_astnode,
    Funcall_astnode,
    Intconst_astnode,
    Floatconst_astnode,
    Stringconst_astnode,
};

class abstract_astnode
{
public:
virtual void print(int blanks) = 0;
enum typeExp astnode_type;
int offset;
string s;
};

// int l_count;

//Abstract Classes :
class statement_astnode : public abstract_astnode
{
    public:
};

class exp_astnode : public abstract_astnode
{
    public:
    std::string data_type;//0 ---> int and 1 ---> float
    int label;
    int i;
};

class ref_astnode : public exp_astnode
{
    public:
};


//Non-Abstract Classes :


class identifier_astnode : public ref_astnode
{
    public:

    string s;

    void print(int blanks);
    identifier_astnode(string st);

};



//Statement class children
class empty_astnode : public statement_astnode
{
    public:
    void print(int blanks);
    empty_astnode();
};

class seq_astnode : public statement_astnode
{
    public:
    void print(int blanks);
    seq_astnode();
    void pushback(statement_astnode* node);

    vector<statement_astnode*> statements;

};

class  assignS_astnode : public statement_astnode
{
    public:
    void print(int blanks);
    assignS_astnode(exp_astnode* left, exp_astnode* right);

    exp_astnode* l_exp;
    exp_astnode* r_exp;
};

class return_astnode : public statement_astnode
{
    public:
    void print(int blanks);
    return_astnode(exp_astnode* expr);

    private :
    exp_astnode* exp;
};

class if_astnode : public statement_astnode
{
    public:
    void print(int blanks);
    if_astnode(exp_astnode* expr, statement_astnode* stmtment1, statement_astnode* stmtment2);

    private :
    exp_astnode* exp;
    statement_astnode* stmt1;
    statement_astnode* stmt2;
};

class while_astnode : public statement_astnode
{
    public:
    void print(int blanks);
    while_astnode(exp_astnode* expr, statement_astnode* stmtent);

    private :
    exp_astnode* exp;
    statement_astnode* stmt;
};

class for_astnode : public statement_astnode
{
    public:
    void print(int blanks);
    for_astnode(exp_astnode* expr1, exp_astnode* expr2, exp_astnode* expr3, statement_astnode* stmtment);

    private :
    exp_astnode* exp1;
    exp_astnode* exp2;
    exp_astnode* exp3;
    statement_astnode* stmt;
};


class proccall_astnode : public statement_astnode
{
    public:

    identifier_astnode* procname;
    vector<exp_astnode*> expressions;

    void print(int blanks);
    proccall_astnode();
    proccall_astnode(identifier_astnode* c);
    void pushback(exp_astnode* node);
    void setname(string name);

};



//ref class children 


class arrayref_astnode : public ref_astnode
{
    public:
    void print(int blanks);
    arrayref_astnode(exp_astnode* expr1, exp_astnode* expr2);

    exp_astnode* exp1;
    exp_astnode* exp2;
};

class member_astnode : public ref_astnode
{
    public:
    void print(int blanks);
    member_astnode(exp_astnode* expr, identifier_astnode* iden);

    private :
    exp_astnode* exp;
    identifier_astnode* id;
};

class arrow_astnode : public ref_astnode
{
    public:
    void print(int blanks);
    arrow_astnode(exp_astnode* expr, identifier_astnode* iden);

    private :
    exp_astnode* exp;
    identifier_astnode* id;
};


//expression class children
class op_binary_astnode : public exp_astnode
{
    public:
    void print(int blanks);
    op_binary_astnode(string st, exp_astnode* expr1, exp_astnode* expr2);

    private:
    string s;
    exp_astnode* exp1;
    exp_astnode* exp2;
};

class op_unary_astnode : public exp_astnode
{
    public:
    void print(int blanks);
    op_unary_astnode(string st, exp_astnode* expr);

    private:
    string s;
    exp_astnode* exp;
};

class assignE_astnode : public exp_astnode
{
    public:
    void print(int blanks);
    assignE_astnode(exp_astnode* expr1, exp_astnode* expr2);

    exp_astnode* exp1;
    exp_astnode* exp2;
};

class funcall_astnode : public exp_astnode
{
    public:
    void print(int blanks);
    funcall_astnode();
    funcall_astnode(identifier_astnode* c);
    void pushback(exp_astnode* node);
    void setname(string name);

    identifier_astnode* funcname;
    vector<exp_astnode*> expressions;
};

class intconst_astnode : public exp_astnode
{
    public:
    void print(int blanks);
    intconst_astnode(int in);

    private:
    int i;
};

class floatconst_astnode : public exp_astnode
{
    public:
    void print(int blanks);
    floatconst_astnode(float fl);

    private:
    float f;
};

class stringconst_astnode : public exp_astnode
{
    public:
    void print(int blanks);
    stringconst_astnode(string st);

    private:
};
void gencode(exp_astnode* l, exp_astnode* r, string s);


class stak
{
    public:
    int max;
    int top;
    vector<string> st;
    stak();
    void push(string s);
    string pop();
    string top_r();
    void swap();
    void print();
};

class lc_node
{
    public:
    int t;
    string st;
    string globl;
    string typ;
    void print();
};

int labeller(int a, int b);
bool isfloattype(string s);
bool isinttype(string s);

#endif