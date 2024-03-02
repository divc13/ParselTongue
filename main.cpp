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

void yyerror(string s);
int yylex();
extern char *yytext;
extern FILE *yyin;
extern int yylineno;
extern string text;
stack<int> indent_stack;
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
	yydebug = 1;
	int verbose = 0;
	argv++, argc--;
	indent_stack.push(0);
	init_symbol_name_tables();

	vector<string> inputFiles;
	vector<string> outputFiles;

	for (int i = 0; i < argc; i++)
	{
		string arg = argv[i];
		if (arg == "--help" || arg == "-help" || arg == "-h") {
            cout << "Usage: " << argv[0] << " [options]" << endl;
            cout << "Options:" << endl;
            cout << "  --input, -input, -i <file1> <file2> ... : Specify input files" << endl;
            cout << "  --output, -output, -o <file1> <file2> ... : Specify output files" << endl;
            cout << "  --verbose, -verbose, -v To get the Parse Tree instead of the AST at the output files" << endl;
            cout << "  --help, -help, -h : Display the help message" << endl;
            exit(0);
        }
		else if (arg == "--input" || arg == "-input" || arg == "-i")
		{
			i++;
			while (i < argc && argv[i][0] != '-')
			{
				if (!fileExists(argv[i]))
				{
					cerr << "Error: File '" << argv[i] << "' not found" << endl;
					return 1;
				}
				inputFiles.push_back(argv[i]);
				i++;
			}
			i--;
		}
		else if (arg == "--output" || arg == "-output" || arg == "-o")
		{
			i++;
			while (i < argc && argv[i][0] != '-')
			{
				outputFiles.push_back(argv[i]);
				i++;
			}
			i--;
		}
		else if (arg == "--verbose" || arg == "-verbose" || arg == "-v")
		{
			cout << "You will find the parse tree at output files instead of AST" << endl;
			verbose = 1;
		}
		else if (arg[0] == '-')
		{
			cerr << "Error: Unknown option '" << arg << "' found" << endl;
		}
		else
		{
			if (!fileExists(argv[i]))
			{
				cerr << "Error: File '" << argv[i] << "' not found" << endl;
				return 1;
			}
			inputFiles.push_back(argv[i]);
		}
	}

	if (inputFiles.size() == 0)
	{
		cout << YELLOW << "No input file provided, Taking input from stdin\n" << RESET;
		if(outputFiles.size() > 0)
			cout << YELLOW << "Output will be given to " << outputFiles[0] << " \n" << RESET;
		else {
			cout << YELLOW << "Output will be given to " << "output_PT_0.dot" << " \n" << RESET;
			char in;
			cout << "Do you agree to this? [y/n] ";
			cin >> in;
			if (in != 'y' && in != 'Y') {
				return 0; 
			}
		}

		yyin = stdin;
		int rtnval = yyparse();
		fclose(yyin);
		if(rtnval < 0) return 0;
		if(!verbose)
			AST_Maker(root);
		if (outputFiles.size() == 0)
			root->make_dot("output_PT_0.dot");
		else
			root->make_dot(outputFiles[0]);
	}
	else if (inputFiles.size() < outputFiles.size())
	{
		cout << "There are " << outputFiles.size() << " output files and only " << inputFiles.size() << " input files. So output will be given to first " << inputFiles.size() << " files and the rest won't be used" << endl;
		cout << "Do you agree to this? [y/n] ";
		char in;
		cin >> in;
		if (in != 'y' && in != 'Y')
		{
			return 0;
		}
	}
	else if (inputFiles.size() > outputFiles.size())
	{
		cout << "There are " << inputFiles.size() << " input files and only " << outputFiles.size() << " output files. So output files will be randomly named as" << endl; // complete this else if

		for (int i = outputFiles.size(); i < inputFiles.size(); i++)
		{
			string randomOutputFile = "output_PT_" + to_string(i) + ".dot";
			cout << randomOutputFile << endl;
			outputFiles.push_back(randomOutputFile);
		}

		cout << "Do you agree to this? [y/n] ";
		char in;
		cin >> in;
		if (in != 'y' && in != 'Y') {
			return 0; 
		}
	}

	for (int file = 0; file < inputFiles.size(); file++)
	{
		yyin = fopen(inputFiles[file].c_str(), "r");
		int rtnval = yyparse();
		if(rtnval < 0) continue;
		if(!verbose)
			AST_Maker(root);
		root->make_dot(outputFiles[file]);
		fclose(yyin);
	}

	return 0;
}