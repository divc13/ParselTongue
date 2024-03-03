all: parser

parser: lex.yy.c parser.tab.c main.cpp parser.tab.h include/error.hpp include/token_map.hpp include/node.hpp node.cpp
	g++ main.cpp lex.yy.c parser.tab.c node.cpp -o parser

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
	rm -f parser lex.yy.c parser.tab.c parser.tab.h action_adder.lex.c action_adder parser.y dot.dot dot.pdf init_parser.tab.h

.PHONY: clean