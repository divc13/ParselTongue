#include <bits/stdc++.h>
using namespace std;

#include "parser.tab.h"
#include "include/token_map.hpp"
#include "include/node.hpp"
#include "include/symtable.hpp"

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
extern symbolTable* currTable;
vector<string> lines;

int main(int argc, char **argv)
{
	// yydebug = 1;
	argv++, argc--;
	indent_stack.push(0);
	init_symbol_name_tables();

	int verbose = atoi(argv[0]);
	inputFile = argv[1];
	string outputFile = argv[2];

	// yyin = fopen(inputFile.c_str(), "r");
	FILE* file = fopen(inputFile.c_str(), "r");

	if (file == nullptr) {
		cout << RED << "Unable to open file: " << inputFile << RESET << endl;
		return 1;
	}

	char *buffer = nullptr;
	size_t bufferSize = 0;
	ssize_t charactersRead;

	// Read lines and store them in a vector
	while ((charactersRead = getline(&buffer, &bufferSize, file)) != -1) {
		lines.push_back(buffer);
	}

	free(buffer);

	if (fseek(file, 0, SEEK_SET) != 0) {
		fclose(file);
		file = fopen(inputFile.c_str(), "r");

		if (file == nullptr) {
			cout << RED << "Unable to open file: " << inputFile << RESET << endl;
			return 1;
		}
	}

	yyin = file;

	int rtnval = yyparse();
	if(rtnval < 0) {
		fclose(yyin);
		return -1;
	}
	if(!verbose) AST_Maker(root);
	root->make_dot(outputFile);
	// ofstream CSV(outputFile);
	// if (!CSV.is_open()) {
	// 	cout << "Error: Unable to open file " << outputFile << endl;
	// 	return 0;
	// }
	// currTable->generateCSV(CSV);
	fclose(yyin);

	return 0;
}