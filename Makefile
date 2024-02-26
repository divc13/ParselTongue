# Makefile for compiling lexer.l and parser.y

# Compiler
CC=g++

# Target executable
TARGET=parser

# Source files
LEX_SRC=lexer.l
YACC_SRC=parser.y

# Generated files
LEX_OUT=lex.yy.c
YACC_OUT=parser.tab.c
YACC_HEADER=parser.tab.h
PARSE_OUT=parser.output

all: $(TARGET)

$(TARGET): $(LEX_OUT) $(YACC_OUT)
	$(CC) $(LEX_OUT) $(YACC_OUT) -o $(TARGET)

$(LEX_OUT): $(LEX_SRC)
	flex $(LEX_SRC)

$(YACC_OUT): $(YACC_SRC)
	bison -d $(YACC_SRC)

clean:
	rm -f $(TARGET) $(LEX_OUT) $(YACC_OUT) $(YACC_HEADER) $(PARSE_OUT)

.PHONY: clean
