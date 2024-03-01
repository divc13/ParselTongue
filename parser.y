%{
#include <bits/stdc++.h>
using namespace std;
#include "node.hpp"

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
%type<Node> file statements statement simple_stmts simple_stmt compound_stmt simple1 simple2 assignment return_stmt raise_stmt assert_stmt global_stmt nonlocal_stmt multi_targets_assgn augassign names block class_def function_def is_params is_arguments is_fn_expression params param_nd param_wd slashed_param_nd slashed_param_wd star_etc sparam_maybe_default param_no_default param_with_default param_maybe_default param_mb_star param annotation default if_stmt elif_stmt else_block while_stmt for_stmt expressions expressions_comma expression disjunction conjunction inversion comparison many_compare_op_bitwise_or_pairs compare_op_bitwise_or_pair eq_bitwise_or noteq_bitwise_or lte_bitwise_or lt_bitwise_or gte_bitwise_or gt_bitwise_or notin_bitwise_or in_bitwise_or isnot_bitwise_or is_bitwise_or bitwise_xor bitwise_or bitwise_and shift_expr sum term factor power primary slices slices_comma slice atom group string strings list arguments kwargs kwarg

%define parse.error custom
%locations
%define parse.lac full
%debug

%%

file: ENDMARK 

		{
			root = new TreeNode("File");
			$$ = root;
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
		}


	| statements ENDMARK 

		{
			root = new TreeNode("File");
			$$ = root;
			TreeNode* temp;
			temp = new TreeNode("statements");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
		}



statements: statements statement

		{
			TreeNode* temp;
			temp = new TreeNode("statements");
			(($$) -> children).push_back(temp);
			temp = new TreeNode("statement");
			(($$) -> children).push_back(temp);
		}


	| statement

		{
			TreeNode* temp;
			temp = new TreeNode("statement");
			(($$) -> children).push_back(temp);
		}



statement: compound_stmt 

		{
			TreeNode* temp;
			temp = new TreeNode("compound_stmt");
			(($$) -> children).push_back(temp);
		}


    | simple_stmts

		{
			TreeNode* temp;
			temp = new TreeNode("simple_stmts");
			(($$) -> children).push_back(temp);
		}



simple_stmts: simple1 simple2 NEWLINE

		{
			TreeNode* temp;
			temp = new TreeNode("simple1");
			(($$) -> children).push_back(temp);
			temp = new TreeNode("simple2");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($3).name, ($3).type);
			(($$) -> children).push_back(temp); 
		}



simple1: simple_stmt

		{
			TreeNode* temp;
			temp = new TreeNode("simple_stmt");
			(($$) -> children).push_back(temp);
		}


	| simple1 DLM_SM_COL simple_stmt

		{
			TreeNode* temp;
			temp = new TreeNode("simple1");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("simple_stmt");
			(($$) -> children).push_back(temp);
		}



simple2: 

		{
			$$ = NULL;
		}


    | DLM_SM_COL

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
		}



simple_stmt: assignment

		{
			TreeNode* temp;
			temp = new TreeNode("assignment");
			(($$) -> children).push_back(temp);
		}


    | expressions 

		{
			TreeNode* temp;
			temp = new TreeNode("expressions");
			(($$) -> children).push_back(temp);
		}


    | return_stmt

		{
			TreeNode* temp;
			temp = new TreeNode("return_stmt");
			(($$) -> children).push_back(temp);
		}


    | raise_stmt

		{
			TreeNode* temp;
			temp = new TreeNode("raise_stmt");
			(($$) -> children).push_back(temp);
		}


    | assert_stmt

		{
			TreeNode* temp;
			temp = new TreeNode("assert_stmt");
			(($$) -> children).push_back(temp);
		}


    | KW_break 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
		}


    | KW_continue 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
		}


    | global_stmt

		{
			TreeNode* temp;
			temp = new TreeNode("global_stmt");
			(($$) -> children).push_back(temp);
		}


    | nonlocal_stmt

		{
			TreeNode* temp;
			temp = new TreeNode("nonlocal_stmt");
			(($$) -> children).push_back(temp);
		}



compound_stmt: function_def

		{
			TreeNode* temp;
			temp = new TreeNode("function_def");
			(($$) -> children).push_back(temp);
		}


    | if_stmt

		{
			TreeNode* temp;
			temp = new TreeNode("if_stmt");
			(($$) -> children).push_back(temp);
		}


    | class_def

		{
			TreeNode* temp;
			temp = new TreeNode("class_def");
			(($$) -> children).push_back(temp);
		}


    | for_stmt

		{
			TreeNode* temp;
			temp = new TreeNode("for_stmt");
			(($$) -> children).push_back(temp);
		}


    | while_stmt

		{
			TreeNode* temp;
			temp = new TreeNode("while_stmt");
			(($$) -> children).push_back(temp);
		}



assignment: primary DLM_COLON expression  

		{
			TreeNode* temp;
			temp = new TreeNode("primary");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("expression");
			(($$) -> children).push_back(temp);
		}


    | primary DLM_COLON expression OP_ASN_ASN expressions 

		{
			TreeNode* temp;
			temp = new TreeNode("primary");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("expression");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($4).name, ($4).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("expressions");
			(($$) -> children).push_back(temp);
		}


    | primary augassign expressions 

		{
			TreeNode* temp;
			temp = new TreeNode("primary");
			(($$) -> children).push_back(temp);
			temp = new TreeNode("augassign");
			(($$) -> children).push_back(temp);
			temp = new TreeNode("expressions");
			(($$) -> children).push_back(temp);
		}


    | multi_targets_assgn expressions  

		{
			TreeNode* temp;
			temp = new TreeNode("multi_targets_assgn");
			(($$) -> children).push_back(temp);
			temp = new TreeNode("expressions");
			(($$) -> children).push_back(temp);
		}



multi_targets_assgn: primary OP_ASN_ASN

		{
			TreeNode* temp;
			temp = new TreeNode("primary");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
		}


    | multi_targets_assgn primary OP_ASN_ASN

		{
			TreeNode* temp;
			temp = new TreeNode("multi_targets_assgn");
			(($$) -> children).push_back(temp);
			temp = new TreeNode("primary");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($3).name, ($3).type);
			(($$) -> children).push_back(temp); 
		}



augassign: OP_ASN_ADD

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
		}


    | OP_ASN_SUB

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
		}


    | OP_ASN_MUL

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
		}


    | OP_ASN_DIV

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
		}


    | OP_ASN_FDIV

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
		}


    | OP_ASN_MOD

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
		}


    | OP_ASN_POW

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
		}


    | OP_ASN_AND

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
		}


    | OP_ASN_OR 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
		}


    | OP_ASN_XOR 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
		}


    | OP_ASN_LS 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
		}


    | OP_ASN_RS

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
		}



return_stmt: KW_return expressions

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("expressions");
			(($$) -> children).push_back(temp);
		}


	| KW_return 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
		}



raise_stmt: KW_raise expression

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("expression");
			(($$) -> children).push_back(temp);
		}


	| KW_raise expression KW_from expression

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("expression");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($3).name, ($3).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("expression");
			(($$) -> children).push_back(temp);
		}


    | KW_raise 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
		}



global_stmt: KW_global NAME names

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("names");
			(($$) -> children).push_back(temp);
		}



nonlocal_stmt: KW_nonlocal names

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("names");
			(($$) -> children).push_back(temp);
		}



names: NAME

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
		}


	| names DLM_COMMA NAME

		{
			TreeNode* temp;
			temp = new TreeNode("names");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode(($3).name, ($3).type);
			(($$) -> children).push_back(temp); 
		}



assert_stmt: KW_assert expression

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("expression");
			(($$) -> children).push_back(temp);
		}


	| KW_assert expression DLM_COMMA expression

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("expression");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($3).name, ($3).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("expression");
			(($$) -> children).push_back(temp);
		}



block: NEWLINE INDENT statements DEDENT 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("statements");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($4).name, ($4).type);
			(($$) -> children).push_back(temp); 
		}


    | simple_stmts

		{
			TreeNode* temp;
			temp = new TreeNode("simple_stmts");
			(($$) -> children).push_back(temp);
		}



class_def: KW_class NAME is_arguments DLM_COLON block

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("is_arguments");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($4).name, ($4).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("block");
			(($$) -> children).push_back(temp);
		}



function_def: KW_def NAME DLM_LFT_PRN is_params DLM_RGT_PRN is_fn_expression DLM_COLON block

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode(($3).name, ($3).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("is_params");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($5).name, ($5).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("is_fn_expression");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($7).name, ($7).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("block");
			(($$) -> children).push_back(temp);
		}



is_params:

		{
			$$ = NULL;
		}


	| params

		{
			TreeNode* temp;
			temp = new TreeNode("params");
			(($$) -> children).push_back(temp);
		}



is_arguments:

		{
			$$ = NULL;
		}


	| DLM_LFT_PRN DLM_RGT_PRN

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
		}


	| DLM_LFT_PRN arguments DLM_RGT_PRN

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("arguments");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($3).name, ($3).type);
			(($$) -> children).push_back(temp); 
		}



is_fn_expression:

		{
			$$ = NULL;
		}


	| DLM_TO expression

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("expression");
			(($$) -> children).push_back(temp);
		}



params: param param_nd

		{
			TreeNode* temp;
			temp = new TreeNode("param");
			(($$) -> children).push_back(temp);
			temp = new TreeNode("param_nd");
			(($$) -> children).push_back(temp);
		}


    | param default param_wd

		{
			TreeNode* temp;
			temp = new TreeNode("param");
			(($$) -> children).push_back(temp);
			temp = new TreeNode("default");
			(($$) -> children).push_back(temp);
			temp = new TreeNode("param_wd");
			(($$) -> children).push_back(temp);
		}


    | star_etc

		{
			TreeNode* temp;
			temp = new TreeNode("star_etc");
			(($$) -> children).push_back(temp);
		}



param_nd: 

		{
			$$ = NULL;
		}


	| param_no_default param_nd

		{
			TreeNode* temp;
			temp = new TreeNode("param_no_default");
			(($$) -> children).push_back(temp);
			temp = new TreeNode("param_nd");
			(($$) -> children).push_back(temp);
		}


    | param_with_default param_wd

		{
			TreeNode* temp;
			temp = new TreeNode("param_with_default");
			(($$) -> children).push_back(temp);
			temp = new TreeNode("param_wd");
			(($$) -> children).push_back(temp);
		}


    | DLM_COMMA OP_ATH_DIV slashed_param_nd

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("slashed_param_nd");
			(($$) -> children).push_back(temp);
		}


    | DLM_COMMA star_etc

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("star_etc");
			(($$) -> children).push_back(temp);
		}



param_wd: 

		{
			$$ = NULL;
		}


	| param_with_default param_wd

		{
			TreeNode* temp;
			temp = new TreeNode("param_with_default");
			(($$) -> children).push_back(temp);
			temp = new TreeNode("param_wd");
			(($$) -> children).push_back(temp);
		}


    | DLM_COMMA OP_ATH_DIV slashed_param_wd

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("slashed_param_wd");
			(($$) -> children).push_back(temp);
		}


    | DLM_COMMA star_etc

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("star_etc");
			(($$) -> children).push_back(temp);
		}



slashed_param_nd: 

		{
			$$ = NULL;
		}


	| param_no_default slashed_param_nd

		{
			TreeNode* temp;
			temp = new TreeNode("param_no_default");
			(($$) -> children).push_back(temp);
			temp = new TreeNode("slashed_param_nd");
			(($$) -> children).push_back(temp);
		}


    | param_with_default param_wd

		{
			TreeNode* temp;
			temp = new TreeNode("param_with_default");
			(($$) -> children).push_back(temp);
			temp = new TreeNode("param_wd");
			(($$) -> children).push_back(temp);
		}


    | DLM_COMMA star_etc

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("star_etc");
			(($$) -> children).push_back(temp);
		}



slashed_param_wd: 

		{
			$$ = NULL;
		}


	| param_with_default slashed_param_wd

		{
			TreeNode* temp;
			temp = new TreeNode("param_with_default");
			(($$) -> children).push_back(temp);
			temp = new TreeNode("slashed_param_wd");
			(($$) -> children).push_back(temp);
		}


    | DLM_COMMA star_etc

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("star_etc");
			(($$) -> children).push_back(temp);
		}



star_etc: OP_ATH_MUL DLM_COMMA param_mb_star sparam_maybe_default

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("param_mb_star");
			(($$) -> children).push_back(temp);
			temp = new TreeNode("sparam_maybe_default");
			(($$) -> children).push_back(temp);
		}



sparam_maybe_default: param 

		{
			TreeNode* temp;
			temp = new TreeNode("param");
			(($$) -> children).push_back(temp);
		}


	| param default

		{
			TreeNode* temp;
			temp = new TreeNode("param");
			(($$) -> children).push_back(temp);
			temp = new TreeNode("default");
			(($$) -> children).push_back(temp);
		}



param_no_default: DLM_COMMA param 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("param");
			(($$) -> children).push_back(temp);
		}



param_with_default: DLM_COMMA param default  

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("param");
			(($$) -> children).push_back(temp);
			temp = new TreeNode("default");
			(($$) -> children).push_back(temp);
		}



param_maybe_default: param DLM_COMMA

		{
			TreeNode* temp;
			temp = new TreeNode("param");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
		}


	| param default DLM_COMMA

		{
			TreeNode* temp;
			temp = new TreeNode("param");
			(($$) -> children).push_back(temp);
			temp = new TreeNode("default");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($3).name, ($3).type);
			(($$) -> children).push_back(temp); 
		}


param_mb_star: 

		{
			$$ = NULL;
		}


	| param_mb_star param_maybe_default

		{
			TreeNode* temp;
			temp = new TreeNode("param_mb_star");
			(($$) -> children).push_back(temp);
			temp = new TreeNode("param_maybe_default");
			(($$) -> children).push_back(temp);
		}


param: NAME 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
		}


    | NAME annotation

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("annotation");
			(($$) -> children).push_back(temp);
		}


annotation: DLM_COLON expression 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("expression");
			(($$) -> children).push_back(temp);
		}


default: OP_ASN_ASN expression

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("expression");
			(($$) -> children).push_back(temp);
		}



if_stmt: KW_if expression DLM_COLON block elif_stmt 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("expression");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($3).name, ($3).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("block");
			(($$) -> children).push_back(temp);
			temp = new TreeNode("elif_stmt");
			(($$) -> children).push_back(temp);
		}


    | KW_if expression DLM_COLON block 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("expression");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($3).name, ($3).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("block");
			(($$) -> children).push_back(temp);
		}


    | KW_if expression DLM_COLON block else_block 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("expression");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($3).name, ($3).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("block");
			(($$) -> children).push_back(temp);
			temp = new TreeNode("else_block");
			(($$) -> children).push_back(temp);
		}



elif_stmt: KW_elif expression DLM_COLON block elif_stmt 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("expression");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($3).name, ($3).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("block");
			(($$) -> children).push_back(temp);
			temp = new TreeNode("elif_stmt");
			(($$) -> children).push_back(temp);
		}


    | KW_elif expression DLM_COLON block 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("expression");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($3).name, ($3).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("block");
			(($$) -> children).push_back(temp);
		}


    | KW_elif expression DLM_COLON block else_block 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("expression");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($3).name, ($3).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("block");
			(($$) -> children).push_back(temp);
			temp = new TreeNode("else_block");
			(($$) -> children).push_back(temp);
		}



else_block: KW_else DLM_COLON block 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("block");
			(($$) -> children).push_back(temp);
		}



while_stmt: KW_while expression DLM_COLON block 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("expression");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($3).name, ($3).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("block");
			(($$) -> children).push_back(temp);
		}


    | KW_while expression DLM_COLON block else_block

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("expression");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($3).name, ($3).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("block");
			(($$) -> children).push_back(temp);
			temp = new TreeNode("else_block");
			(($$) -> children).push_back(temp);
		}



for_stmt: KW_for primary KW_in expression DLM_COLON block 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("primary");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($3).name, ($3).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("expression");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($5).name, ($5).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("block");
			(($$) -> children).push_back(temp);
		}


    | KW_for primary KW_in expression DLM_COLON block else_block 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("primary");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($3).name, ($3).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("expression");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($5).name, ($5).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("block");
			(($$) -> children).push_back(temp);
			temp = new TreeNode("else_block");
			(($$) -> children).push_back(temp);
		}


 

expressions: expressions_comma

		{
			TreeNode* temp;
			temp = new TreeNode("expressions_comma");
			(($$) -> children).push_back(temp);
		}


    | expressions_comma DLM_COMMA  

		{
			TreeNode* temp;
			temp = new TreeNode("expressions_comma");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
		}



expressions_comma: expression

		{
			TreeNode* temp;
			temp = new TreeNode("expression");
			(($$) -> children).push_back(temp);
		}


    | expressions_comma DLM_COMMA expression

		{
			TreeNode* temp;
			temp = new TreeNode("expressions_comma");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("expression");
			(($$) -> children).push_back(temp);
		}



expression: disjunction KW_if disjunction KW_else expression 

		{
			TreeNode* temp;
			temp = new TreeNode("disjunction");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("disjunction");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($4).name, ($4).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("expression");
			(($$) -> children).push_back(temp);
		}


    | disjunction

		{
			TreeNode* temp;
			temp = new TreeNode("disjunction");
			(($$) -> children).push_back(temp);
		}



disjunction: conjunction

		{
			TreeNode* temp;
			temp = new TreeNode("conjunction");
			(($$) -> children).push_back(temp);
		}


    | disjunction OP_LOG_OR conjunction

		{
			TreeNode* temp;
			temp = new TreeNode("disjunction");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("conjunction");
			(($$) -> children).push_back(temp);
		}


    
conjunction: inversion

		{
			TreeNode* temp;
			temp = new TreeNode("inversion");
			(($$) -> children).push_back(temp);
		}


    | conjunction OP_LOG_AND inversion

		{
			TreeNode* temp;
			temp = new TreeNode("conjunction");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("inversion");
			(($$) -> children).push_back(temp);
		}



inversion: OP_LOG_NOT inversion 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("inversion");
			(($$) -> children).push_back(temp);
		}


    | comparison

		{
			TreeNode* temp;
			temp = new TreeNode("comparison");
			(($$) -> children).push_back(temp);
		}



comparison: bitwise_or many_compare_op_bitwise_or_pairs 

		{
			TreeNode* temp;
			temp = new TreeNode("bitwise_or");
			(($$) -> children).push_back(temp);
			temp = new TreeNode("many_compare_op_bitwise_or_pairs");
			(($$) -> children).push_back(temp);
		}


    | bitwise_or

		{
			TreeNode* temp;
			temp = new TreeNode("bitwise_or");
			(($$) -> children).push_back(temp);
		}



many_compare_op_bitwise_or_pairs: compare_op_bitwise_or_pair

		{
			TreeNode* temp;
			temp = new TreeNode("compare_op_bitwise_or_pair");
			(($$) -> children).push_back(temp);
		}


    | many_compare_op_bitwise_or_pairs compare_op_bitwise_or_pair

		{
			TreeNode* temp;
			temp = new TreeNode("many_compare_op_bitwise_or_pairs");
			(($$) -> children).push_back(temp);
			temp = new TreeNode("compare_op_bitwise_or_pair");
			(($$) -> children).push_back(temp);
		}



compare_op_bitwise_or_pair: eq_bitwise_or

		{
			TreeNode* temp;
			temp = new TreeNode("eq_bitwise_or");
			(($$) -> children).push_back(temp);
		}


    | noteq_bitwise_or

		{
			TreeNode* temp;
			temp = new TreeNode("noteq_bitwise_or");
			(($$) -> children).push_back(temp);
		}


    | lte_bitwise_or

		{
			TreeNode* temp;
			temp = new TreeNode("lte_bitwise_or");
			(($$) -> children).push_back(temp);
		}


    | lt_bitwise_or

		{
			TreeNode* temp;
			temp = new TreeNode("lt_bitwise_or");
			(($$) -> children).push_back(temp);
		}


    | gte_bitwise_or

		{
			TreeNode* temp;
			temp = new TreeNode("gte_bitwise_or");
			(($$) -> children).push_back(temp);
		}


    | gt_bitwise_or

		{
			TreeNode* temp;
			temp = new TreeNode("gt_bitwise_or");
			(($$) -> children).push_back(temp);
		}


    | notin_bitwise_or

		{
			TreeNode* temp;
			temp = new TreeNode("notin_bitwise_or");
			(($$) -> children).push_back(temp);
		}


    | in_bitwise_or

		{
			TreeNode* temp;
			temp = new TreeNode("in_bitwise_or");
			(($$) -> children).push_back(temp);
		}


    | isnot_bitwise_or

		{
			TreeNode* temp;
			temp = new TreeNode("isnot_bitwise_or");
			(($$) -> children).push_back(temp);
		}


    | is_bitwise_or

		{
			TreeNode* temp;
			temp = new TreeNode("is_bitwise_or");
			(($$) -> children).push_back(temp);
		}



eq_bitwise_or: OP_REL_EQ bitwise_or 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("bitwise_or");
			(($$) -> children).push_back(temp);
		}



noteq_bitwise_or: OP_REL_NEQ bitwise_or 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("bitwise_or");
			(($$) -> children).push_back(temp);
		}



lte_bitwise_or: OP_REL_LTE bitwise_or 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("bitwise_or");
			(($$) -> children).push_back(temp);
		}



lt_bitwise_or: OP_REL_LT bitwise_or 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("bitwise_or");
			(($$) -> children).push_back(temp);
		}



gte_bitwise_or: OP_REL_GTE bitwise_or 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("bitwise_or");
			(($$) -> children).push_back(temp);
		}



gt_bitwise_or: OP_REL_GT bitwise_or 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("bitwise_or");
			(($$) -> children).push_back(temp);
		}



notin_bitwise_or: OP_LOG_NOT KW_in bitwise_or 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("bitwise_or");
			(($$) -> children).push_back(temp);
		}



in_bitwise_or: KW_in bitwise_or 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("bitwise_or");
			(($$) -> children).push_back(temp);
		}



isnot_bitwise_or: KW_is OP_LOG_NOT bitwise_or 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("bitwise_or");
			(($$) -> children).push_back(temp);
		}



is_bitwise_or: KW_is bitwise_or 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("bitwise_or");
			(($$) -> children).push_back(temp);
		}



bitwise_or: bitwise_or OP_BIT_OR bitwise_xor 

		{
			TreeNode* temp;
			temp = new TreeNode("bitwise_or");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("bitwise_xor");
			(($$) -> children).push_back(temp);
		}


    | bitwise_xor

		{
			TreeNode* temp;
			temp = new TreeNode("bitwise_xor");
			(($$) -> children).push_back(temp);
		}



bitwise_xor: bitwise_xor OP_BIT_XOR bitwise_and 

		{
			TreeNode* temp;
			temp = new TreeNode("bitwise_xor");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("bitwise_and");
			(($$) -> children).push_back(temp);
		}


    | bitwise_and

		{
			TreeNode* temp;
			temp = new TreeNode("bitwise_and");
			(($$) -> children).push_back(temp);
		}



bitwise_and: bitwise_and OP_BIT_AND shift_expr 

		{
			TreeNode* temp;
			temp = new TreeNode("bitwise_and");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("shift_expr");
			(($$) -> children).push_back(temp);
		}


    | shift_expr

		{
			TreeNode* temp;
			temp = new TreeNode("shift_expr");
			(($$) -> children).push_back(temp);
		}



shift_expr: shift_expr OP_BIT_LS sum 

		{
			TreeNode* temp;
			temp = new TreeNode("shift_expr");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("sum");
			(($$) -> children).push_back(temp);
		}


    | shift_expr OP_BIT_RS sum 

		{
			TreeNode* temp;
			temp = new TreeNode("shift_expr");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("sum");
			(($$) -> children).push_back(temp);
		}


    | sum

		{
			TreeNode* temp;
			temp = new TreeNode("sum");
			(($$) -> children).push_back(temp);
		}



sum: sum OP_ATH_ADD term 

		{
			TreeNode* temp;
			temp = new TreeNode("sum");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("term");
			(($$) -> children).push_back(temp);
		}


    | sum OP_ATH_SUB term 

		{
			TreeNode* temp;
			temp = new TreeNode("sum");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("term");
			(($$) -> children).push_back(temp);
		}


    | term

		{
			TreeNode* temp;
			temp = new TreeNode("term");
			(($$) -> children).push_back(temp);
		}



term: term OP_ATH_MUL factor 

		{
			TreeNode* temp;
			temp = new TreeNode("term");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("factor");
			(($$) -> children).push_back(temp);
		}


    | term OP_ATH_DIV factor 

		{
			TreeNode* temp;
			temp = new TreeNode("term");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("factor");
			(($$) -> children).push_back(temp);
		}


    | term OP_ATH_FDIV factor 

		{
			TreeNode* temp;
			temp = new TreeNode("term");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("factor");
			(($$) -> children).push_back(temp);
		}


    | term OP_ATH_MOD factor 

		{
			TreeNode* temp;
			temp = new TreeNode("term");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("factor");
			(($$) -> children).push_back(temp);
		}


    | factor

		{
			TreeNode* temp;
			temp = new TreeNode("factor");
			(($$) -> children).push_back(temp);
		}



factor: OP_ATH_ADD factor 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("factor");
			(($$) -> children).push_back(temp);
		}


    | OP_ATH_SUB factor 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("factor");
			(($$) -> children).push_back(temp);
		}


    | OP_BIT_NEG factor 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("factor");
			(($$) -> children).push_back(temp);
		}


    | power

		{
			TreeNode* temp;
			temp = new TreeNode("power");
			(($$) -> children).push_back(temp);
		}



power: primary OP_ATH_POW factor 

		{
			TreeNode* temp;
			temp = new TreeNode("primary");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("factor");
			(($$) -> children).push_back(temp);
		}


    | primary

		{
			TreeNode* temp;
			temp = new TreeNode("primary");
			(($$) -> children).push_back(temp);
		}



primary: primary DLM_DOT NAME 

		{
			TreeNode* temp;
			temp = new TreeNode("primary");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode(($3).name, ($3).type);
			(($$) -> children).push_back(temp); 
		}


    | primary DLM_LFT_PRN DLM_RGT_PRN  

		{
			TreeNode* temp;
			temp = new TreeNode("primary");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode(($3).name, ($3).type);
			(($$) -> children).push_back(temp); 
		}


    | primary DLM_LFT_PRN arguments DLM_RGT_PRN  

		{
			TreeNode* temp;
			temp = new TreeNode("primary");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("arguments");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($4).name, ($4).type);
			(($$) -> children).push_back(temp); 
		}


    | primary DLM_LFT_SQ slices DLM_RGT_SQ 

		{
			TreeNode* temp;
			temp = new TreeNode("primary");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("slices");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($4).name, ($4).type);
			(($$) -> children).push_back(temp); 
		}


    | atom

		{
			TreeNode* temp;
			temp = new TreeNode("atom");
			(($$) -> children).push_back(temp);
		}



slices: slice 

		{
			TreeNode* temp;
			temp = new TreeNode("slice");
			(($$) -> children).push_back(temp);
		}


    | slices_comma

		{
			TreeNode* temp;
			temp = new TreeNode("slices_comma");
			(($$) -> children).push_back(temp);
		}


    | slices_comma DLM_COMMA

		{
			TreeNode* temp;
			temp = new TreeNode("slices_comma");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
		}



slices_comma: slice DLM_COMMA slice

		{
			TreeNode* temp;
			temp = new TreeNode("slice");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("slice");
			(($$) -> children).push_back(temp);
		}


    | slices_comma DLM_COMMA slice

		{
			TreeNode* temp;
			temp = new TreeNode("slices_comma");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("slice");
			(($$) -> children).push_back(temp);
		}



slice: expression 

		{
			TreeNode* temp;
			temp = new TreeNode("expression");
			(($$) -> children).push_back(temp);
		}



atom: NAME

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
		}


    | KW_True 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("rue");
			(($$) -> children).push_back(temp);
		}


    | KW_False 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("alse");
			(($$) -> children).push_back(temp);
		}


    | KW_None 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("one");
			(($$) -> children).push_back(temp);
		}


    | strings

		{
			TreeNode* temp;
			temp = new TreeNode("strings");
			(($$) -> children).push_back(temp);
		}


    | FLOAT_LITERAL

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
		}


    | INT_LITERAL

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
		}


    | group

		{
			TreeNode* temp;
			temp = new TreeNode("group");
			(($$) -> children).push_back(temp);
		}


    | list

		{
			TreeNode* temp;
			temp = new TreeNode("list");
			(($$) -> children).push_back(temp);
		}



group: DLM_LFT_PRN expression DLM_RGT_PRN 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("expression");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($3).name, ($3).type);
			(($$) -> children).push_back(temp); 
		}



string: STRING_LITERAL 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
		}


strings: strings string

		{
			TreeNode* temp;
			temp = new TreeNode("strings");
			(($$) -> children).push_back(temp);
			temp = new TreeNode("string");
			(($$) -> children).push_back(temp);
		}


    | string

		{
			TreeNode* temp;
			temp = new TreeNode("string");
			(($$) -> children).push_back(temp);
		}



list: DLM_LFT_SQ DLM_RGT_SQ 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
		}


    | DLM_LFT_SQ expressions DLM_RGT_SQ 

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("expressions");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($3).name, ($3).type);
			(($$) -> children).push_back(temp); 
		}



arguments: expressions_comma DLM_COMMA kwargs DLM_COMMA

		{
			TreeNode* temp;
			temp = new TreeNode("expressions_comma");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("kwargs");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($4).name, ($4).type);
			(($$) -> children).push_back(temp); 
		}


    | expressions_comma DLM_COMMA

		{
			TreeNode* temp;
			temp = new TreeNode("expressions_comma");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
		}


    | kwargs DLM_COMMA

		{
			TreeNode* temp;
			temp = new TreeNode("kwargs");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
		}


    | expressions_comma DLM_COMMA kwargs 

		{
			TreeNode* temp;
			temp = new TreeNode("expressions_comma");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("kwargs");
			(($$) -> children).push_back(temp);
		}


    | expressions_comma 

		{
			TreeNode* temp;
			temp = new TreeNode("expressions_comma");
			(($$) -> children).push_back(temp);
		}


    | kwargs 

		{
			TreeNode* temp;
			temp = new TreeNode("kwargs");
			(($$) -> children).push_back(temp);
		}



kwargs: kwarg

		{
			TreeNode* temp;
			temp = new TreeNode("kwarg");
			(($$) -> children).push_back(temp);
		}


    | kwargs DLM_COMMA kwarg

		{
			TreeNode* temp;
			temp = new TreeNode("kwargs");
			(($$) -> children).push_back(temp);
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("kwarg");
			(($$) -> children).push_back(temp);
		}



kwarg: NAME OP_ASN_ASN expression  

		{
			TreeNode* temp;
			temp = new TreeNode(($1).name, ($1).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode(($2).name, ($2).type);
			(($$) -> children).push_back(temp); 
			temp = new TreeNode("expression");
			(($$) -> children).push_back(temp);
		}




%%					
#include "error.hpp"