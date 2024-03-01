#include <bits/stdc++.h>
using namespace std;

#include "parser.tab.h"
#include "token_map.hpp"
#include "node.hpp"

#define RED "\033[1;31m"
#define RESET "\033[0m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN "\033[1;36m"

void yyerror (string s);
int yylex();
extern char* yytext;
extern FILE* yyin;
extern int yylineno;
extern string text;
stack <int> indent_stack;
extern TreeNode* root;


int main(int argc, char** argv)
{
	// yydebug = 1;
	++argv, --argc;
	if ( argc > 0 ){
		yyin = fopen( argv[0], "r" );
	}
	else{
		cout << YELLOW << "No input file provided, Taking input from stdin.....\n" << RESET;
		yyin = stdin;
	}
	indent_stack.push(0);
	init_symbol_name_tables();
	yyparse( );
		
	fclose(yyin);
	root->make_dot();
	return 0;
}