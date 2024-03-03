#include <bits/stdc++.h>
using namespace std;

#include "parser.tab.h"
#include "include/token_map.hpp"
#include "include/node.hpp"

#define RED "\033[1;31m"
#define RESET "\033[0m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN "\033[1;36m"

void yyerror(string s);
int yylex();
extern char *yytext;
extern FILE *yyin;
extern int yylineno;
extern string text;
stack<int> indent_stack;
string inputFile = "";
extern TreeNode *root;

bool fileExists(const string &filename)
{
	ifstream file(filename);
	bool exists = file.good();
	file.close();
	return exists;
}

int main(int argc, char **argv)
{
	// yydebug = 1;
	argv++, argc--;
	indent_stack.push(0);
	init_symbol_name_tables();

	int verbose = atoi(argv[0]);
	inputFile = argv[1];
	string outputFile = argv[2];

	yyin = fopen(inputFile.c_str(), "r");
	int rtnval = yyparse();
	if(rtnval < 0) {
		fclose(yyin);
		return -1;
	}
	if(!verbose)
		AST_Maker(root);
	root->make_dot(outputFile);
	fclose(yyin);

	return 0;
}