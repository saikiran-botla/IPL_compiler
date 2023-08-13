#ifndef TYPE_HH
#define TYPE_HH
#include<string>
#include<vector>


class datatype
{
    public:
    std::string type;
    int size;
    void print();
};

enum basic_type
{
    INT_TYPE,
    FLOAT_TYPE,
    VOID_TYPE,
    CHAR_TYPE
};

datatype createtype(basic_type type);

class type_specifier_class;
class declaration_class;
class declarator_class;
class declarator_list_class;
class declaration_list_class;
class fun_declarator_class;
class parameter_list_class;
class parameter_declaration_class;



class type_specifier_class{
    public:
    std::string type;
};

class declarator_class{
    public:
    std::string id;
    std::vector<int> index;
    int star = 0;
};

class declaration_class{
    public:
    type_specifier_class* type;
    declarator_list_class* decl_list;
};

class declarator_list_class{
    public:
    std::vector<declarator_class*> decl_list;
};

class declaration_list_class{
    public:
    std::vector<declaration_class*> params;
};

class fun_declarator_class{
    public:
    std::string id;
    parameter_list_class* params;
};

class parameter_list_class{
    public:
    std::vector<parameter_declaration_class*> params;
};

class parameter_declaration_class{
    public:
    type_specifier_class* type;
    declarator_class* decl;
};

#endif