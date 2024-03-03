%{
#include <bits/stdc++.h>
using namespace std;
#include "include/node.hpp"

void yyerror (string s);
int yylex();
extern FILE* yyin;
extern char* yytext;
extern int yylineno;
extern string text;

extern map<string, int> StringToToken;
extern map<string, string> TokenNameToString;

// 1 if file is indented using tabs, 0 if indented using spaces, -1 initially
int is_indent_tabs = -1;
extern stack <int> indent_stack;

extern TreeNode* root;

%}

%code requires {

    struct __info{
        char* name;
        char* type;
    };
}

%union {
    struct __info info;
    struct node* Node;
}

%token<info> DLM_LFT_PRN DLM_RGT_PRN DLM_LFT_SQ DLM_RGT_SQ DLM_LFT_CRLY DLM_RGT_CRLY DLM_COMMA DLM_COLON DLM_DOT DLM_SM_COL DLM_AT DLM_TO INT_LITERAL FLOAT_LITERAL STRING_LITERAL NAME NEWLINE DEDENT INDENT KW_False KW_await KW_else KW_import KW_None KW_break KW_except KW_in KW_raise KW_True KW_class KW_finally KW_is KW_return KW_continue KW_for KW_lambda KW_tryas KW_def KW_from KW_nonlocal KW_while KW_assert KW_del KW_global KW_with KW_async KW_elif KW_if KW_yield OP_ATH_ADD OP_ATH_SUB OP_ATH_MUL OP_ATH_DIV OP_ATH_FDIV OP_ATH_MOD OP_ATH_POW OP_REL_EQ OP_REL_NEQ OP_REL_GT OP_REL_LT OP_REL_GTE OP_REL_LTE OP_LOG_AND OP_LOG_OR OP_LOG_NOT OP_BIT_AND OP_BIT_OR OP_BIT_XOR OP_BIT_NEG OP_BIT_LS OP_BIT_RS OP_ASN_ASN OP_ASN_ADD OP_ASN_SUB OP_ASN_MUL OP_ASN_DIV OP_ASN_FDIV OP_ASN_MOD OP_ASN_POW OP_ASN_AND OP_ASN_OR OP_ASN_XOR OP_ASN_LS OP_ASN_RS
%token<info> ENDMARK 0
%type<Node> bitwise_operators if_expression operand typedecl bitwise_operator file statements statement simple_stmts simple_stmt compound_stmt simple1 simple2 assignment return_stmt raise_stmt assert_stmt global_stmt nonlocal_stmt multi_targets_assgn augassign names block class_def function_def is_params is_arguments is_fn_expression params param_nd param_wd  param_no_default param_with_default param annotation param_default if_stmt elif_stmt else_block while_stmt for_stmt expressions expressions_comma expression disjunction conjunction inversion comparison  bitwise_xor bitwise_or bitwise_and shift_expr sum term factor power primary slices slices_comma slice atom group string strings list arguments kwargs kwarg

%define parse.error custom
%locations
%define parse.lac full
%debug

%%

file: ENDMARK 
	| NEWLINE ENDMARK 
	| statements ENDMARK 

statements: statements statement
	| statement

statement: compound_stmt 
    | simple_stmts

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

typedecl: primary DLM_COLON expression

assignment: typedecl
    | typedecl OP_ASN_ASN expressions 
    | primary augassign expressions 
    | multi_targets_assgn   

multi_targets_assgn: primary OP_ASN_ASN expressions
    | primary OP_ASN_ASN multi_targets_assgn

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

global_stmt: KW_global names

nonlocal_stmt: KW_nonlocal names

names: NAME
	| names DLM_COMMA NAME

assert_stmt: KW_assert expression
	| KW_assert expression DLM_COMMA expression

block: NEWLINE INDENT statements DEDENT 
    | simple_stmts

class_def: KW_class NAME is_arguments DLM_COLON block

function_def: KW_def NAME DLM_LFT_PRN is_params DLM_RGT_PRN is_fn_expression DLM_COLON block

is_params:
	| params

is_arguments:
	| DLM_LFT_PRN DLM_RGT_PRN
	| DLM_LFT_PRN arguments DLM_RGT_PRN

is_fn_expression:
	| DLM_TO expression

params: param param_nd 
    | param_default param_wd

param_nd: 
	| param_no_default param_nd
    | param_with_default param_wd
    | DLM_COMMA

param_wd: 
	| param_with_default param_wd
    | DLM_COMMA 

param_no_default: DLM_COMMA param 

param_with_default: DLM_COMMA param_default  

param: NAME 
    | annotation
annotation: NAME DLM_COLON expression 
param_default:param OP_ASN_ASN expression


if_stmt: KW_if expression DLM_COLON block elif_stmt 
    | KW_if expression DLM_COLON block 
    | KW_if expression DLM_COLON block else_block 

elif_stmt: KW_elif expression DLM_COLON block elif_stmt 
    | KW_elif expression DLM_COLON block 
    | KW_elif expression DLM_COLON block else_block 

else_block: KW_else DLM_COLON block 

while_stmt: KW_while expression DLM_COLON block 
    | KW_while expression DLM_COLON block else_block

for_stmt: KW_for primary KW_in expression DLM_COLON block 
    | KW_for primary KW_in expression DLM_COLON block else_block 
 
expressions: expressions_comma
    | expressions_comma DLM_COMMA  

expressions_comma: expression
    | expressions_comma DLM_COMMA expression

if_expression: disjunction KW_if disjunction KW_else expression

expression: if_expression 
    | disjunction

disjunction: conjunction
    | disjunction OP_LOG_OR conjunction
    
conjunction: inversion
    | conjunction OP_LOG_AND inversion

inversion: OP_LOG_NOT inversion 
    | comparison


comparison: bitwise_or
    | comparison bitwise_operator bitwise_or

bitwise_operator: OP_REL_EQ
    | OP_REL_NEQ
    | OP_REL_LTE
    | OP_REL_LT
    | OP_REL_GTE
    | OP_REL_GT
    | KW_in
    | bitwise_operators
    | KW_is

bitwise_operators: OP_LOG_NOT KW_in
    | KW_is OP_LOG_NOT


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

primary: NAME DLM_DOT NAME 
    | NAME DLM_DOT operand
    | operand 
    | atom

operand: NAME DLM_LFT_PRN DLM_RGT_PRN  
    | NAME DLM_LFT_PRN arguments DLM_RGT_PRN  
    | NAME DLM_LFT_SQ slices DLM_RGT_SQ 
    | NAME DLM_LFT_SQ DLM_RGT_SQ

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
    | DLM_LFT_PRN DLM_RGT_PRN

string: STRING_LITERAL 
strings: strings string
    | string

list: DLM_LFT_SQ DLM_RGT_SQ 
    | DLM_LFT_SQ expressions DLM_RGT_SQ 

arguments: expressions_comma DLM_COMMA kwargs DLM_COMMA
    | expressions_comma DLM_COMMA
    | kwargs DLM_COMMA
    | expressions_comma DLM_COMMA kwargs 
    | expressions_comma 
    | kwargs 

kwargs: kwarg
    | kwargs DLM_COMMA kwarg

kwarg: NAME OP_ASN_ASN expression  


%%					
#include "include/error.hpp"