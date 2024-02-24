%option noyywrap
%option yylineno

%{

    #include "parser.tab.h"

    extern int is_indent_tabs;
    extern stack<int> indent_stack;
	extern void yyerror (string s);
    extern map<string, int> symbol_to_name;
		
%}

IDENTIFIER						[a-zA-Z_][a-zA-Z0-9_]*
DATATYPE						int|float|bool|str
LIST							list
KEYWORD							False|await|else|import|None|break|except|in|raise|True|class|finally|is|return|continue|for|lambda|tryas|def|from|nonlocal|while|assert|del|global|with|async|elif|if|yield

STRING_LITERAL					{STRINGPREFIX}?({SHORTSTRING}|{LONGSTRING})
STRINGPREFIX					r|u|R|U|f|F|fr|Fr|fR|FR|rf|rF|Rf|RF
SHORTSTRING						"'"{SHORTSTRING_ITEM}*"'"|'"'{SHORTSTRING_ITEM}*'"'
LONGSTRING						"'''"{LONGSTRING_ITEM}*"'''"|'"""'{LONGSTRING_ITEM}*'"""'
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

WHITESPACE						[ \t\f]
INDENT_SPACE					^[\f]*[ ]*
INDENT_TAB						^[\f]*[\t]*
INDENT_MIX_ERROR				^[\t \f]*
BLANKLINE						^{WHITESPACE}*[\n]
EXPLICIT_LINE_JOIN				\\\n
COMMENT							\#[.*]
NEWLINE							\n

%%

{COMMENT} {;}

{EXPLICIT_LINE_JOIN} {;}

{BLANKLINE} {;}

{NEWLINE} {

	return NEWLINE;

}

{INDENT_TAB} {

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
		return INDENT;
	}

	if (indent_stack.top() != tabs)
	{
		while (indent_stack.top() != 0 && indent_stack.top() > tabs) indent_stack.pop();

		if (indent_stack.top() != tabs) 
		{
			yyerror("Unexpected indent found.");
			return 0;
		}

		return DEDENT;
	}

}

{INDENT_SPACE} {

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
		return INDENT;
	}

	if (indent_stack.top() != spaces)
	{
		while (indent_stack.top() != 0 && indent_stack.top() > spaces) indent_stack.pop();

		if (indent_stack.top() != spaces) 
		{
			yyerror("Unexpected indent found.");
			return 0;
		}

		return DEDENT;
	}

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

{DATATYPE} {
	yylval.str = strdup(yytext);
	string token_str = yytext;
	return symbol_to_name[token_str];
}

{LIST} {
	return LIST;
}

{IDENTIFIER} {
	yylval.aType.name = strdup(yytext);
	yylval.aType.lineno = yylineno;
	return IDENTIFIER;
}

{FLOATNUMBER} {
	yylval.bType.name = strdup(yytext);
	yylval.bType.type = strdup("FLOAT");
	return LITERAL;
}

{INTEGER} {
	yylval.bType.name = strdup(yytext);
	yylval.bType.type = strdup("INTEGER");
	return LITERAL;
}

{STRING_LITERAL} {
	yylval.bType.name = strdup(yytext);
	yylval.bType.type = strdup("STRING");
	return LITERAL;
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

. {
	yyerror("Unexpected token found.");
    return 0;
}

%%
