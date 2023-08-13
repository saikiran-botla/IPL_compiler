
#include <fstream>
#include "astnode.hh"
#include<stdio.h>
#include<iostream>
using namespace std;


void printAst(const char *astname, const char *fmt...) // fmt is a format string that tells about the type of the arguments.
{   
	typedef vector<abstract_astnode *>* pv;
	va_list args;
	va_start(args, fmt);
	if ((astname != NULL) && (astname[0] != '\0'))
	{
		cout << "{ ";
		cout << "\"" << astname << "\"" << ": ";
	}
	cout << "{" << endl;
	while (*fmt != '\0')
	{
		if (*fmt == 'a')
		{
			char * field = va_arg(args, char *);
			abstract_astnode *a = va_arg(args, abstract_astnode *);
			cout << "\"" << field << "\": " << endl;
			
			a->print(0);
		}
		else if (*fmt == 's')
		{
			char * field = va_arg(args, char *);
			char *str = va_arg(args, char *);
			cout << "\"" << field << "\": ";

			cout << str << endl;
		}
		else if (*fmt == 'i')
		{
			char * field = va_arg(args, char *);
			int i = va_arg(args, int);
			cout << "\"" << field << "\": ";

			cout << i;
		}
		else if (*fmt == 'f')
		{
			char * field = va_arg(args, char *);
			double f = va_arg(args, double);
			cout << "\"" << field << "\": ";
			cout << f;
		}
		else if (*fmt == 'l')
		{
			char * field = va_arg(args, char *);
			pv f =  va_arg(args, pv);
			cout << "\"" << field << "\": ";
			cout << "[" << endl;
			for (int i = 0; i < (int)f->size(); ++i)
			{
				(*f)[i]->print(0);
				if (i < (int)f->size() - 1)
					cout << "," << endl;
				else
					cout << endl;
			}
			cout << endl;
			cout << "]" << endl;
		}
		++fmt;
		if (*fmt != '\0')
			cout << "," << endl;
	}
	cout << "}" << endl;
	if ((astname != NULL) && (astname[0] != '\0'))
		cout << "}" << endl;
	va_end(args);
}






//Statement_astnode children
empty_astnode::empty_astnode() : statement_astnode()
{
	this->astnode_type = Empty_astnode;
}

void empty_astnode:: print(int blanks)
{
	cout<<"\"empty\""<<endl;
}

seq_astnode::seq_astnode() : statement_astnode()
{
	astnode_type = Seq_astnode;
}

void seq_astnode::pushback(statement_astnode* node)
{
	statements.push_back(node);
}

void seq_astnode::print(int blanks)
{
	printAst("","l","seq",statements);
}

assignS_astnode::assignS_astnode(exp_astnode* left, exp_astnode* right) : statement_astnode()
{
	l_exp = left;
	r_exp = right;
	astnode_type = AssignS_astnode;
}

void assignS_astnode::print(int blanks)
{
	printAst("assignS", "aa", "left", l_exp, "right", r_exp);
}

proccall_astnode::proccall_astnode(identifier_astnode* c) : statement_astnode()
{
	procname = c;
	astnode_type = Proccall_astnode;
}

proccall_astnode::proccall_astnode() : statement_astnode()
{
	astnode_type = Proccall_astnode;
}

void proccall_astnode::setname(string name)
{
	procname = new identifier_astnode(name);
}

void proccall_astnode::pushback(exp_astnode* node)
{
	expressions.push_back(node);
}

void proccall_astnode::print(int blanks)
{
	printAst("proccall","al","fname",procname,"params", expressions);
}

return_astnode::return_astnode(exp_astnode* expr) : statement_astnode()
{
	exp = expr;
	astnode_type = Return_astnode;
}

void return_astnode::print(int blanks)
{
	printAst("", "a", "return", exp);
}

if_astnode::if_astnode(exp_astnode* expr, statement_astnode* stmtment1, statement_astnode* stmtment2) : statement_astnode()
{
	exp = expr;
	stmt1 = stmtment1;
	stmt2 = stmtment2;
	astnode_type = If_astnode;
}

void if_astnode::print(int blanks)
{
	printAst("if", "aaa", "cond", exp, "then", stmt1, "else", stmt2);
}

while_astnode::while_astnode(exp_astnode* expr, statement_astnode* stmtent) : statement_astnode()
{
	exp = expr;
	stmt  = stmtent;
	astnode_type = While_astnode;
}

void while_astnode::print(int blanks)
{
	printAst("while","aa", "cond", exp, "stmt", stmt);
}

for_astnode::for_astnode(exp_astnode* expr1, exp_astnode* expr2, exp_astnode* expr3, statement_astnode* stmtment) : statement_astnode()
{
	exp1 = expr1;
	exp2 = expr2;
	exp3 = expr3;
	stmt = stmtment;
	astnode_type = For_astnode;
}

void for_astnode::print(int blanks)
{
	printAst("for","aaaa", "init", exp1, "guard", exp2, "step", exp3, "body", stmt);
}




//ref_astnode children

identifier_astnode::identifier_astnode(string st) : ref_astnode()
{
	s = st;
	astnode_type = Identifier_astnode;
}

void identifier_astnode::print(int blanks)
{
	string str = "\"" + s + "\"";
	char* str1  = const_cast<char*>(str.c_str());
	printAst("", "s", "identifier", str1);
}

arrayref_astnode::arrayref_astnode(exp_astnode* expr1, exp_astnode* expr2) : ref_astnode()
{
	exp1 = expr1;
	exp2 = expr2;
	astnode_type = Arrayref_astnode;
}

void arrayref_astnode::print(int blanks)
{
	printAst("arrayref", "aa", "array", exp1, "index", exp2);
}

member_astnode::member_astnode(exp_astnode* expr, identifier_astnode* iden) : ref_astnode()
{
	exp = expr;
	id = iden;
	astnode_type = Member_astnode;
}

void member_astnode::print(int blanks)
{
	printAst("member", "aa", "struct", exp, "field", id);
}

arrow_astnode::arrow_astnode(exp_astnode* expr, identifier_astnode* iden) : ref_astnode()
{
	exp = expr;
	id = iden;
	astnode_type = Member_astnode;
}

void arrow_astnode::print(int blanks)
{
	printAst("arrow", "aa", "pointer", exp, "field", id);
}



//expression_class children

op_binary_astnode::op_binary_astnode(string st, exp_astnode* expr1, exp_astnode* expr2) : exp_astnode()
{
	s = st;
	exp1 = expr1;
	exp2 = expr2;
	astnode_type = Op_binary_astnode;
}

void op_binary_astnode::print(int blanks)
{
	string str = "\""+s+"\"";
	char* str1 = const_cast<char*>(str.c_str());
	printAst("op_binary", "saa", "op", str1, "left", exp1, "right", exp2);
}

op_unary_astnode::op_unary_astnode(string st, exp_astnode* expr) : exp_astnode()
{
	s = st;
	exp = expr;
	astnode_type = Op_unary_astnode;
}

void op_unary_astnode::print(int blanks)
{
	string str = "\""+s+"\"";
	char* str1 = const_cast<char*>(str.c_str());
	printAst("op_unary", "sa", "op", str1, "child", exp);
}

assignE_astnode::assignE_astnode(exp_astnode* expr1, exp_astnode* expr2) : exp_astnode()
{
	exp1 = expr1;
	exp2 = expr2;
	astnode_type = AssignE_astnode;
}

void assignE_astnode::print(int blanks)
{
	printAst("assignE","aa", "left", exp1, "right", exp2);
}

funcall_astnode::funcall_astnode(identifier_astnode* c) : exp_astnode()
{
	funcname = c;
	astnode_type = Funcall_astnode;
}

funcall_astnode::funcall_astnode() : exp_astnode()
{
	astnode_type = Funcall_astnode;
}

void funcall_astnode::setname(string name)
{
	funcname = new identifier_astnode(name);
}

void funcall_astnode::pushback(exp_astnode* node)
{
	expressions.push_back(node);
}

void funcall_astnode::print(int blanks)
{
	printAst("funcall","al","fname",funcname,"params", expressions);
}

intconst_astnode::intconst_astnode(int in) : exp_astnode()
{
	i = in;
	astnode_type = Intconst_astnode;
}

void intconst_astnode::print(int blanks)
{
	printAst("","i","intconst", i);
}

floatconst_astnode::floatconst_astnode(float fl) : exp_astnode()
{
	f = fl;
	astnode_type = Floatconst_astnode;
}

void floatconst_astnode::print(int blanks) 
{
	printAst("","f","floatconst", f);
}

stringconst_astnode::stringconst_astnode(string st) : exp_astnode()
{
	s = st;
	astnode_type = Stringconst_astnode;
}

void stringconst_astnode::print(int blanks)
{
	printAst("","s","stringconst", &s[0]);
}

bool isfloattype(string s)
{
	return (s == "float");
}

bool isinttype(string s)
{
	return (s=="int");
}

