%{

#include <bits/stdc++.h>
#include "token_map.h"
using namespace std;

#define RED "\033[1;31m"
#define RESET "\033[0m"
#define YELLOW "\033[1;33m"

void yyerror (string s);
int yylex();
extern char* yytext;
extern FILE* yyin;
extern int yylineno;

// 1 if file is indented using tabs, 0 if indented using spaces, -1 initially
int is_indent_tabs = -1;
stack <int> indent_stack;



%}

%code requires {

	#include<bits/stdc++.h>
	using namespace std;

	struct typeA{
		int lineno;
		char* name;
	};

	struct typeB{
		char* name;
		char* type;
	};

}

%union {
		
	// Used for KEYWORD, DATATYPE, DELIMITER
	char* str;

	// Used for IDENTIFIER
	struct typeA aType;

	// USED for LITERAL, OPERATOR
	struct typeB bType;
}

%token NEWLINE
%token ENDMARKER
%token INDENT
%token DEDENT
%token LIST
%token <str> KW_False KW_await KW_else KW_import KW_None KW_break KW_except KW_in KW_raise KW_True KW_class KW_finally KW_is KW_return KW_continue KW_for KW_lambda KW_tryas KW_def KW_from KW_nonlocal KW_while KW_assert KW_del KW_global KW_with KW_async KW_elif KW_if KW_yield
%token <str> DT_int DT_float DT_bool DT_str
%token <aType> IDENTIFIER
%token <bType> LITERAL
%token <bType> OP_ATH_ADD OP_ATH_SUB OP_ATH_MUL OP_ATH_DIV OP_ATH_FDIV OP_ATH_MOD OP_ATH_POW OP_REL_EQ OP_REL_NEQ OP_REL_GT OP_REL_LT OP_REL_GTE OP_REL_LTE OP_LOG_AND OP_LOG_OR OP_LOG_NOT OP_BIT_AND OP_BIT_OR OP_BIT_XOR OP_BIT_NEG OP_BIT_LS OP_BIT_RS OP_ASN_ASN OP_ASN_ADD OP_ASN_SUB OP_ASN_MUL OP_ASN_DIV OP_ASN_FDIV OP_ASN_MOD OP_ASN_POW OP_ASN_AND OP_ASN_OR OP_ASN_XOR OP_ASN_LS OP_ASN_RS
%token <str> DLM_LFT_PRN DLM_RGT_PRN DLM_LFT_SQ DLM_RGT_SQ DLM_LFT_CRLY DLM_RGT_CRLY DLM_COMMA DLM_COLON DLM_DOT DLM_SM_COL DLM_AT DLM_TO

%start file

%%

file : KW_assert


%%					

int main(int argc, char** argv)
{
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
	yyparse ( );
		
	fclose(yyin);

	return 0;
}

void yyerror (string s) {
	cout << RED << "Found Error at line " << yylineno <<" : " << RESET << yytext << endl;
	cout << RED << s << RESET << endl;
} 