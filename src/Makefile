all: runner 

runner: lex.yy.c parser.tab.c main.cpp parser.tab.h error.cpp output.cpp init.cpp include/parsing_error.hpp x86-64.cpp include/error.hpp threeAC.cpp include/node.hpp node.cpp include/error.hpp symTable.cpp include/symtable.hpp
	g++ main.cpp lex.yy.c parser.tab.c node.cpp symTable.cpp error.cpp output.cpp init.cpp threeAC.cpp x86-64.cpp -o runner

lex.yy.c: lexer.l
	flex lexer.l

parser.tab.c: parser.y
	bison -d parser.y

parser.tab.h: parser.y
	bison -d parser.y

parser.y: action_adder init_parser.y
	./action_adder init_parser.y

action_adder: action_adder.lex.c
	g++ -o action_adder action_adder.lex.c

action_adder.lex.c: action_adder.l
	flex -o action_adder.lex.c action_adder.l 

clean:
	rm -f compiler lex.yy.c parser.tab.c parser.tab.h action_adder.lex.c action_adder parser.y dot.dot dot.pdf init_parser.tab.h

.PHONY: clean