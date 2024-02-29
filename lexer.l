%option noyywrap
%option yylineno

%{

	#include "parser.tab.h"

	extern int is_indent_tabs;
	extern stack<int> indent_stack;
	extern void yyerror (string s);
	extern map<string, int> symbol_to_name;
	int num_pops = 0;

	string text = "";

	static int curr_line = 1;
	static int curr_col  = 1;
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

	#define YY_USER_ACTION update_loc();

%}

IDENTIFIER						[a-zA-Z_][a-zA-Z0-9_]*
KEYWORD							False|await|else|import|None|break|except|in|raise|True|class|finally|is|return|continue|for|lambda|tryas|def|from|nonlocal|while|assert|del|global|with|async|elif|if|yield

STRING_LITERAL					{STRINGPREFIX}?({SHORTSTRING}|{LONGSTRING})
STRINGPREFIX					r|u|R|U|f|F|fr|Fr|fR|FR|rf|rF|Rf|RF
SHORTSTRING						"\'"{SHORTSTRING_ITEM}*"\'"|"\""{SHORTSTRING_ITEM}*"\""
LONGSTRING						"\'\'\'"{LONGSTRING_ITEM}*"\'\'\'"|"\"\"\""{LONGSTRING_ITEM}*"\"\"\""
SHORTSTRING_ITEM				{SHORTSTRING_CHAR}|{STRING_ESCAPE_SEQ}
LONGSTRING_ITEM					{LONGSTRING_CHAR}|{STRING_ESCAPE_SEQ}
SHORTSTRING_CHAR				[^\\\n\"\']
LONGSTRING_CHAR					[^\\]
STRING_ESCAPE_SEQ				\\[.\n]

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

ARITHMETIC_OPERATOR		 		"+"|"-"|"*"|"/"|"//"|"%"|"**"
RELATIONAL_OPERATOR 			"=="|"!="|">"|"<"|">="|"<="
LOGICAL_OPERATOR 				and|or|not
BITWISE_OPERATOR 				"&"|"|"|"^"|"~"|"<<"|">>"
ASSIGNMENT_OPERATOR 			"="|"+="|"-="|"*="|"/="|"//="|"%="|"**="|"&="|"|="|"^="|"<<="|">>="
DELIMITER 						"("|")"|"["|"]"|"{"|"}"|","|":"|"."|";"|"@"|"->"

WHITESPACE						[ \t\f\r]
INDENT_SPACE					^[\f]*[ ]*
INDENT_TAB						^[\f]*[\t]*
INDENT_MIX_ERROR				^[\t \f]*
BLANKLINE1						([ \t\f\r]*\n)*
BLANKLINE2						^([ \t\f\r]*\n)*
EXPLICIT_LINE_JOIN				\\\n
COMMENT							\#[.*]
LINE_COMMENT					^{WHITESPACE}*\#[.*]\n
NEWLINE							\n

%x IDT
%x DDT
%%

<IDT><<EOF>> {
	if (indent_stack.top() > 0) {
		indent_stack.pop();
		return DEDENT;
	}
	return ENDMARK;
}

[{COMMENT}{LINE_COMMENT}] {;}

{EXPLICIT_LINE_JOIN} {;}

<IDT>{BLANKLINE1} {;}
{BLANKLINE2} {;}

{NEWLINE} {
	BEGIN IDT;
	return NEWLINE;
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
		return DEDENT;
	}
	BEGIN INITIAL;
}


{INDENT_MIX_ERROR} {

	yyerror("Indentation cannot be a mixture of spaces and tabs.");
	return 0;

}

{WHITESPACE} {;}

{KEYWORD} {
	yylval.str = strdup(yytext);
	string token_str = yytext;
	return symbol_to_name[token_str];
}


{LOGICAL_OPERATOR} {
	yylval.bType.name = strdup(yytext);
	yylval.bType.type = strdup("LOGICAL");
	string token_str = yytext;
	return symbol_to_name[token_str];
}

{IDENTIFIER} {
	yylval.aType.name = strdup(yytext);
	yylval.aType.lineno = yylineno;
	return NAME;
}

{INTEGER} {
	yylval.str = strdup(yytext);
	return INT_LITERAL;
}

{FLOATNUMBER} {
	yylval.str = strdup(yytext);
	return FLOAT_LITERAL;
}

{STRING_LITERAL} {
	yylval.str = strdup(yytext);
	return STRING_LITERAL;
}

{ARITHMETIC_OPERATOR} {
	yylval.bType.name = strdup(yytext);
	yylval.bType.type = strdup("ARITHMETIC");
	string token_str = yytext;
	return symbol_to_name[token_str];
}

{RELATIONAL_OPERATOR} {
	yylval.bType.name = strdup(yytext);
	yylval.bType.type = strdup("RELATIONAL");
	string token_str = yytext;
	return symbol_to_name[token_str];
}


{BITWISE_OPERATOR} {
	yylval.bType.name = strdup(yytext);
	yylval.bType.type = strdup("BITWISE");
	string token_str = yytext;
	return symbol_to_name[token_str];
}

{ASSIGNMENT_OPERATOR} {
	yylval.bType.name = strdup(yytext);
	yylval.bType.type = strdup("ASSIGNMENT");
	string token_str = yytext;
	return symbol_to_name[token_str];
}

{DELIMITER} {
	yylval.str = strdup(yytext);
	string token_str = yytext;
	return symbol_to_name[token_str];
}

. {;}

%%