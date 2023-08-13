%skeleton "lalr1.cc"
%require  "3.0.1"

%defines 
%define api.namespace {IPL}
%define api.parser.class {Parser}

%define parse.trace

%code requires{
   #include "astnode.hh"
   #include "test.hh"
   #include "symtab.hh"
   #include <bits/stdc++.h>

   namespace IPL {
      class Scanner;
   }

  // # ifndef YY_NULLPTR
  // #  if defined __cplusplus && 201103L <= __cplusplus
  // #   define YY_NULLPTR nullptr
  // #  else
  // #   define YY_NULLPTR 0
  // #  endif
  // # endif

}

%printer { std::cerr << $$; } VOID
%printer { std::cerr << $$; } INT
%printer { std::cerr << $$; } FLOAT
%printer { std::cerr << $$; } STRUCT
%printer { std::cerr << $$; } RETURN
%printer { std::cerr << $$; } IDENTIFIER
%printer { std::cerr << $$; } INT_CONSTANT
%printer { std::cerr << $$; } FLOAT_CONSTANT
%printer { std::cerr << $$; } IF
%printer { std::cerr << $$; } ELSE
%printer { std::cerr << $$; } OR_OP
%printer { std::cerr << $$; } AND_OP
%printer { std::cerr << $$; } EQ_OP
%printer { std::cerr << $$; } NE_OP
%printer { std::cerr << $$; } LE_OP
%printer { std::cerr << $$; } GE_OP
%printer { std::cerr << $$; } PTR_OP
%printer { std::cerr << $$; } INC_OP
%printer { std::cerr << $$; } STRING_LITERAL
%printer { std::cerr << $$; } FOR
%printer { std::cerr << $$; } WHILE
%printer { std::cerr << $$; } OTHERS


%parse-param { Scanner  &scanner  }
%locations
%code{
   #include <iostream>
   #include <cstdlib>
   #include <fstream>
   #include <string>
   #include <vector>
   #include <map>
   #include <bits/stdc++.h>
   
   
   #include "scanner.hh"
   #include "test.hh"
   #include "symtab.hh"
   std::map<string, abstract_astnode*> ast;
   std::map<string, lc_node*> lc_map;
   std::string returnftype;

   std::string fname,sname;
   SymTabEntry* currfunc;
   extern SymTab gst;
   SymTab* st;
   std::map<string,parameter_list_class*> par;


   int nodeCount = 0;

   int offset=0;

#undef yylex
#define yylex IPL::Parser::scanner.yylex

}




%define api.value.type variant
%define parse.assert

%start translation_unit

%token <std::string> VOID
%token <std::string> INT
%token <std::string> FLOAT
%token <std::string> STRUCT
%token <std::string> RETURN
%token <std::string> IDENTIFIER
%token <std::string> INT_CONSTANT
%token <std::string> FLOAT_CONSTANT
%token <std::string> IF
%token <std::string> ELSE
%token <std::string> OR_OP
%token <std::string> AND_OP
%token <std::string> EQ_OP
%token <std::string> NE_OP
%token <std::string> LE_OP
%token <std::string> GE_OP
%token <std::string> PTR_OP
%token <std::string> INC_OP
%token <std::string> STRING_LITERAL
%token <std::string> FOR
%token <std::string> WHILE
%token <std::string> OTHERS
%token ',' ':' '(' ')' '+' '/' '{' '=' '}' '*' '-' '!' '&' '.' '\n' '[' ']' '<' '>' ';'


%nterm <abstract_astnode*> translation_unit;
%nterm <abstract_astnode*> struct_specifier;
%nterm <abstract_astnode*> function_definition;
%nterm <type_specifier_class*> type_specifier;
%nterm <fun_declarator_class*> fun_declarator;
%nterm <parameter_list_class*> parameter_list;
%nterm <parameter_declaration_class*> parameter_declaration;
%nterm <declarator_class*> declarator;
%nterm <abstract_astnode*> compound_statement;
%nterm <seq_astnode*> statement_list;
%nterm <statement_astnode*> statement;
%nterm <assignE_astnode*> assignment_expression;
%nterm <assignS_astnode*> assignment_statement;
%nterm <proccall_astnode*> procedure_call;
%nterm <exp_astnode*> expression;
%nterm <exp_astnode*> logical_and_expression;
%nterm <exp_astnode*> equality_expression;
%nterm <exp_astnode*> relational_expression;
%nterm <exp_astnode*> additive_expression;
%nterm <exp_astnode*> unary_expression;
%nterm <exp_astnode*> multiplicative_expression;
%nterm <statement_astnode*> selection_statement;
%nterm <exp_astnode*> postfix_expression;
%nterm <std::string> unary_operator;
%nterm <statement_astnode*> iteration_statement;
%nterm <declaration_list_class*> declaration_list;
%nterm <declaration_class*> declaration;
%nterm <declarator_list_class*> declarator_list;
%nterm <declarator_class*> declarator_arr;
%nterm <exp_astnode*> primary_expression;
%nterm <vector<exp_astnode*>> expression_list;



%%


translation_unit:
     struct_specifier 
     {
     
     }
     |  function_definition 
     {
          ast.insert({fname,$1});
     }
     | translation_unit struct_specifier 
     {
     }
     | translation_unit function_definition 
     {
          ast.insert({fname,$2});
     }
     ;

struct_specifier:
          
       STRUCT IDENTIFIER '{' declaration_list '}' ';'
     {
          st =  new SymTab();
          std::vector<declaration_class*> b = $4->params;
          
          int goffset=0;

          for(int i=0;i<(int)b.size();i++){

               std::string typ = b[i]->type->type;
               declarator_list_class* decl_list = b[i]->decl_list;
               vector<declarator_class*> pp = decl_list->decl_list;

               for(int j=0;j<(int)pp.size();j++){
                    SymTabEntry* t1 = new SymTabEntry();
                    t1->returntype = typ;
                    t1->size = 0;
                    t1->varfun = "var";
                    string id = pp[j]->id;
                    

                    int offset;
                    if(typ == "int"){
                         offset=4;
                         for(int k=0;k<(int)(pp[j]->index).size();k++){
                              offset*=pp[j]->index[k];
                         }
                    }
                    if(typ == "float"){
                         offset=4;
                         for(int k=0;k<(int)(pp[j]->index).size();k++){
                              offset*=pp[j]->index[k];
                         }
                    }
                    if(typ == "char"){
                         offset=4;
                         for(int k=0;k<(int)(pp[j]->index).size();k++){
                              offset*=pp[j]->index[k];
                         }
                    }
                    if(typ== "void"){
                         offset=4;
                         for(int k=0;k<(int)(pp[j]->index).size();k++){
                              offset*=pp[j]->index[k];
                         }
                    }
                    if(typ.find("struct")!=string::npos){
                         if(pp[j]->star>0){
                              offset=4;

                         }
                         else{
                              offset=gst.Entries[typ].size;
                         }
                         
                         for(int k=0;k<(int)(pp[j]->index).size();k++){
                              offset*=pp[j]->index[k];
                         }
                    }

                    t1->offset=goffset;
                    goffset+=offset;
                    t1->size=offset;
                    t1->type = typ;

                    for(int l=0;l<(int)pp[j]->star;l++){
                         t1->returntype = t1->returntype + "*";
                    }
                    for(int m=0;m<(int)pp[j]->index.size();m++){
                         t1->returntype = t1->returntype+"["+ to_string(pp[j]->index[m]) +"]";
                    }
                    t1->scope = "local";
                    st->Entries.insert({id, *t1});
               }
          }

          sname = "struct " + $2;
          fname = "";

          SymTabEntry* t = new SymTabEntry();
          t->returntype = "-";
          t->size = goffset;
          t->scope = "global";
          t->varfun = "struct";
          t->symtab = st;
          t->offset = 0;

          gst.Entries.insert({sname, *t});

     }
     ;

function_definition:
     type_specifier fun_declarator
     {
          st =  new SymTab();
          std::string return_type = $1->type;
          sname = "";
          fname = $2->id;

          SymTabEntry* t = new SymTabEntry();
          t->returntype = return_type;
          t->size = 0;
          t->varfun = "fun";
          t->scope = "global";
          t->symtab = st;

          gst.Entries.insert({$2->id, *t});

          int goffset=12;
          vector<parameter_declaration_class*> params=$2->params->params;
          for(int i=(int)params.size() -1;i>=0;i--){
               SymTabEntry* t1 = new SymTabEntry();
               //declarator_class* decl = params[i]->decl;
               t1->returntype = params[i]->type->type;
               t1->type = params[i]->type->type;
               t1->size = 0;
               t1->varfun = "var";
               string id = params[i]->decl->id;
               int offset;
               if(params[i]->type->type == "int"){
                    offset=4;
                    for(int k=0;k<(int)(params[i]->decl->index).size();k++){
                         offset*=params[i]->decl->index[k];
                    }
               }
               if(params[i]->type->type == "float"){
                    offset=4;
                    for(int k=0;k<(int)(params[i]->decl->index).size();k++){
                         offset*=params[i]->decl->index[k];
                    }
               }
               if(params[i]->type->type == "char"){
                    offset=4;
                    for(int k=0;k<(int)(params[i]->decl->index).size();k++){
                         offset*=params[i]->decl->index[k];
                    }
               }
               if(params[i]->type->type == "void"){
                    offset=4;
                    for(int k=0;k<(int)(params[i]->decl->index).size();k++){
                         offset*=params[i]->decl->index[k];
                    }
               }

               if(params[i]->type->type.find("struct")!=string::npos){
                    if(params[i]->decl->star>0){
                         offset=4;
                    }
                    else{
                         offset=gst.Entries[params[i]->type->type].size;
                    }
                    for(int k=0;k<(int)(params[i]->decl->index).size();k++){
                         offset*=params[i]->decl->index[k];
                    }
               }
               for(int l=0;l<(int)params[i]->decl->star;l++){
                    t1->returntype = t1->returntype + "*";
               }
               for(int m=0;m<(int)params[i]->decl->index.size();m++){
                    t1->returntype = t1->returntype+"["+ to_string(params[i]->decl->index[m]) +"]";
               }

               t1->offset=goffset;
               t1->size=offset;
               goffset+=offset;
               t1->scope = "param";

               st->Entries.insert({id, *t1});
               returnftype = $1->type;
          }
     }
      compound_statement 
     {
          $$ = $4;
     }
     ;

type_specifier:
     VOID
     {
          $$ = new type_specifier_class();
          $$->type = "void";
     }
     | INT
     {
          $$ = new type_specifier_class();
          $$->type = "int";
     }
     | FLOAT
     {
          $$ = new type_specifier_class();
          $$->type = "float";
     }
     | STRUCT IDENTIFIER
     {
          $$ = new type_specifier_class();
          $$->type = "struct "+$2;
     }
     ;

fun_declarator: 
     IDENTIFIER '(' parameter_list ')'
     {
          fname = $1;
          $$ = new fun_declarator_class();
          $$->params = $3;
          $$->id = $1;

          par[$1] = $3;
     }  
     | IDENTIFIER '(' ')' 
     {
          fname = $1;
          $$ = new fun_declarator_class();
          $$->params = new parameter_list_class();
          $$->id = $1;
     }
     ;

parameter_list:
     parameter_declaration 
     {
          $$ = new parameter_list_class();
          $$->params.push_back($1);
     }
     | parameter_list ',' parameter_declaration  
     {
          $$ = $1;
          $$->params.push_back($3);
     }
     ;

parameter_declaration:
    type_specifier declarator  
    {
         $$ = new parameter_declaration_class();
         $$->type = $1;
         $$->decl = $2;
    }
    ;

declarator_arr:
     IDENTIFIER
     { 
          $$ = new declarator_class();
          $$->id = $1;
     }
     | declarator_arr '[' INT_CONSTANT ']'
     { 
          $$ = $1;
          $$->index.push_back(stoi($3));
     }
     ;


declarator :
     declarator_arr
     {
          $$ = $1;
     }
     | '*' declarator
     {
          $$ = $2;
          $$->star = $$->star + 1;
     }
     ;

declaration_list: 
     declaration 
     {
          $$ = new declaration_list_class();
          $$->params.push_back($1);
     }
     | declaration_list declaration 
     {
          $$ = $1;
          $$->params.push_back($2);
     }
     ;

declaration:
     type_specifier declarator_list ';' 
     {
          $$ = new declaration_class();
          $$->type = $1;
          $$->decl_list = $2;
     }
     ;

declarator_list:
     declarator 
     {
          $$ = new declarator_list_class();
          $$->decl_list.push_back($1);
     }
     | declarator_list ',' declarator 
     {
          $$ = $1;
          $$->decl_list.push_back($3);
     }
     ;

compound_statement: 
     '{' '}' 
     {
          $$ = new seq_astnode();
     }
     | '{' statement_list '}' 
     {
          $$ = $2;
     }
     | '{' declaration_list
     {
          std::vector<declaration_class*> b = $2->params;
          int goffset=0;
          for(int i=0;i<(int)b.size();i++){

               std::string typ = b[i]->type->type;
               declarator_list_class* decl_list = b[i]->decl_list;
               vector<declarator_class*> pp = decl_list->decl_list;

               
               for(int j=0;j<(int)pp.size();j++){
                    SymTabEntry* t1 = new SymTabEntry();
                    t1->returntype = typ;
                    t1->size = 0;
                    t1->varfun = "var";
                    string id = pp[j]->id;

                    int offset;
                    if(typ == "int"){
                         offset=-4;
                         for(int k=0;k<(int)(pp[j]->index).size();k++){
                              offset*=pp[j]->index[k];
                         }
                    }
                    if(typ == "float"){
                         offset=-4;
                         for(int k=0;k<(int)(pp[j]->index).size();k++){
                              offset*=pp[j]->index[k];
                         }
                    }
                    if(typ == "char"){
                         offset=-4;
                         for(int k=0;k<(int)(pp[j]->index).size();k++){
                              offset*=pp[j]->index[k];
                         }
                    }
                    if(typ== "void"){
                         offset=-4;
                         for(int k=0;k<(int)(pp[j]->index).size();k++){
                              offset*=pp[j]->index[k];
                         }
                    }


                    if(typ.find("struct")!=string::npos){
                         if(pp[j]->star>0){
                              offset=-4;
                              for(int k=0;k<(int)(pp[j]->index).size();k++){
                                   offset*=pp[j]->index[k];
                              }
                         }
                         else{
                              offset=-1*gst.Entries[typ].size;
                              for(int k=0;k<(int)(pp[j]->index).size();k++){
                                   offset*=pp[j]->index[k];
                              }
                         }
                    }
                    //t1->offset=goffset+offset;
                    //goffset+=offset;
                    //t1->size=-offset;
                    //t1->scope = "local";
                    //t1->type = typ;

                    st->Entries.insert({id, *t1});
                    st->Entries[id].scope = "local";
                    st->Entries[id].offset=goffset+offset;
                    goffset+=offset;
                    st->Entries[id].size=-offset;
                    st->Entries[id].scope = "local";
                    st->Entries[id].type = typ;
                    st->Entries[id].varfun = "var";
                    st->Entries[id].returntype = typ;
                    st->Entries[id].type = typ;
                    
                    for(int l=0;l<(int)pp[j]->star;l++){
                         st->Entries[id].returntype = st->Entries[id].returntype + "*";
                    }
                    for(int m=0;m<(int)pp[j]->index.size();m++){
                         st->Entries[id].returntype = st->Entries[id].returntype +"["+ to_string(pp[j]->index[m]) +"]";
                    }
               }
          }
     }
      statement_list '}' 
     {
          $$ = $4;
     }
     | '{' declaration_list '}'
     {
          $$ = new seq_astnode();
          std::vector<declaration_class*> b = $2->params;
          int goffset=0;
          for(int i=0;i<(int)b.size();i++){

               std::string typ = b[i]->type->type;
               declarator_list_class* decl_list = b[i]->decl_list;
               vector<declarator_class*> pp = decl_list->decl_list;

               for(int j=0;j<(int)pp.size();j++){
                    SymTabEntry* t1 = new SymTabEntry();
                    t1->returntype = typ;
                    t1->size = 0;
                    t1->varfun = "var";
                    string id = pp[j]->id;

                    int offset;
                    if(typ == "int"){
                         offset=-4;
                         for(int k=0;k<(int)(pp[j]->index).size();k++){
                              offset*=pp[j]->index[k];
                         }
                    }
                    if(typ == "float"){
                         offset=-4;
                         for(int k=0;k<(int)(pp[j]->index).size();k++){
                              offset*=pp[j]->index[k];
                         }
                    }
                    if(typ == "char"){
                         offset=-4;
                         for(int k=0;k<(int)(pp[j]->index).size();k++){
                              offset*=pp[j]->index[k];
                         }
                    }
                    if(typ== "void"){
                         offset=-4;
                         for(int k=0;k<(int)(pp[j]->index).size();k++){
                              offset*=pp[j]->index[k];
                         }
                    }


                    if(typ.find("struct")!=string::npos){
                         if(pp[j]->star>0){
                              offset=-4;
                              for(int k=0;k<(int)(pp[j]->index).size();k++){
                                   offset*=pp[j]->index[k];
                              }
                         }
                         else{
                              offset=-1*gst.Entries[typ].size;
                              for(int k=0;k<(int)(pp[j]->index).size();k++){
                                   offset*=pp[j]->index[k];
                              }
                         }
                    }
                    //t1->offset=goffset+offset;
                    //goffset+=offset;
                    //t1->size=-offset;
                    //t1->scope = "local";
                    //t1->type = typ;

                    st->Entries.insert({id, *t1});
                    st->Entries[id].scope = "local";
                    st->Entries[id].offset=goffset+offset;
                    goffset+=offset;
                    st->Entries[id].size=-offset;
                    st->Entries[id].scope = "local";
                    st->Entries[id].type = typ;
                    st->Entries[id].varfun = "var";
                    st->Entries[id].returntype = typ;
                    st->Entries[id].type = typ;
                    
                    for(int l=0;l<(int)pp[j]->star;l++){
                         st->Entries[id].returntype = st->Entries[id].returntype + "*";
                    }
                    for(int m=0;m<(int)pp[j]->index.size();m++){
                         st->Entries[id].returntype = st->Entries[id].returntype +"["+ to_string(pp[j]->index[m]) +"]";
                    }
               }
          }
     }
     ;

statement_list:
     statement
     {
          $$ = new seq_astnode();
          $$->pushback($1);
     }   
     | statement_list statement
     {
          $$ = $1;
          $$->pushback($2);
     }
     ;

statement:
     ';'  
     {
          $$ = new empty_astnode();
     }
     |  '{' statement_list '}'   
     {
          $$ = $2;
     }   
     | selection_statement   
     {
          $$ = $1;
     }   
     | iteration_statement   
     {
          $$ = $1;
     }    
     | assignment_statement   
     {
          $$ = $1;
     }    
     | procedure_call      
     {
          $$ = $1;
     }       
     | RETURN expression ';' 
     {
          string s = $2->data_type;
          if(returnftype == "int" && s == "float"){
               exp_astnode* t = new op_unary_astnode("TO_INT",$2);
               $$ = new return_astnode(t);
          }
          else if(returnftype == "float" && s == "int"){
               exp_astnode* t = new op_unary_astnode("TO_FLOAT",$2);
               $$ = new return_astnode(t);
          }
          else{
               $$ = new return_astnode($2);
          }
     }
     ;

assignment_expression:
     unary_expression '=' expression 
     {
          exp_astnode* l = $1;
          exp_astnode* r = $3;

          if(isinttype(l->data_type) && isfloattype(r->data_type))
          {
               r = new op_unary_astnode("TO_INT",r);
          }

          if(isinttype(r->data_type) && isfloattype(l->data_type))
          {
               r = new op_unary_astnode("TO_FLOAT",r);
          }
          $$ = new assignE_astnode(l,r);
          $$->data_type = l->data_type;
     }
     ;

assignment_statement: 
    assignment_expression ';' 
    {
         $$ = new assignS_astnode($1->exp1, $1->exp2);
    }
    ;

procedure_call:
     IDENTIFIER '(' ')' ';'
     {
          $$ = new proccall_astnode(new identifier_astnode($1));
     }
     | IDENTIFIER '(' expression_list ')' ';' 
     {
          $$ = new proccall_astnode(new identifier_astnode($1));
          if($1 == "printf" || $1== "scanf"){
               $$->expressions = $3;
               int t = 0;
               for(auto it = lc_map.begin(); it!= lc_map.end(); ++it)
               {
                    t++;
                    //cout<<t<<"hello"<<endl;
               }
               lc_node* lc_n = new lc_node();
               lc_n->t = t;
               lc_n->st = $$->expressions[0]->s;
               lc_n->globl = fname;
               lc_n->typ = fname;
               lc_map.insert({".LC"+to_string(t), lc_n});
               $$->expressions[0]->s = ".LC"+to_string(t);
               //cout<<$$->expressions[0]->s<<"hello"<<endl;
          }
          else{
               vector<parameter_declaration_class*> list = par[$1]->params;

               vector<exp_astnode*> expre;
               for(int i=0;i<(int)$3.size();i++)
               {
                    exp_astnode* t = $3[i];
                    parameter_declaration_class* k = list[i];
                    std::string partype = k->type->type;
                    std::string exptype = t->data_type;

                    if(exptype == "int" && partype == "float")
                    {
                         t = new op_unary_astnode("TO_FLOAT",t);
                         expre.push_back(t);
                    }
                    if(exptype == "float" && partype == "int")
                    {
                         t = new op_unary_astnode("TO_INT",t);
                         expre.push_back(t);
                    }
               }
               $$->expressions = expre;
          }  
     }
     ;

expression:
     logical_and_expression 
     {
          $$ = $1;
     }
     | expression OR_OP logical_and_expression 
     {
          exp_astnode* l = $1;
          exp_astnode* r = $3;
          $$ = new op_binary_astnode("OR_OP",l,r);
          $$->data_type = "int";
     }
	
     ;

logical_and_expression:
     equality_expression 
     {
          $$ = $1;
     }
     | logical_and_expression AND_OP equality_expression
     {
          exp_astnode* l = $1;
          exp_astnode* r = $3;
          $$ = new op_binary_astnode("AND_OP",l,r);
          $$->data_type = "int";
     } 
     ;

equality_expression :
     relational_expression 
     {
          $$ = $1;
     }
     | equality_expression EQ_OP relational_expression 
     {
          exp_astnode* l = $1;
          exp_astnode* r = $3;
          
          if(isfloattype(l->data_type) || isfloattype(r->data_type)) 
          {
               if(isinttype(l->data_type))
               {
                    l = new op_unary_astnode("TO_FLOAT",l);
               }
               if(isinttype(r->data_type))
               {
                    r = new op_unary_astnode("EQ_OP_FLOAT",r);
               }
               $$ = new op_binary_astnode("EQ_OP_FLOAT", l, r);
               $$->data_type = "int";
          }
          else
          {
               $$ = new op_binary_astnode("EQ_OP_INT", l, r);
               $$->data_type = "int";
          }
     }
     | equality_expression NE_OP relational_expression
     {
          exp_astnode* l = $1;
          exp_astnode* r = $3;
          
          if(isfloattype(l->data_type) || isfloattype(r->data_type)) 
          {
               if(isinttype(l->data_type))
               {
                    l = new op_unary_astnode("TO_FLOAT",l);
               }
               if(isinttype(r->data_type))
               {
                    r = new op_unary_astnode("TO_FLOAT",r);
               }
               $$ = new op_binary_astnode("NE_OP_FLOAT", l, r);
               $$->data_type = "int";
          }
          else
          {
               $$ = new op_binary_astnode("NE_OP_INT", l, r);
               $$->data_type = "int";
          }
     }
     ;

relational_expression:
     additive_expression 
     {
          $$ = $1;
     }
     | relational_expression '<' additive_expression 
     {
          exp_astnode* l = $1;
          exp_astnode* r = $3;
          
          if(isfloattype(l->data_type) || isfloattype(r->data_type)) 
          {
               if(isinttype(l->data_type))
               {
                    l = new op_unary_astnode("TO_FLOAT",l);
               }
               if(isinttype(r->data_type))
               {
                    r = new op_unary_astnode("TO_FLOAT",r);
               }
               $$ = new op_binary_astnode("LT_OP_FLOAT", l, r);
               $$->data_type = "int";
          }
          else
          {
               $$ = new op_binary_astnode("LT_OP_INT", l, r);
               $$->data_type = "int";
          }
     }
     | relational_expression '>' additive_expression  
     {
          exp_astnode* l = $1;
          exp_astnode* r = $3;
          
          if(isfloattype(l->data_type) || isfloattype(r->data_type)) 
          {
               if(isinttype(l->data_type))
               {
                    l = new op_unary_astnode("TO_FLOAT",l);
               }
               if(isinttype(r->data_type))
               {
                    r = new op_unary_astnode("TO_FLOAT",r);
               }
               $$ = new op_binary_astnode("GT_OP_FLOAT", l, r);
               $$->data_type = "int";
          }
          else
          {
               $$ = new op_binary_astnode("GT_OP_INT", l, r);
               $$->data_type = "int";
          }
     }
     | relational_expression LE_OP additive_expression
     {
          exp_astnode* l = $1;
          exp_astnode* r = $3;
          
          if(isfloattype(l->data_type) || isfloattype(r->data_type)) 
          {
               if(isinttype(l->data_type))
               {
                    l = new op_unary_astnode("TO_FLOAT",l);
               }
               if(isinttype(r->data_type))
               {
                    r = new op_unary_astnode("TO_FLOAT",r);
               }
               $$ = new op_binary_astnode("LE_OP_FLOAT", l, r);
               $$->data_type = "int";
          }
          else
          {
               $$ = new op_binary_astnode("LE_OP_INT", l, r);
               $$->data_type = "int";
          }
     }  
     | relational_expression GE_OP additive_expression 
     {
          exp_astnode* l = $1;
          exp_astnode* r = $3;
          
          if(isfloattype(l->data_type) || isfloattype(r->data_type)) 
          {
               if(isinttype(l->data_type))
               {
                    l = new op_unary_astnode("TO_FLOAT",l);
               }
               if(isinttype(r->data_type))
               {
                    r = new op_unary_astnode("TO_FLOAT",r);
               }
               $$ = new op_binary_astnode("GE_OP_FLOAT", l, r);
               $$->data_type = "int";
          }
          else
          {
               $$ = new op_binary_astnode("GE_OP_INT", l, r);
               $$->data_type = "int";
          }
     }
     ;

additive_expression:
     multiplicative_expression 
     {
          $$ = $1;
     }
     | additive_expression '+' multiplicative_expression
     {
          exp_astnode* l = $1;
          exp_astnode* r = $3;
          
          if(isfloattype(l->data_type) || isfloattype(r->data_type)) 
          {
               if(isinttype(l->data_type))
               {
                    l = new op_unary_astnode("TO_FLOAT",l);
               }
               if(isinttype(r->data_type))
               {
                    r = new op_unary_astnode("TO_FLOAT",r);
               }
               $$ = new op_binary_astnode("PLUS_FLOAT", l, r);
               $$->data_type = "float";
          }
          else
          {
               $$ = new op_binary_astnode("PLUS_INT", l, r);
               $$->data_type = "int";
          }
     }
     | additive_expression '-' multiplicative_expression  
     {
          exp_astnode* l = $1;
          exp_astnode* r = $3;
          
          if(isfloattype(l->data_type) || isfloattype(r->data_type)) 
          {
               if(isinttype(l->data_type))
               {
                    l = new op_unary_astnode("TO_FLOAT",l);
               }
               if(isinttype(r->data_type))
               {
                    r = new op_unary_astnode("TO_FLOAT",r);
               }
               $$ = new op_binary_astnode("MINUS_FLOAT", l, r);
               $$->data_type = "float";
          }
          else
          {
               $$ = new op_binary_astnode("MINUS_INT", l, r);
               $$->data_type = "int";
          }
     }
     ;

unary_expression:
     postfix_expression
     {
          $$ = $1;
     }
     | unary_operator unary_expression
     {
          $$ = new op_unary_astnode($1, $2);
          if($1 == "NOT"){
               $$->data_type = "int";
          }
          else{
               $$->data_type = $2->data_type;
          }
     }
     ;

multiplicative_expression: 
     unary_expression
     {
          $$ = $1;
     }
     | multiplicative_expression '*' unary_expression
     {
          exp_astnode *l = $1;
          exp_astnode *r = $3;
          
          if(isfloattype(l->data_type) || isfloattype(r->data_type)) 
          {
               if(isinttype(l->data_type))
               {
                    l = new op_unary_astnode("TO_FLOAT",l);
               }
               if(isinttype(r->data_type))
               {
                    r = new op_unary_astnode("TO_FLOAT",r);
               }
               $$ = new op_binary_astnode("MULT_FLOAT", l, r);
               $$->data_type = "float";
          }
          else
          {
               $$ = new op_binary_astnode("MULT_INT", l, r);
               $$->data_type = "int";
          }
     }
     | multiplicative_expression '/' unary_expression
     {
          exp_astnode *l = $1;
          exp_astnode *r = $3;

          if(isfloattype(l->data_type) || isfloattype(r->data_type)) 
          {
               if(isinttype(l->data_type))
               {
                    l = new op_unary_astnode("TO_FLOAT",l);
               }
               if(isinttype(r->data_type))
               {
                    r = new op_unary_astnode("TO_FLOAT",r);
               }
               $$ = new op_binary_astnode("DIV_FLOAT", l, r);
               $$->data_type = "float";
          }
          else
          {
               $$ = new op_binary_astnode("DIV_INT", l, r);
               $$->data_type = "int";
          }
     }
     ;
  


postfix_expression: 
     primary_expression
     {
          $$ = $1;
     }
     | postfix_expression '[' expression ']'
     {
          exp_astnode *l = $1;
          exp_astnode *r = $3;
          $$ = new arrayref_astnode(l, r);
          $$->data_type = $1->data_type;
     }
     | IDENTIFIER '(' ')'
     {
          funcall_astnode* f = new funcall_astnode();
          f->setname($1);
          $$ = f;
     }
     | IDENTIFIER '(' expression_list ')'
     {
          funcall_astnode* f = new funcall_astnode();
          f->setname($1);
          f->expressions = $3;
          $$ = f;
     }
     | postfix_expression '.' IDENTIFIER
     {
          identifier_astnode* i = new identifier_astnode($3);
          member_astnode* m = new member_astnode($1, i);
          $$ = m;
     }
     | postfix_expression PTR_OP IDENTIFIER
     {
          identifier_astnode* i = new identifier_astnode($3);
          arrow_astnode* m = new arrow_astnode($1, i); 
          $$ = m;   
     }
     | postfix_expression INC_OP
     {
          $$ = new op_unary_astnode("PP", $1);
          $$->data_type = $$->data_type;
     }
     ;

primary_expression:
     IDENTIFIER
     {
          $$ = new identifier_astnode($1);
          SymTabEntry* t = st->search($1);
          if(t == NULL)
          {
               cout<<" Error: "<<endl;
          }
          $$->data_type = t->type;
          $$->offset = t->offset;
     }
     | INT_CONSTANT
     {
          $$ = new intconst_astnode(stoi($1));
          $$->data_type = "int";
     }
     | FLOAT_CONSTANT
     {
          $$ = new floatconst_astnode(stof($1));
          $$->data_type = "float";
     }
     | STRING_LITERAL
     {
          $$ = new stringconst_astnode($1);
          $$->data_type = "string";
     }
     | '(' expression ')'
     {
          $$ = $2;
     }
     ;

expression_list: 
     expression
     {
          $$.push_back($1);
     }
     | expression_list ',' expression
     {
          $$ = $1;
          $$.push_back($3);
     }
     ;

unary_operator:
     '-'
     {
          $$ = "UMINUS";
     }
     | '!'
     {
          $$ = "NOT";
     }
     | '&'
     {
          $$ = "ADDRESS";
     }
     | '*'
     {
          $$ = "DEREF";
     }
     ;

selection_statement: 
     IF '(' expression ')' statement ELSE statement
     {
          $$ = new if_astnode($3, $5, $7);
     } 
    ;

iteration_statement: 
     WHILE '(' expression ')' statement
     {
          $$ = new while_astnode($3, $5);
     }
     | FOR '(' assignment_expression ';' expression ';' assignment_expression ')' statement
     {
          $$ = new for_astnode($3, $5, $7, $9);
     }
     ;

%%
void IPL::Parser::error( const location_type &l, const std::string &err_message )
{
   std::cerr << "Error: " << err_message << " at " << l << "\n";
}


