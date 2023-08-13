#include <cstring>
#include <cstddef>
#include <istream>
#include <iostream>
#include <fstream>

#include "scanner.hh"
#include "parser.tab.hh"
#include "test.hh"

SymTab gst, gstfun, gststruct; 
string filename;
extern std::map<string,abstract_astnode*> ast;

int main(const int argc, const char **argv)
{

  using namespace std;
  fstream in_file;

  in_file.open(argv[1], ios::in);
  // Generate a scanner
  IPL::Scanner scanner(in_file);
  // Generate a Parser, passing the scanner as an argument.
  // Remember %parse-param { Scanner  &scanner  }
  IPL::Parser parser(scanner);
  
  #ifdef YYDEBUG
   parser.set_debug_level(1);
  #endif 

  parser.parse();
for (const auto &entry : gst.Entries)
{
	if (entry.second.varfun == "fun")
	gstfun.Entries.insert({entry.first, entry.second});
}
// create gststruct with struct entries only

for (const auto &entry : gst.Entries)
{
	if (entry.second.varfun == "struct")
	gststruct.Entries.insert({entry.first, entry.second});
}
// start the JSON printing

cout << "{\"globalST\": " << endl;
gst.printgst();
cout << "," << endl;

cout << "  \"structs\": [" << endl;
for (auto it = gststruct.Entries.begin(); it != gststruct.Entries.end(); ++it)

{   cout << "{" << endl;
	cout << "\"name\": " << "\"" << it->first << "\"," << endl;
	cout << "\"localST\": " << endl;
	it->second.symtab->print();
	cout << "}" << endl;
	if (next(it,1) != gststruct.Entries.end()) 
	cout << "," << endl;
}
cout << "]," << endl;
cout << "  \"functions\": [" << endl;

for (auto it = gstfun.Entries.begin(); it != gstfun.Entries.end(); ++it)

{
	cout << "{" << endl;
	cout << "\"name\": " << "\"" << it->first << "\"," << endl;
	cout << "\"localST\": " << endl;
	it->second.symtab->print();
	cout << "," << endl;
	cout << "\"ast\": " << endl;
	ast[it->first]->print(0);
	cout << "}" << endl;
	if (next(it,1) != gstfun.Entries.end()) cout << "," << endl;
	
}
	cout << "]" << endl;
	cout << "}" << endl;

	fclose(stdout);
}

