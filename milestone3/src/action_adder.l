%option noyywrap

%{

#include <bits/stdc++.h>
using namespace std;
ofstream OUT("parser.y");
string builder = "";
string header = "";

// #define YY_USER_ACTION cout << builder << endl;

void action_printer(string builder)
{
	int itr = 0;

	OUT << "\n\n\t\t{\n";

	OUT << "\t\t\tTreeNode* temp = new TreeNode(\"" << header << "\");\n";
	OUT << "\t\t\t$$ = temp;\n";

	if (header.compare("file") == 0)
	{
		OUT << "\t\t\troot = temp;\n";
	}

	while(itr < builder.length())
	{
		//terminal
		if (builder[itr] == 't')
		{
			OUT << "\t\t\ttemp = new TreeNode(($" << itr + 1 << ").name, ($" << itr + 1 << ").lineno, ($" << itr + 1 << ").colno, ($" << itr + 1 << ").type);\n";
			OUT << "\t\t\t(($$) -> children).push_back(temp); \n";
		}

		//non terminal
		else if (builder[itr] == 'n')
		{
			OUT << "\t\t\t(($$) -> children).push_back($" << itr + 1 << ");\n";
		}

		itr++;
	}

	if (builder.length())
	{
		itr = 0;
		OUT << "\t\t\tlog_reduce(__LINE__, {($$)->name";
		while (itr < builder.length())
		{
			if (builder[itr] == 'n')
				OUT << ", ($" << itr + 1 << ")->name";
			else
				OUT << ", ($" << itr + 1 << ").name";
			itr++;
		}
		OUT << "});\n";
		if (builder[0] == 'n')
		{
			OUT << "\t\t\t($$) -> lineno = ($1) -> lineno;\n";
			OUT << "\t\t\t($$) -> column = ($1) -> column;\n";
		}
		else
		{
			OUT << "\t\t\t($$) -> lineno = ($1).lineno;\n";
			OUT << "\t\t\t($$) -> column = ($1).colno;\n";
		}
	}
	else 
	{
		OUT << "\t\t\tlog_reduce(__LINE__, {($$)->name});\n";
	}
	
	OUT << "\t\t}\n\n";

}


%}

KEYWORD				KW_[A-Za-z_]*
OPERATOR            OP_[A-Za-z_]*
DELIMITER           DLM_[A-Za-z_]*
CAPITAL             [A-Z_]*
NONTERMINAL         [a-z0-9_]*
TERMINAL            {KEYWORD}|{OPERATOR}|{DELIMITER}|{CAPITAL}
WHITESPACE          [\t\r\n ]
WS                  [\t\f ]*

%x RL_START
%x BODY

%%


"%%" {
	OUT << yytext;
	BEGIN RL_START;
}

<RL_START>"%%" {
	OUT << yytext;
	BEGIN INITIAL;
}

<RL_START>":" {
	OUT << yytext;
	BEGIN BODY;
}

<BODY>{TERMINAL} {
	builder += 't';
	OUT << yytext;
}

<RL_START>{NONTERMINAL} {
	header = yytext;
	OUT << yytext;
}

<BODY>{NONTERMINAL} {
	OUT << yytext;
	builder += 'n';
	string tmp = yytext;
}

<BODY>{WS}*[\n\t]{WHITESPACE}*"|" {
	action_printer(builder);
	int flag = 0;
	builder.clear();
	OUT << yytext;
}

<BODY>{WS}*[\n\t]{WHITESPACE}* {
	action_printer(builder);
	builder.clear();
	OUT << yytext;
	BEGIN RL_START;
}

<BODY>{WS}*"|" {
	action_printer(builder);
	int flag = 0;
	builder.clear();
	OUT << yytext;
}

<BODY>{WS}* {
	OUT << yytext;
}

<*>{WHITESPACE} {
	OUT << yytext;
}

. {
	OUT << yytext;
}

%%

int main(int argc, char *argv[]) {

	FILE *file = fopen(argv[1], "r");
	if (!file) {
		cout << "Error opening file" << "\n";
		exit(0);
	}
	yyin = file;
	yylex();
	fclose(file);
	return 0;
}