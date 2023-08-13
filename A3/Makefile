CXX   = g++-8
EXE = iplC
CXXDEBUG = -g -Wall
CXXSTD = -std=c++11


.PHONY: all
all: parser lexer 	
	$(CXX) $(CXXDEBUG) $(CXXSTD) -o iplC driver.cpp astnode.cpp symtab.cpp symtab.hh test.hh parser.o scanner.o 

parser: parser.yy scanner.hh
	bison -d -v $<
	$(CXX) $(CXXDEBUG) $(CXXSTD) -c parser.tab.cc -o parser.o 

lexer: scanner.l scanner.hh parser.tab.hh parser.tab.cc	
	flex++ --outfile=scanner.yy.cc  $<
	$(CXX)  $(CXXDEBUG) $(CXXSTD) -c scanner.yy.cc -o scanner.o

clean:
	rm -f *.dot *.h parser.tab.cc scanner.yy.cc position.hh stack.hh *.output *.cc *.o parsar.tab.hh iplC parser.output parser.tab.hh
