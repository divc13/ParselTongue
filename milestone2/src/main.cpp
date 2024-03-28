#include <bits/stdc++.h>
using namespace std;
#include "parser.tab.h"
#include "include/symtable.hpp"
#include "include/error.hpp"


map<string, int> StringToToken;
map<string, string> TokenNameToString;
map<string, string> opType;


// #include "include/token_map.hpp"

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
extern symbolTable* globTable;
vector<string> lines;

int verbose = 0;
string tokenString = "";
vector<string> verbose_stack;
ofstream VERBOSE;

//<input file> <output file> <output file option template> <verbose_flag> <ast_flag> <ptree_flag> <csv_flag> <md_flag>
int main(int argc, char **argv)
{
	// yydebug = 1;
	argv++, argc--;
	indent_stack.push(0);
	init_token_map();
	init_error();
	init_note();

	inputFile = argv[0];
	string outputFile = argv[1];
	string noExtentionOutputFile = argv[2];
	verbose = atoi(argv[3]);
	int ast_flag = atoi(argv[4]);
	int ptree_flag = atoi(argv[5]);
	int csv_flag = atoi(argv[6]);
	int md_flag = atoi(argv[7]);

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
			return 0;
		}
	}

	if (verbose)
	{
		VERBOSE.open(noExtentionOutputFile + ".debug");
		if (!VERBOSE.is_open()) {
			cout << RED << "Error: Unable to open file " << noExtentionOutputFile << ".debug" << RESET << endl;
			return 0;
		}
	}

	yyin = file;

	int rtnval = yyparse();
	if(rtnval < 0) {
		fclose(yyin);
		return -1;
	}

	if (ptree_flag)
		root->make_dot(noExtentionOutputFile + "_ptree.dot");

	AST_Maker(root);

	// UNCOMMENT BEFORE SUBMISSION //

	// if (ast_flag)
	// 	root->make_dot(noExtentionOutputFile + "_ast.dot");

	
	initTypes();

	rtnval = symTable_Maker(root);
	// if (rtnval < 0) {
	// 	fclose(yyin);
	// 	return -1;
	// } 

	// REMOVE VEFORE SUBMISSION //
	
	if (ast_flag)
		root->make_dot_debug(noExtentionOutputFile + "_ast.dot");

	// if (rtnval < 0) {
	// 	fclose(yyin);
	// 	return -1;
	// } 

	if (csv_flag)
	{
		ofstream CSV(noExtentionOutputFile + ".csv");
		if (!CSV.is_open()) {
			cout << RED << "Error: Unable to open file " << noExtentionOutputFile << ".csv" << RESET << endl;
			return 0;
		}
		
		CSV << "# File Name: " << inputFile << endl;

		globTable->dumpCSV(CSV);
		CSV.close();
	}

	if (md_flag)
	{
		ofstream MD(noExtentionOutputFile + ".md");
		if (!MD.is_open()) {
			cout << RED << "Error: Unable to open file " << noExtentionOutputFile << ".md" << RESET << endl;
			return 0;
		}

    	MD << "<span style=\"font-size: 25px;\">__File Name: " << inputFile << "__</span>\n";
		globTable->dumpMD(MD);
		MD.close();
	}

	fclose(yyin);

	return 0;
}