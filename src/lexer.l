%option noyywrap
%option yylineno

%{

	#include "parser.tab.h"
	#include <bits/stdc++.h>
	using namespace std;

	extern int is_indent_tabs;
	extern stack<int> indent_stack;
	extern void yyerror (string s);
	extern map<string, int> StringToToken;
	int num_pops = 0;
	int insideBrackets = 0;

	string text = "";

	static int curr_line = 1;
	static int curr_col  = 1;

	// to maintain locations at error pt
	static void update_loc(){

		yylloc.first_line   = curr_line;
		yylloc.first_column = curr_col;

		
		char * s;
		for(s = yytext; *s != '\0'; s++){
			if(*s == '\n'){
				curr_line++;
				curr_col = 1;
				text = "";
			}
			else{
				curr_col++;
				text += *s;
			}
		}
		
		yylloc.last_line   = curr_line;
		yylloc.last_column = curr_col-1;
	}

	extern int verbose;
	extern string tokenString;
	extern vector<string> verbose_stack;
	extern ofstream VERBOSE;

	void log_token(int rule_line, struct __info info) {
		if(verbose)
		{
			VERBOSE << "Token Received: " + string(info.type) + "(" + string(info.name) + "), From Line: " + to_string(info.lineno) + ", Lexer Rule Line: " + to_string(rule_line) + "\n";
			if (tokenString.length())
				verbose_stack.push_back(tokenString);
			tokenString = string(info.name);
			VERBOSE << "CURRENT STACK STATE \n";
			for(int i = 0; i < verbose_stack.size(); i++)
				VERBOSE << "| " + verbose_stack[i] + " ";
			VERBOSE << "|\n\n\n";
		}
	}

	string literal_token = "";

	#define YY_USER_ACTION update_loc();

%}

IDENTIFIER						[a-zA-Z_][a-zA-Z0-9_]*
KEYWORD							False|await|else|import|None|break|except|in|raise|True|class|finally|is|return|continue|for|lambda|tryas|def|from|nonlocal|while|assert|del|global|with|async|elif|if|yield

STRING_LITERAL					{STRINGPREFIX}?({SHORTSTRING}|{LONGSTRING})
STRINGPREFIX					r|R
SHORTSTRING						"\'"{SHORTSTRING_ITEM}*"\'"|"\""{SHORTSTRING_ITEM}*"\""
LONGSTRING						"\'\'\'"{SLONGSTRING_ITEM}*"\'\'\'"|"\"\"\""{DLONGSTRING_ITEM}*"\"\"\""
SHORTSTRING_ITEM				{SHORTSTRING_CHAR}|{STRING_ESCAPE_SEQ}
SLONGSTRING_ITEM				{SLONGSTRING_CHAR}|{STRING_ESCAPE_SEQ}
DLONGSTRING_ITEM				{DLONGSTRING_CHAR}|{STRING_ESCAPE_SEQ}
SHORTSTRING_CHAR				[^\\\n\"\']
SLONGSTRING_CHAR				[^\\']|\'[^\\']|\'\'[^\\']
DLONGSTRING_CHAR				[^\\"]|\"[^\\"]|\"\"[^\\"]
STRING_ESCAPE_SEQ				"\\".|"\\\r\n"|"\\\n"

INTEGER							{DECINTEGER}|{BININTEGER}|{OCTINTEGER}|{HEXINTEGER}
DECINTEGER	 					{NONZERODIGIT}([_]?{DIGIT})*|"0"+([_]?"0")*
BININTEGER						"0"[bB]([_]?{BINDIGIT})+
OCTINTEGER						"0"[oO]([_]?{OCTDIGIT})+
HEXINTEGER						"0"[xX]([_]?{HEXDIGIT})+
NONZERODIGIT					[1-9]
DIGIT							[0-9]
BINDIGIT						[01]
OCTDIGIT						[0-7]
HEXDIGIT						{DIGIT}|[a-fA-F]

FLOATNUMBER						{POINTFLOAT}|{EXPONENTFLOAT}
POINTFLOAT						{DIGITPART}?{FRACTION}|{DIGITPART}"."
EXPONENTFLOAT					({DIGITPART}|{POINTFLOAT}){EXPONENT}
DIGITPART						{DIGIT}([_]?{DIGIT})*
FRACTION						"."{DIGITPART}
EXPONENT						[eE][+-]?{DIGITPART}

OP_ARITHMETIC					"+"|"-"|"*"|"/"|"//"|"%"|"**"
OP_RELATIONAL					"=="|"!="|">"|"<"|">="|"<="
OP_LOGICAL						and|or|not
OP_BITWISE		 				"&"|"|"|"^"|"~"|"<<"|">>"
OP_ASSIGNMENT					"="|"+="|"-="|"*="|"/="|"//="|"%="|"**="|"&="|"|="|"^="|"<<="|">>="

DELIMITER 						"("|")"|"["|"]"|"{"|"}"|","|":"|"."|";"|"@"|"->"

WHITESPACE						[ \t\f\r]
INDENT_SPACE					^[\f]*[ ]*
INDENT_TAB						^[\f]*[\t]*
INDENT_MIX_ERROR				^[\t \f]*
BLANKLINE1						([ \t\f\r]*\n)*
BLANKLINE2						^([ \t\f\r]*\n)*
EXPLICIT_LINE_JOIN				{WHITESPACE}*("\\\r\n"|"\\\n")
COMMENT							"#".*
LINE_COMMENT					^{WHITESPACE}*"#".*{BLANKLINE1}
NEWLINE							\n

%x IDT
%x DDT

%%

<*><<EOF>> {

	if (indent_stack.top() > 0) {
		indent_stack.pop();
		yylval.info.name = strdup("DEDENT");
		yylval.info.type = strdup("DEDENT");
		yylval.info.lineno = yylineno;
		yylval.info.colno = yylloc.first_column;
		log_token(__LINE__, yylval.info);
		return DEDENT;
	}
	
	yylval.info.name = strdup("EOF");
	yylval.info.type = strdup("EOF");
	yylval.info.lineno = yylineno;
	yylval.info.colno = yylloc.first_column;
	log_token(__LINE__, yylval.info);
	return ENDMARK;
}

<*>{LINE_COMMENT} {;}

<*>{COMMENT} {;}

{EXPLICIT_LINE_JOIN}+{WHITESPACE}* {;}

<IDT>{BLANKLINE1} {;}
{BLANKLINE2} {
	if (!insideBrackets) {
		BEGIN IDT;
		yylval.info.name = strdup("NEWLINE");
		yylval.info.type = strdup("NEWLINE");
		yylval.info.lineno = yylineno;
		yylval.info.colno = yylloc.first_column;
		log_token(__LINE__, yylval.info);
		return NEWLINE;
	}
}

{NEWLINE} {
	if (!insideBrackets) {
		BEGIN IDT;
		yylval.info.name = strdup("NEWLINE");
		yylval.info.type = strdup("NEWLINE");
		yylval.info.lineno = yylineno;
		yylval.info.colno = yylloc.first_column;
		log_token(__LINE__, yylval.info);
		return NEWLINE;
	}
}

<IDT>{INDENT_TAB} {

	if (is_indent_tabs == -1) is_indent_tabs = 1;

	else if (is_indent_tabs == 0)
	{
		yyerror("Indentations cannot be a mixture of spaces and tabs.");
		return 0;
	}

	string indent_string = yytext;
	int tabs = count(indent_string.begin(), indent_string.end(), '\t');
	

	if (tabs > indent_stack.top())
	{
		indent_stack.push(tabs);
		BEGIN INITIAL;
		yylval.info.name = strdup("INDENT");
		yylval.info.type = strdup("INDENT");
		yylval.info.lineno = yylineno;
		yylval.info.colno = yylloc.first_column;
		log_token(__LINE__, yylval.info);
		return INDENT;
	}
	BEGIN INITIAL;
	if (tabs != indent_stack.top())
	{

		while (indent_stack.top() != 0 && indent_stack.top() > tabs)
		{
			indent_stack.pop();
			num_pops++;
		} 

		if (indent_stack.top() != tabs) 
		{
			yyerror("Unexpected indent found.");
			return 0;
		}

		BEGIN DDT;
	}
}

<IDT>{INDENT_SPACE} {

	if (is_indent_tabs == -1) is_indent_tabs = 0;
	
	else if (is_indent_tabs == 1)
	{
		yyerror("Indentations cannot be a mixture of spaces and tabs.");
		return 0;
	}

	string indent_string = yytext;
	int spaces = count(indent_string.begin(), indent_string.end(), ' ');

	if (spaces > indent_stack.top())
	{
		indent_stack.push(spaces);
		BEGIN INITIAL;
		yylval.info.name = strdup("INDENT");
		yylval.info.type = strdup("INDENT");
		yylval.info.lineno = yylineno;
		yylval.info.colno = yylloc.first_column;
		log_token(__LINE__, yylval.info);
		return INDENT;
	}
	BEGIN INITIAL;
	if (spaces != indent_stack.top())
	{
		while (indent_stack.top() != 0 && indent_stack.top() > spaces)
		{
			indent_stack.pop();
			num_pops++;
		} 

		if (indent_stack.top() != spaces) 
		{
			yyerror("Unexpected indent found.");
			return 0;
		}
		BEGIN DDT;
	}

}

<IDT>. {
	unput(yytext[0]);
	text.pop_back();
	curr_col--;
	if (indent_stack.top() > 0) {
		indent_stack.pop();
		yylval.info.name = strdup("DEDENT");
		yylval.info.type = strdup("DEDENT");
		yylval.info.lineno = yylineno;
		yylval.info.colno = yylloc.first_column;
		log_token(__LINE__, yylval.info);
		return DEDENT;
	}
	num_pops = 0;
	BEGIN INITIAL;
}

<DDT>. {
	unput(yytext[0]);
	text.pop_back();
	curr_col--;
	if (num_pops > 0) {
		num_pops--;
		yylval.info.name = strdup("DEDENT");
		yylval.info.type = strdup("DEDENT");
		yylval.info.lineno = yylineno;
		yylval.info.colno = yylloc.first_column;
		log_token(__LINE__, yylval.info);
		return DEDENT;
	}
	BEGIN INITIAL;
}


<IDT>{INDENT_MIX_ERROR} {

	yyerror("Indentation cannot be a mixture of spaces and tabs.");
	return 0;

}

{WHITESPACE} {;}

{KEYWORD} {
	yylval.info.name = strdup(yytext);
	yylval.info.type = strdup("KEYWORD");
	yylval.info.lineno = yylineno;
	yylval.info.colno = yylloc.first_column;
	string token_str = yytext;
	log_token(__LINE__, yylval.info);
	return StringToToken[token_str];
}


{OP_ARITHMETIC} {
	yylval.info.name = strdup(yytext);
	yylval.info.type = strdup("OP_ARITHMETIC");
	yylval.info.lineno = yylineno;
	yylval.info.colno = yylloc.first_column;
	string token_str = yytext;
	log_token(__LINE__, yylval.info);
	return StringToToken[token_str];
}

{OP_RELATIONAL} {
	yylval.info.name = strdup(yytext);
	yylval.info.type = strdup("OP_RELATIONAL");
	yylval.info.lineno = yylineno;
	yylval.info.colno = yylloc.first_column;
	string token_str = yytext;
	log_token(__LINE__, yylval.info);
	return StringToToken[token_str];
}

{OP_LOGICAL} {
	yylval.info.name = strdup(yytext);
	yylval.info.type = strdup("OP_LOGICAL");
	yylval.info.lineno = yylineno;
	yylval.info.colno = yylloc.first_column;
	string token_str = yytext;
	log_token(__LINE__, yylval.info);
	return StringToToken[token_str];
}

{OP_BITWISE} {
	yylval.info.name = strdup(yytext);
	yylval.info.type = strdup("OP_BITWISE");
	yylval.info.lineno = yylineno;
	yylval.info.colno = yylloc.first_column;
	string token_str = yytext;
	log_token(__LINE__, yylval.info);
	return StringToToken[token_str];
}

{OP_ASSIGNMENT} {
	yylval.info.name = strdup(yytext);
	yylval.info.type = strdup("OP_ASSIGNMENT");
	yylval.info.lineno = yylineno;
	yylval.info.colno = yylloc.first_column;
	string token_str = yytext;
	log_token(__LINE__, yylval.info);
	return StringToToken[token_str];
}

{IDENTIFIER} {
	yylval.info.name = strdup(yytext);
	yylval.info.type = strdup("IDENTIFIER");
	yylval.info.lineno = yylineno;
	yylval.info.colno = yylloc.first_column;
	log_token(__LINE__, yylval.info);
	return NAME;
}

{INTEGER} {
	yylval.info.name = strdup(yytext);
	yylval.info.type = strdup("INT_LITERAL");
	yylval.info.lineno = yylineno;
	yylval.info.colno = yylloc.first_column;
	log_token(__LINE__, yylval.info);
	return INT_LITERAL;
}

{FLOATNUMBER} {
	yylval.info.name = strdup(yytext);
	yylval.info.type = strdup("FLOAT_LITERAL");
	yylval.info.lineno = yylineno;
	yylval.info.colno = yylloc.first_column;
	log_token(__LINE__, yylval.info);
	return FLOAT_LITERAL;
}

{STRING_LITERAL} {
	string tmp(yytext);

	yylval.info.name = strdup(tmp.c_str());
	yylval.info.type = strdup("STRING_LITERAL");
	yylval.info.lineno = yylineno;
	yylval.info.colno = yylloc.first_column;

	log_token(__LINE__, yylval.info);
	return STRING_LITERAL;
}

{DELIMITER} {
	if(yytext[0] == '(' || yytext[0] == '[' || yytext[0] == '{')
		insideBrackets++;
	if(yytext[0] == ')' || yytext[0] == ']' || yytext[0] == '}')
		insideBrackets--;

	yylval.info.name = strdup(yytext);
	yylval.info.type = strdup("DELIMITER");
	yylval.info.lineno = yylineno;
	yylval.info.colno = yylloc.first_column;
	string token_str = yytext;
	log_token(__LINE__, yylval.info);
	return StringToToken[token_str];
}

. {;}

%%