all: parser

parser: lex.yy.c parser.tab.c main.cpp parser.tab.h error.hpp token_map.hpp
	g++ main.cpp lex.yy.c parser.tab.c -o parser

lex.yy.c: lexer.l
	flex lexer.l

parser.tab.c: parser.y
	bison -d parser.y

parser.tab.h: parser.y
	bison -d parser.y

clean:
	rm -f parser lex.yy.c parser.tab.c parser.tab.h 

.PHONY: clean