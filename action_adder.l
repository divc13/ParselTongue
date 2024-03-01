%option noyywrap
/* works for one rule on one line */

%{

#include <bits/stdc++.h>
using namespace std;
ofstream OUT("parser.y");
string builder = "";
vector<string>nonterminals;

// #define YY_USER_ACTION cout << builder << endl;

void action_printer(string builder)
{
    int itr = 0;
    int child = 1;
    int ntrml = 0;

    OUT << "\n\n\t\t{\n";

    if (builder.length() == 0)
    {
        OUT << "\t\t\t$$ = NULL;\n";
        OUT << "\t\t}\n\n";

        return;
    }

    if (builder[0] == 'f')
    {
        OUT << "\t\t\troot = new TreeNode(\"File\");\n";
        OUT << "\t\t\t$$ = root;\n";
        itr++;
    }

    OUT << "\t\t\tTreeNode* temp;\n";

    while(itr < builder.length())
    {
        //terminal
        if (builder[itr] == 't')
        {
            OUT << "\t\t\ttemp = new TreeNode(($" << child << ").name, ($" << child << ").type);\n";
            OUT << "\t\t\t(($$) -> children).push_back(temp); \n";
        }

        //non terminal
        else if (builder[itr] == 'n')
        {
            OUT << "\t\t\ttemp = new TreeNode(\"" << nonterminals[ntrml] << "\");\n"; 
            OUT << "\t\t\t(($$) -> children).push_back(temp);\n";
            ntrml++;
        }

        child++;
        itr++;
    }
    OUT << "\t\t}\n\n";

}


%}

KEYWORD				KW_[a-z_]*
OPERATOR            OP_[A-Za-z_]*
DELIMITER           DLM_[A-Za-z_]*
CAPITAL             [A-Z_]*
file                "file"
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

<RL_START>{file} {
    OUT << yytext;
    builder += 'f';
}

<RL_START>{NONTERMINAL} {;
    OUT << yytext;
}

<BODY>{NONTERMINAL} {
    OUT << yytext;
    builder += 'n';
    string tmp = yytext;
    nonterminals.push_back(tmp);
}

<BODY>{TERMINAL} {
    builder += 't';
    OUT << yytext;
}

<BODY>{WS}*[\n\t]{WHITESPACE}*"|" {
    action_printer(builder);
    int flag = 0;
    if (builder[0] == 'f') flag = 1;
    builder.clear();
    if (flag) builder += 'f';
    nonterminals.clear();
    OUT << yytext;
}

<BODY>{WS}*[\n\t]{WHITESPACE}* {
    action_printer(builder);
    builder.clear();
    nonterminals.clear();
    OUT << yytext;
    BEGIN RL_START;
}

<BODY>{WS}*"|" {
    action_printer(builder);
    int flag = 0;
    if (builder[0] == 'f') flag = 1;
    builder.clear();
    if (flag) builder += 'f';
    nonterminals.clear();
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