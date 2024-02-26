%{

    // grammar to be modified yet
    // problem of NAME in atom
    // NAME is assgnable but other parts of atom are not

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
%token <aType> NAME
%token <str> INT_LITERAL FLOAT_LITERAL STRING_LITERAL
%token <bType> OP_ATH_ADD OP_ATH_SUB OP_ATH_MUL OP_ATH_DIV OP_ATH_FDIV OP_ATH_MOD OP_ATH_POW OP_REL_EQ OP_REL_NEQ OP_REL_GT OP_REL_LT OP_REL_GTE OP_REL_LTE OP_LOG_AND OP_LOG_OR OP_LOG_NOT OP_BIT_AND OP_BIT_OR OP_BIT_XOR OP_BIT_NEG OP_BIT_LS OP_BIT_RS OP_ASN_ASN OP_ASN_ADD OP_ASN_SUB OP_ASN_MUL OP_ASN_DIV OP_ASN_FDIV OP_ASN_MOD OP_ASN_POW OP_ASN_AND OP_ASN_OR OP_ASN_XOR OP_ASN_LS OP_ASN_RS
%token <str> DLM_LFT_PRN DLM_RGT_PRN DLM_LFT_SQ DLM_RGT_SQ DLM_LFT_CRLY DLM_RGT_CRLY DLM_COMMA DLM_COLON DLM_DOT DLM_SM_COL DLM_AT DLM_TO
%start file

%define parse.error verbose

%%

file: 
    | statements

statements: statements statement
    | statement

statement: compound_stmt | simple_stmts

simple_stmts: simple1 simple2 NEWLINE

simple1: simple_stmt
    | simple1 DLM_SM_COL simple_stmt

simple2: 
    | DLM_SM_COL

simple_stmt: assignment
    | expressions 
    | return_stmt
    | raise_stmt
    | assert_stmt
    | KW_break 
    | KW_continue 
    | global_stmt
    | nonlocal_stmt

compound_stmt: function_def
    | if_stmt
    | class_def
    | for_stmt
    | while_stmt

assignment: single_target_col  
    | single_target_col OP_ASN_ASN expressions 
    | single_target_augasn expressions 
    | multi_targets_assgn expressions  

multi_targets_assgn: multi_target
    | multi_targets_assgn multi_target

augassign: OP_ASN_ADD
    | OP_ASN_SUB
    | OP_ASN_MUL
    | OP_ASN_DIV
    | OP_ASN_FDIV
    | OP_ASN_MOD
    | OP_ASN_POW
    | OP_ASN_AND
    | OP_ASN_OR 
    | OP_ASN_XOR 
    | OP_ASN_LS 
    | OP_ASN_RS

return_stmt: KW_return expressions
	| KW_return 

raise_stmt: KW_raise expression
	| KW_raise expression KW_from expression
    | KW_raise 

global_stmt: KW_global NAME names

nonlocal_stmt: KW_nonlocal names

names: NAME
	| names DLM_COMMA NAME

assert_stmt: KW_assert expression
	| KW_assert expression DLM_COMMA expression

block: NEWLINE INDENT statements DEDENT 
    | simple_stmts

class_def: KW_class NAME is_arguments DLM_COLON block

function_def: KW_def NAME DLM_LFT_PRN is_params is_fn_expression DLM_COLON block

is_params: DLM_RGT_PRN
	| params

is_arguments:
	| DLM_LFT_PRN DLM_RGT_PRN
	| DLM_LFT_PRN arguments DLM_RGT_PRN

is_fn_expression:
	| DLM_TO expression

params: param_nd_star param_no_default OP_ATH_DIV DLM_RGT_PRN
	| param_nd_star param_no_default OP_ATH_DIV DLM_COMMA param_nd_star sparam_no_default DLM_RGT_PRN
	| param_nd_star param_no_default OP_ATH_DIV DLM_COMMA param_nd_star param_wd_star sparam_with_default DLM_RGT_PRN
	| param_nd_star param_wd_star param_with_default OP_ATH_DIV DLM_RGT_PRN
	| param_nd_star param_wd_star param_with_default OP_ATH_DIV DLM_COMMA param_wd_star sparam_with_default DLM_RGT_PRN
	| param_nd_star sparam_no_default DLM_RGT_PRN
	| param_nd_star param_wd_star sparam_with_default DLM_RGT_PRN
	| param_nd_star param_no_default OP_ATH_DIV DLM_COMMA star_etc
	| param_nd_star param_no_default OP_ATH_DIV DLM_COMMA param_nd_star param_no_default star_etc
	| param_nd_star param_no_default OP_ATH_DIV DLM_COMMA param_nd_star param_wd_star param_with_default star_etc
	| param_nd_star param_wd_star param_with_default OP_ATH_DIV DLM_COMMA star_etc
	| param_nd_star param_wd_star param_with_default OP_ATH_DIV DLM_COMMA param_wd_star param_with_default star_etc
	| param_nd_star star_etc
	| param_nd_star param_wd_star param_with_default star_etc

star_etc: OP_ATH_MUL DLM_COMMA param_mb_star sparam_maybe_default DLM_RGT_PRN

sparam_no_default: param
sparam_with_default: param default
sparam_maybe_default: param 
	| param default
param_no_default: param DLM_COMMA 
param_nd_star: 
	| param_nd_star param_no_default
param_with_default: sparam_with_default DLM_COMMA 
param_wd_star: 
	| param_wd_star param_with_default
param_maybe_default: param DLM_COMMA
	| param default DLM_COMMA
param_mb_star: 
	| param_mb_star param_maybe_default
param: NAME 
    | NAME annotation
annotation: DLM_COLON expression 
default: OP_ASN_ASN expression

if_stmt: KW_if expression DLM_COLON block elif_stmt 
    | KW_if expression DLM_COLON block 
    | KW_if expression DLM_COLON block else_block 

elif_stmt: KW_elif expression DLM_COLON block elif_stmt 
    | KW_elif expression DLM_COLON block 
    | KW_elif expression DLM_COLON block else_block 

else_block: KW_else DLM_COLON block 

while_stmt: KW_while expression DLM_COLON block 
    | KW_while expression DLM_COLON block else_block

for_stmt: KW_for multi_targets KW_in expressions DLM_COLON block 
    | KW_for multi_targets KW_in expressions DLM_COLON block else_block 
 

expressions: expressions_comma
    | expressions_comma DLM_COMMA  

expressions_comma: expression
    | expressions_comma DLM_COMMA expression

expression: disjunction KW_if disjunction KW_else expression 
    | disjunction

disjunction: conjunction
    | disjunction OP_LOG_OR conjunction
    
conjunction: inversion
    | conjunction OP_LOG_AND inversion

inversion: OP_LOG_NOT inversion 
    | comparison

comparison: bitwise_or many_compare_op_bitwise_or_pairs 
    | bitwise_or

many_compare_op_bitwise_or_pairs: compare_op_bitwise_or_pair
    | many_compare_op_bitwise_or_pairs compare_op_bitwise_or_pair

compare_op_bitwise_or_pair: eq_bitwise_or
    | noteq_bitwise_or
    | lte_bitwise_or
    | lt_bitwise_or
    | gte_bitwise_or
    | gt_bitwise_or
    | notin_bitwise_or
    | in_bitwise_or
    | isnot_bitwise_or
    | is_bitwise_or

eq_bitwise_or: OP_REL_EQ bitwise_or 

noteq_bitwise_or: OP_REL_NEQ bitwise_or 

lte_bitwise_or: OP_REL_LTE bitwise_or 

lt_bitwise_or: OP_REL_LT bitwise_or 

gte_bitwise_or: OP_REL_GTE bitwise_or 

gt_bitwise_or: OP_REL_GT bitwise_or 

notin_bitwise_or: OP_LOG_NOT KW_in bitwise_or 

in_bitwise_or: KW_in bitwise_or 

isnot_bitwise_or: KW_is OP_LOG_NOT bitwise_or 

is_bitwise_or: KW_is bitwise_or 

bitwise_or: bitwise_or OP_BIT_OR bitwise_xor 
    | bitwise_xor

bitwise_xor: bitwise_xor OP_BIT_XOR bitwise_and 
    | bitwise_and

bitwise_and: bitwise_and OP_BIT_AND shift_expr 
    | shift_expr

shift_expr: shift_expr OP_BIT_LS sum 
    | shift_expr OP_BIT_RS sum 
    | sum

sum: sum OP_ATH_ADD term 
    | sum OP_ATH_SUB term 
    | term

term: term OP_ATH_MUL factor 
    | term OP_ATH_DIV factor 
    | term OP_ATH_FDIV factor 
    | term OP_ATH_MOD factor 
    | factor

factor: OP_ATH_ADD factor 
    | OP_ATH_SUB factor 
    | OP_BIT_NEG factor 
    | power

power: primary OP_ATH_POW factor 
    | primary

primary: prim1
    | prim2
    | atom

prim1: prim1 DLM_DOT NAME 
    | prim2 DLM_DOT NAME 
    | atom DLM_DOT NAME 
    | prim1 DLM_LFT_SQ slices DLM_RGT_SQ
    | prim2 DLM_LFT_SQ slices DLM_RGT_SQ
    | atom DLM_LFT_SQ slices DLM_RGT_SQ

prim2: prim1 DLM_LFT_PRN DLM_RGT_PRN 
    | prim2 DLM_LFT_PRN DLM_RGT_PRN
    | atom DLM_LFT_PRN DLM_RGT_PRN 
    | prim1 DLM_LFT_PRN arguments DLM_RGT_PRN 
    | prim2 DLM_LFT_PRN arguments DLM_RGT_PRN
    | atom DLM_LFT_PRN arguments DLM_RGT_PRN

slices: slice 
    | slices_comma
    | slices_comma DLM_COMMA

slices_comma: slice DLM_COMMA slice
    | slices_comma DLM_COMMA slice

slice: expression 

atom: NAME
    | KW_True 
    | KW_False 
    | KW_None 
    | strings
    | FLOAT_LITERAL
    | INT_LITERAL
    | group
    | list

group: DLM_LFT_PRN expression DLM_RGT_PRN 

string: STRING_LITERAL 
strings: strings string
    | string

list: DLM_LFT_SQ DLM_RGT_SQ 
    | DLM_LFT_SQ expressions DLM_RGT_SQ 

arguments: args DLM_COMMA
    | args 

args: expressions_comma DLM_COMMA kwargs 
    | expressions_comma 
    | kwargs 

kwargs: kwarg
    | kwargs kwarg

kwarg: NAME OP_ASN_ASN expression  

multi_targets: multi_target_comma DLM_COMMA 
    | multi_target_comma

multi_target_comma: multi_target
    | multi_target_comma DLM_COMMA multi_target

multi_target: single_target_assign 
    | target_list

target_list: DLM_LFT_SQ single_target_list OP_ASN_ASN

single_target_list: single_target_comma single_target_list 
    | single_target_RSQ

single_target_comma: parenthesized_name DLM_COMMA
    | NAME DLM_COMMA
    | prim1 DLM_COMMA

single_target_RSQ: parenthesized_name DLM_RGT_SQ
    | NAME DLM_RGT_SQ
    | prim1 DLM_RGT_SQ

single_target_col: parenthesized_name DLM_COLON expression
    | NAME DLM_COLON expression
    | prim1 DLM_COLON expression

single_target_augasn: parenthesized_name augassign
    | NAME augassign
    | prim1 augassign

single_target_assign: parenthesized_name OP_ASN_ASN
    | NAME OP_ASN_ASN
    | prim1 OP_ASN_ASN

parenthesized_name: DLM_LFT_PRN NAME DLM_RGT_PRN
    | DLM_LFT_PRN parenthesized_name DLM_RGT_PRN

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