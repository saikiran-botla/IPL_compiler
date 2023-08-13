
#include <fstream>
#include "astnode.hh"
#include<stdio.h>
#include<iostream>
using namespace std;

int l_count = 0;

void printAst(const char *astname, const char *fmt...) // fmt is a format string that tells about the type of the arguments.
{   
	typedef vector<abstract_astnode *>* pv;
	va_list args;
	va_start(args, fmt);
	if ((astname != NULL) && (astname[0] != '\0'))
	{
		// cout << "{ ";
		// cout << "\"" << astname << "\"" << ": ";
	}
	// cout << "{" << endl;
	while (*fmt != '\0')
	{
		if (*fmt == 'a')
		{
			char * field = va_arg(args, char *);
			abstract_astnode *a = va_arg(args, abstract_astnode *);
			// cout << "\"" << field << "\": " << endl;
			
			a->print(0);
		}
		else if (*fmt == 's')
		{
			char * field = va_arg(args, char *);
			char *str = va_arg(args, char *);
			// cout << "\"" << field << "\": ";

			// cout << str << endl;
		}
		else if (*fmt == 'i')
		{
			char * field = va_arg(args, char *);
			int i = va_arg(args, int);
			// cout << "\"" << field << "\": ";

			// cout << i;
		}
		else if (*fmt == 'f')
		{
			char * field = va_arg(args, char *);
			double f = va_arg(args, double);
			// cout << "\"" << field << "\": ";
			// cout << f;
		}
		else if (*fmt == 'l')
		{
			char * field = va_arg(args, char *);
			pv f =  va_arg(args, pv);
			// cout << "\"" << field << "\": ";
			// cout << "[" << endl;
			for (int i = 0; i < (int)f->size(); ++i)
			{
				(*f)[i]->print(0);
				if (i < (int)f->size() - 1){}
					// cout << "," << endl;
				else{}
					// cout << endl;
			}
			// cout << endl;
			// cout << "]" << endl;
		}
		++fmt;
		if (*fmt != '\0'){}
			// cout << "," << endl;
	}
	// cout << "}" << endl;
	if ((astname != NULL) && (astname[0] != '\0'))
		// cout << "}" << endl;
	va_end(args);
}



int labeller(int a, int b)
{
	if(a==b){
		return a+1;
	}
	if(a>b){
		return a;
	}
	return b;
}

void stak::print(){
	for(int i=0;i<=top;i++){
		cout<<st[i]<<" ";
	}
	cout<<endl;
}

stak::stak()
{
	max = 5;
	top = 5;
	st.push_back("%esi");
	st.push_back("%edi");
	st.push_back("%ebx");
	st.push_back("%ecx");
	st.push_back("%edx");
	st.push_back("%eax");
}

void stak::push( string stringsss)
{
	st.push_back(stringsss);
	top += 1;
}

string stak::top_r()
{
	return st[top];
}

void stak::swap()
{
	string p = st[top];
	st[top] = st[top-1];
	st[top-1] = p;
	return;
}

string stak::pop()
{
	string p = st[top];
	top = top-1;
	st.pop_back();
	return p;
}

stak reg = stak();

//Statement_astnode children
empty_astnode::empty_astnode() : statement_astnode()
{
	this->astnode_type = Empty_astnode;
}

void empty_astnode:: print(int blanks)
{
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
	if(l_exp->label < r_exp->label && l_exp->label < 6){
		reg.swap();
		r_exp->print(0);
		string pop_reg = reg.pop();
		l_exp->print(0);
		string r = reg.top_r();
		if(l_exp->astnode_type == Identifier_astnode){
			cout<<"\tmovl\t"<<pop_reg<<", "<<l_exp->offset<<"(%ebp)"<<endl;
		}
		reg.push(pop_reg);
		reg.swap();
	}

	if(l_exp->label >= r_exp->label && r_exp->label < 6){
		l_exp->print(0);
		string pop_reg = reg.pop();
		r_exp->print(0);
		string r = reg.top_r();
		if(l_exp->astnode_type == Identifier_astnode){
			cout<<"\tmovl\t"<<r<<", "<<l_exp->offset<<"(%ebp)"<<endl;
		}	
		reg.push(pop_reg);
	}
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
	if(procname->s != "printf"){
		for(int i=0;i< (int)expressions.size();i++){
			expressions[i]->print(0);
			cout<<"\tpushl\t"<<reg.top_r()<<"\n";
		}
		cout<<"\tcall\t"<<procname->s<<endl;
		cout<<"\taddl\t$"<<4*expressions.size()<<", %esp"<<endl;
	}
	else{
		for(int i=0;i< (int)expressions.size()-1;i++){
			expressions[(int)expressions.size()-1-i]->print(0);
			cout<<"\tpushl\t"<<reg.top_r()<<"\n";
		}
		cout<<"\tpushl\t"<<"$"<<expressions[0]->s<<"\n";
		cout<<"\tcall\t"<<procname->s<<endl;
		cout<<"\taddl\t$"<<4*expressions.size()<<", %esp"<<endl;;
	}
}

return_astnode::return_astnode(exp_astnode* expr) : statement_astnode()
{
	exp = expr;
	astnode_type = Return_astnode;
}

void return_astnode::print(int blanks)
{
	exp->print(0);
	cout<<"\tmovl\t"<<reg.top_r()<<", %eax\n";
	cout<<"\tleave\n";
	cout<<"\tret\n";
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
	exp->print(0);
	cout<<"\tcmpl\t$0, "<<reg.top_r()<<endl;
	cout<<"\tje\t.L"<<l_count<<endl;
	int block_0 = l_count;
	l_count++;
	stmt1->print(0);
	cout<<"\tjmp\t.L"<<l_count<<endl;
	cout<<".L"<<block_0<<":"<<endl;
	int block_1 = l_count;
	l_count++;
	stmt2->print(0);
	cout<<"\tjmp\t.L"<<block_1<<endl;
	cout<<".L"<<block_1<<":"<<endl;
}

while_astnode::while_astnode(exp_astnode* expr, statement_astnode* stmtent) : statement_astnode()
{
	exp = expr;
	stmt  = stmtent;
	astnode_type = While_astnode;
}

void while_astnode::print(int blanks)
{
	cout<<"\tjmp\t.L"<<l_count<<endl;
	int block_0 = l_count;
	l_count++;
	cout<<".L"<<l_count<<":"<<endl;
	int block_1 = l_count;
	l_count++;
	stmt->print(0);
	cout<<".L"<<block_0<<":"<<endl;
	exp->print(0);
	cout<<"\tcmpl\t$0, "<<reg.top_r()<<endl;
	cout<<"\tjne\t.L"<<block_1<<endl;
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
	exp1->print(0);
	cout<<"\tjmp\t.L"<<l_count<<endl;
	int block_0 = l_count;
	l_count++;
	cout<<".L"<<l_count<<":"<<endl;
	int block_1 = l_count;
	l_count++;
	stmt->print(0);
	exp3->print(0);
	cout<<".L"<<block_0<<":"<<endl;
	exp2->print(0);
	cout<<"\tcmpl\t$0, "<<reg.top_r()<<endl;
	cout<<"\tjne\t.L"<<block_1<<endl;
}




//ref_astnode children

identifier_astnode::identifier_astnode(string st) : ref_astnode()
{
	s = st;
	astnode_type = Identifier_astnode;
}

void identifier_astnode::print(int blanks)
{
	cout<<"\tmovl\t"<<offset<<"(%ebp)"<<", "<<reg.top_r()<<endl;
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
	//labeller shud be written;
	label = labeller(exp1->label, exp2->label);
}

void op_binary_astnode::print(int blanks)
{
	gencode(exp1, exp2, s);
}

void gencode(exp_astnode* exp1, exp_astnode* exp2, string s)
{
	// if(exp2->label == 1){
	// 	exp1->print(0);
	// 	string r = reg.top_r();
	// 	if(s == "PLUS_INT"){
	// 		if(exp2->astnode_type == Identifier_astnode){
	// 			cout<<"addl -4(%ebp), "<<r<<endl;
	// 		}
	// 		if(exp2->astnode_type == Intconst_astnode){
	// 			cout<<"addl "<<exp2->i<<" "<<r<<endl;
	// 		}
	// 	}
	// 	if(s == "MINUS_INT"){
	// 		if(exp2->astnode_type == Identifier_astnode){
	// 			cout<<"subl -4(%ebp), "<<r<<endl;
	// 		}
	// 		if(exp2->astnode_type == Intconst_astnode){
	// 			cout<<"subl "<<exp2->i<<" "<<r<<endl;
	// 		}
	// 	}
	// }

	if(exp1->label < exp2->label && exp1->label < 6){
		if(s!= "DIV_INT"){
			reg.swap();
			exp2->print(0);
			string pop_reg = reg.pop();
			exp1->print(0);
			string r = reg.top_r();
			if(s == "PLUS_INT"){
				cout<<"\taddl\t"<< pop_reg << ", "<<r<<endl;
			} 
			if(s == "MINUS_INT"){
				cout<<"\tsubl\t"<< pop_reg << ", "<<r<<endl;
			}
			if(s == "OR_OP"){
				cout<<"\tcmpl\t$0, "<< r<<endl;
				cout<<"\tjne\t.L"<<l_count<<endl;
				l_count++;
				cout<<"\tcmpl\t$0, "<<pop_reg<<endl;
				cout<<"\tje\t.L"<<l_count<<endl;
				l_count++;
				cout<<".L"<<l_count-2<<":"<<endl;
				cout<<"\tmovl\t$1, "<<r<<endl;
				cout<<"\tjmp\t.L"<<l_count<<endl;
				l_count++;
				cout<<".L"<<l_count-2<<":"<<endl;
				cout<<"\tmovl\t$0, "<<r<<endl;
				cout<<".L"<<l_count-1<<":"<<endl;
			}
			if(s == "AND_OP"){
				cout<<"\tcmpl\t$0, "<< r<<endl;
				cout<<"\tje .L"<<l_count<<endl;
				l_count++;
				cout<<"\tcmpl\t$0, "<<pop_reg<<endl;
				cout<<"\tje\t.L"<<l_count-1<<endl;
				l_count++;
				cout<<"\tmovl\t$1, "<<r<<endl;
				cout<<"\tjmp\t.L"<<l_count-1<<endl;
				cout<<".L"<<l_count-2<<":"<<endl;
				cout<<"\tmovl\t$0, "<<r<<endl;
				cout<<".L"<<l_count-1<<":"<<endl;
			}
			if(s == "EQ_OP_INT"){
				cout<<"\tcmpl\t"<<pop_reg<<", "<<r<<endl;
				cout<<"\tsete\t"<<"%al\n";
				cout<<"\tmovzbl\t%al, "<<r<<endl;
			}
			if(s == "NE_OP_INT"){
				cout<<"\tcmpl	"<<pop_reg<<", "<<r<<endl;
				cout<<"\tsetne		"<<"%al\n";
				cout<<"\tmovzbl	%al, "<<r<<endl;
			}
			if(s == "LT_OP_INT"){
				cout<<"\tcmpl	"<<pop_reg<<", "<<r<<endl;
				cout<<"\tsetl		"<<"%al\n";
				cout<<"\tmovzbl	%al, "<<r<<endl;
			}
			if(s == "GT_OP_INT"){
				cout<<"\tcmpl\t"<<pop_reg<<", "<<r<<endl;
				cout<<"\tsetg\t"<<"%al\n";
				cout<<"\tmovzbl\t%al, "<<r<<endl;
			}
			if(s == "LE_OP_INT"){
				cout<<"\tcmpl\t"<<pop_reg<<", "<<r<<endl;
				cout<<"\tsetle\t"<<"%al\n";
				cout<<"\tmovzbl\t%al, "<<r<<endl;
			}
			if(s == "GE_OP_INT"){
				cout<<"\tcmpl\t"<<pop_reg<<", "<<r<<endl;
				cout<<"\tsetge\t"<<"%al\n";
				cout<<"\tmovzbl\t%al, "<<r<<endl;
			}
			if(s == "MULT_INT"){
				cout<<"\timull\t"<<pop_reg<<", "<<r<<endl;
			}
			reg.push(pop_reg);
			reg.swap();
		}
		else if(s == "DIV_INT"){
			// if(reg.top() == "%eax"){
			// 	cout<<"\tpushl\t%edx"<<endl;
			// 	cout<<"\tpushl\t%ecx"<<endl;
			// }
			// if(reg.top() == "%edx"){
			// 	cout<<"\tpushl\t%eax"<<endl;
			// 	cout<<"\tpushl\t%ecx"<<endl;
			// }
			// if(reg.top() == "%ecx"){
			// 	cout<<"\tpushl\t%edx"<<endl;
			// 	cout<<"\tpushl\t%ecx"<<endl;
			// }
			if(reg.top_r() != "%eax"){
				cout<<"\tpushl\t%eax"<<endl;
			}
			if(reg.top_r() != "%edx"){
				cout<<"\tpushl\t%edx"<<endl;
			}
			if(reg.top_r() != "%ecx"){
				cout<<"\tpushl\t%ecx"<<endl;
			}
			reg.push("%ecx");
			reg.push("%eax");
			exp1->print(0);
			string pop_reg = reg.pop();
			exp2->print(0);
			cout<<"\tcltd"<<endl;
			cout<<"\tidivl\t"<<reg.top_r()<<endl;
			reg.pop();
			if(reg.top_r() == "%eax"){
				cout<<"\tpopl\t%ecx"<<endl;
				cout<<"\tpopl\t%edx"<<endl;
			}
			else if(reg.top_r() == "%edx"){
				cout<<"\tmovl\t"<<pop_reg<<", %edx"<<endl;
				cout<<"\tpopl\t%ecx"<<endl;
				cout<<"\tpopl\t%eax"<<endl;
			}
			else if(reg.top_r() == "%ecx"){
				cout<<"\tmovl\t"<<pop_reg<<", %ecx"<<endl;
				cout<<"\tpopl\t%edx"<<endl;
				cout<<"\tpopl\t%eax"<<endl;
			}
			else{
				cout<<"\tmovl\t"<<pop_reg<<", "<<reg.top_r()<<endl;
				cout<<"\tpopl\t%ecx"<<endl;
				cout<<"\tpopl\t%edx"<<endl;
				cout<<"\tpopl\t%eax"<<endl;
			}
		}
	}

	if(exp1->label >= exp2->label && exp2->label < 6){
		if(s!= "DIV_INT"){
		exp1->print(0);
		string pop_reg = reg.pop();
		exp2->print(0);
		string r = reg.top_r();
		if(s == "PLUS_INT"){
			
			cout<<"\taddl\t"<< r << ", "<<pop_reg<<endl;
		}
		if(s == "MINUS_INT"){
			cout<<"\tsubl\t"<< r << ", "<<pop_reg<<endl;
		}
		if(s == "OR_OP"){
			cout<<"\tcmpl\t$0, "<< pop_reg<<endl;
			cout<<"\tjne\t.L"<<l_count<<endl;
			l_count++;
			cout<<"\tcmpl\t$0, "<<r<<endl;
			cout<<"\tje\t.L"<<l_count<<endl;
			l_count++;
			cout<<".L"<<l_count-2<<":"<<endl;
			cout<<"\tmovl\t$1, "<<pop_reg<<endl;
			cout<<"\tjmp\t.L"<<l_count<<endl;
			l_count++;
			cout<<".L"<<l_count-2<<":"<<endl;
			cout<<"\tmovl\t$0, "<<pop_reg<<endl;
			cout<<".L"<<l_count-1<<":"<<endl;
		}
		if(s == "AND_OP"){
			cout<<"\tcmpl\t$0, "<< pop_reg<<endl;
			cout<<"\tje\t.L"<<l_count<<endl;
			l_count++;
			cout<<"\tcmpl\t$0, "<<r<<endl;
			cout<<"\tje\t.L"<<l_count-1<<endl;
			l_count++;
			cout<<"\tmovl\t$1, "<<pop_reg<<endl;
			cout<<"\tjmp\t.L"<<l_count-1<<endl;
			cout<<".L"<<l_count-2<<":"<<endl;
			cout<<"\tmovl\t$0, "<<pop_reg<<endl;
			cout<<".L"<<l_count-1<<":"<<endl;
		}
		if(s == "EQ_OP_INT"){
			cout<<"\tcmpl\t"<<r<<", "<<pop_reg<<endl;
			cout<<"\tsete\t"<<"%al\n";
			cout<<"\tmovzbl\t%al, "<<pop_reg<<endl;
		}
		if(s == "NE_OP_INT"){
			cout<<"\tcmpl\t"<<r<<", "<<pop_reg<<endl;
			cout<<"\tsetne\t"<<"%al\n";
			cout<<"\tmovzbl\t%al, "<<pop_reg<<endl;
		}
		if(s == "LT_OP_INT"){
			cout<<"\tcmpl	"<<r<<", "<<pop_reg<<endl;
			cout<<"\tsetl\t"<<"%al\n";
			cout<<"\tmovzbl	%al, "<<pop_reg<<endl;
		}
		if(s == "GT_OP_INT"){
			cout<<"\tcmpl\t"<<r<<", "<<pop_reg<<endl;
			cout<<"\tsetg\t"<<"%al\n";
			cout<<"\tmovzbl\t%al, "<<pop_reg<<endl;
		}
		if(s == "LE_OP_INT"){
			cout<<"\tcmpl\t"<<r<<", "<<pop_reg<<endl;
			cout<<"\tsetle\t"<<"%al\n";
			cout<<"\tmovzbl\t%al, "<<pop_reg<<endl;
		}
		if(s == "GE_OP_INT"){
			cout<<"\tcmpl\t"<<r<<", "<<pop_reg<<endl;
			cout<<"\tsetge\t"<<"%al\n";
			cout<<"\tmovzbl\t%al, "<<pop_reg<<endl;
		}
		if(s == "MULT_INT"){
			cout<<"\timull\t"<<r<<", "<<pop_reg<<endl;
		}
		reg.push(pop_reg);
		}
		else if(s == "DIV_INT"){
			// if(reg.top() == "%eax"){
			// 	cout<<"\tpushl\t%edx"<<endl;
			// 	cout<<"\tpushl\t%ecx"<<endl;
			// }
			// if(reg.top() == "%edx"){
			// 	cout<<"\tpushl\t%eax"<<endl;
			// 	cout<<"\tpushl\t%ecx"<<endl;
			// }
			// if(reg.top() == "%ecx"){
			// 	cout<<"\tpushl\t%edx"<<endl;
			// 	cout<<"\tpushl\t%ecx"<<endl;
			// }
			if(reg.top_r() != "%eax"){
				cout<<"\tpushl\t%eax"<<endl;
			}
			if(reg.top_r() != "%edx"){
				cout<<"\tpushl\t%edx"<<endl;
			}
			if(reg.top_r() != "%ecx"){
				cout<<"\tpushl\t%ecx"<<endl;
			}
			reg.push("%ecx");
			reg.push("%eax");
			exp1->print(0);
			string pop_reg = reg.pop();
			exp2->print(0);
			cout<<"\tcltd"<<endl;
			cout<<"\tidivl\t"<<reg.top_r()<<endl;
			reg.pop();
			if(reg.top_r() == "%eax"){
				cout<<"\tpopl\t%ecx"<<endl;
				cout<<"\tpopl\t%edx"<<endl;
			}
			else if(reg.top_r() == "%edx"){
				cout<<"\tmovl\t"<<pop_reg<<", %edx"<<endl;
				cout<<"\tpopl\t%ecx"<<endl;
				cout<<"\tpopl\t%eax"<<endl;
			}
			else if(reg.top_r() == "%ecx"){
				cout<<"\tmovl\t"<<pop_reg<<", %ecx"<<endl;
				cout<<"\tpopl\t%edx"<<endl;
				cout<<"\tpopl\t%eax"<<endl;
			}
			else{
				cout<<"\tmovl\t"<<pop_reg<<", "<<reg.top_r()<<endl;
				cout<<"\tpopl\t%ecx"<<endl;
				cout<<"\tpopl\t%edx"<<endl;
				cout<<"\tpopl\t%eax"<<endl;
			}
		}
	}
}

op_unary_astnode::op_unary_astnode(string st, exp_astnode* expr) : exp_astnode()
{
	s = st;
	exp = expr;
	astnode_type = Op_unary_astnode;
	label = exp->label;
}

void op_unary_astnode::print(int blanks)
{
	if(s == "UMINUS"){
		exp->print(0);
		cout<<"\tnegl\t"<<reg.top_r()<<endl;
	}
	if(s == "NOT"){
		exp->print(0);
		cout<<"\tcmpl\t$0, "<<reg.top_r()<<endl;
		cout<<"\tsete\t%al"<<endl;
		cout<<"\tmovzbl\t%al, "<<reg.top_r()<<endl;
	}
	if(s == "PP"){
		exp->print(0);
		cout<<"\taddl\t$"<<1<<", "<<reg.top_r()<<endl;
	}
}

assignE_astnode::assignE_astnode(exp_astnode* expr1, exp_astnode* expr2) : exp_astnode()
{
	exp1 = expr1;
	exp2 = expr2;
	astnode_type = AssignE_astnode;
	label = labeller(exp1->label, exp2->label);

}

void assignE_astnode::print(int blanks)
{
	if(exp1->label < exp2->label && exp1->label < 6){
		reg.swap();
		exp2->print(0);
		string pop_reg = reg.pop();
		exp1->print(0);
		string r = reg.top_r();
		if(exp1->astnode_type == Identifier_astnode){
			cout<<"\tmovl\t"<<pop_reg<<", "<<exp1->offset<<"(%ebp)"<<endl;
		}
		reg.push(pop_reg);
		reg.swap();
	}

	if(exp1->label >= exp2->label && exp2->label < 6){
		exp1->print(0);
		string pop_reg = reg.pop();
		exp2->print(0);
		string r = reg.top_r();
		if(exp1->astnode_type == Identifier_astnode){
			cout<<"\tmovl\t"<<r<<", "<<exp1->offset<<"(%ebp)"<<endl;
		}	
		reg.push(pop_reg);
	}
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
	cout<<"\tmovl\t$"<<i<<", "<<reg.top_r()<<endl;
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
	
}

bool isfloattype(string s)
{
	return (s == "float");
}

bool isinttype(string s)
{
	return (s=="int");
}

void lc_node::print(){
	cout<<"\t.section\t.rodata\n";
	cout<<".LC"<<t<<":"<<endl;
	cout<<"\t.string\t"<<st<<endl;
	cout<<"\t.text\n";
}
