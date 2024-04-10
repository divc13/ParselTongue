#include "../parser.tab.h"

void init_token_map()
{
	// symbol to name 
	StringToToken["+"] = OP_ATH_ADD;
	StringToToken["-"] = OP_ATH_SUB;
	StringToToken["*"] = OP_ATH_MUL;
	StringToToken["/"] = OP_ATH_DIV;
	StringToToken["//"] = OP_ATH_FDIV;
	StringToToken["%"] = OP_ATH_MOD;
	StringToToken["**"] = OP_ATH_POW;

	StringToToken["=="] = OP_REL_EQ;
	StringToToken["!="] = OP_REL_NEQ;
	StringToToken[">"] = OP_REL_GT;
	StringToToken["<"] = OP_REL_LT;
	StringToToken[">="] = OP_REL_GTE;
	StringToToken["<="] = OP_REL_LTE;
	
	StringToToken["and"] = OP_LOG_AND;
	StringToToken["or"] = OP_LOG_OR;
	StringToToken["not"] = OP_LOG_NOT;
	
	StringToToken["&"] = OP_BIT_AND;
	StringToToken["|"] = OP_BIT_OR;
	StringToToken["^"] = OP_BIT_XOR;
	StringToToken["~"] = OP_BIT_NEG;
	StringToToken["<<"] = OP_BIT_LS;
	StringToToken[">>"] = OP_BIT_RS;
	
	StringToToken["="] = OP_ASN_ASN;
	StringToToken["+="] = OP_ASN_ADD;
	StringToToken["-="] = OP_ASN_SUB;
	StringToToken["*="] = OP_ASN_MUL;
	StringToToken["/="] = OP_ASN_DIV;
	StringToToken["//="] = OP_ASN_FDIV;
	StringToToken["%="] = OP_ASN_MOD;
	StringToToken["**="] = OP_ASN_POW;
	StringToToken["&="] = OP_ASN_AND;
	StringToToken["|="] = OP_ASN_OR;
	StringToToken["^="] = OP_ASN_XOR;
	StringToToken["<<="] = OP_ASN_LS;
	StringToToken[">>="] = OP_ASN_RS;

	StringToToken["("] = DLM_LFT_PRN;
	StringToToken[")"] = DLM_RGT_PRN;
	StringToToken["["] = DLM_LFT_SQ;
	StringToToken["]"] = DLM_RGT_SQ;
	StringToToken["{"] = DLM_LFT_CRLY;
	StringToToken["}"] = DLM_RGT_CRLY;
	StringToToken[","] = DLM_COMMA;
	StringToToken[":"] = DLM_COLON;
	StringToToken["."] = DLM_DOT;
	StringToToken[";"] = DLM_SM_COL;
	StringToToken["@"] = DLM_AT;
	StringToToken["->"] = DLM_TO;

	StringToToken["False"] = KW_False;
	StringToToken["await"] = KW_await;
	StringToToken["else"] = KW_else;
	StringToToken["import"] = KW_import;
	StringToToken["None"] = KW_None;
	StringToToken["break"] = KW_break;
	StringToToken["except"] = KW_except;
	StringToToken["in"] = KW_in;
	StringToToken["raise"] = KW_raise;
	StringToToken["True"] = KW_True;
	StringToToken["class"] = KW_class;
	StringToToken["finally"] = KW_finally;
	StringToToken["is"] = KW_is;
	StringToToken["return"] = KW_return;
	StringToToken["continue"] = KW_continue;
	StringToToken["for"] = KW_for;
	StringToToken["lambda"] = KW_lambda;
	StringToToken["tryas"] = KW_tryas;
	StringToToken["def"] = KW_def;
	StringToToken["from"] = KW_from;
	StringToToken["nonlocal"] = KW_nonlocal;
	StringToToken["while"] = KW_while;
	StringToToken["assert"] = KW_assert;
	StringToToken["del"] = KW_del;
	StringToToken["global"] = KW_global;
	StringToToken["with"] = KW_with;
	StringToToken["async"] = KW_async;
	StringToToken["elif"] = KW_elif;
	StringToToken["if"] = KW_if;
	StringToToken["yield"] = KW_yield;

	StringToToken["\n"] = NEWLINE;

	  
	// name to symbol 
	TokenNameToString["OP_BIT_AND"] = "&";
	TokenNameToString["OP_BIT_OR"] = "|";
	TokenNameToString["OP_BIT_XOR"] = "^";
	TokenNameToString["OP_BIT_NEG"] = "~";
	TokenNameToString["OP_BIT_LS"] = "<<";
	TokenNameToString["OP_BIT_RS"] = ">>";

	TokenNameToString["OP_ATH_ADD"] = "+";
	TokenNameToString["OP_ATH_SUB"] = "-";
	TokenNameToString["OP_ATH_MUL"] = "*";
	TokenNameToString["OP_ATH_DIV"] = "/";
	TokenNameToString["OP_ATH_FDIV"] = "//";
	TokenNameToString["OP_ATH_MOD"] = "%";
	TokenNameToString["OP_ATH_POW"] = "**";

	TokenNameToString["OP_REL_EQ"] = "==";
	TokenNameToString["OP_REL_NEQ"] = "!=";
	TokenNameToString["OP_REL_GT"] = ">";
	TokenNameToString["OP_REL_LT"] = "<";
	TokenNameToString["OP_REL_GTE"] = ">=";
	TokenNameToString["OP_REL_LTE"] = "<=";
	
	TokenNameToString["OP_ASN_ASN"] = "=";
	TokenNameToString["OP_ASN_ADD"] = "+=";
	TokenNameToString["OP_ASN_SUB"] = "-=";
	TokenNameToString["OP_ASN_MUL"] = "*=";
	TokenNameToString["OP_ASN_DIV"] = "/=";
	TokenNameToString["OP_ASN_FDIV"] = "//=";
	TokenNameToString["OP_ASN_MOD"] = "%=";
	TokenNameToString["OP_ASN_POW"] = "**=";
	TokenNameToString["OP_ASN_AND"] = "&=";
	TokenNameToString["OP_ASN_OR"] = "|=";
	TokenNameToString["OP_ASN_XOR"] = "^=";
	TokenNameToString["OP_ASN_LS"] = "<<=";
	TokenNameToString["OP_ASN_RS"] = ">>=";

	TokenNameToString["DLM_LFT_PRN"] = "(";
	TokenNameToString["DLM_RGT_PRN"] = ")";
	TokenNameToString["DLM_LFT_SQ"] = "[";
	TokenNameToString["DLM_RGT_SQ"] = "]";
	TokenNameToString["DLM_LFT_CRLY"] = "{";
	TokenNameToString["DLM_RGT_CRLY"] = "}";
	TokenNameToString["DLM_COMMA"] = ",";
	TokenNameToString["DLM_COLON"] = ":";
	TokenNameToString["DLM_DOT"] = ".";
	TokenNameToString["DLM_SM_COL"] = ";";
	TokenNameToString["DLM_AT"] = "@";
	TokenNameToString["DLM_TO"] = "->";

	TokenNameToString["OP_LOG_AND"] = "and";
	TokenNameToString["OP_LOG_OR"] = "or";
	TokenNameToString["OP_LOG_NOT"] = "not";

	TokenNameToString["KW_False"] = "False";
	TokenNameToString["KW_await"] = "await";
	TokenNameToString["KW_else"] = "else";
	TokenNameToString["KW_import"] = "import";
	TokenNameToString["KW_None"] = "None";
	TokenNameToString["KW_break"] = "break";
	TokenNameToString["KW_except"] = "except";
	TokenNameToString["KW_in"] = "in";
	TokenNameToString["KW_raise"] = "raise";
	TokenNameToString["KW_True"] = "True";
	TokenNameToString["KW_class"] = "class";
	TokenNameToString["KW_finally"] = "finally";
	TokenNameToString["KW_is"] = "is";
	TokenNameToString["KW_return"] = "return";
	TokenNameToString["KW_continue"] = "continue";
	TokenNameToString["KW_for"] = "for";
	TokenNameToString["KW_lambda"] = "lambda";
	TokenNameToString["KW_tryas"] = "tryas";
	TokenNameToString["KW_def"] = "def";
	TokenNameToString["KW_from"] = "from";
	TokenNameToString["KW_nonlocal"] = "nonlocal";
	TokenNameToString["KW_while"] = "while";
	TokenNameToString["KW_assert"] = "assert";
	TokenNameToString["KW_del"] = "del";
	TokenNameToString["KW_global"] = "global";
	TokenNameToString["KW_with"] = "with";
	TokenNameToString["KW_async"] = "async";
	TokenNameToString["KW_elif"] = "elif";
	TokenNameToString["KW_if"] = "if";
	TokenNameToString["KW_yield"] = "yield";
	
	TokenNameToString["INDENT"] = "INDENT";
	TokenNameToString["DEDENT"] = "DEDENT";
	TokenNameToString["NEWLINE"] = "NEWLINE";
	
	TokenNameToString["NAME"] = "IDENTIFIER";
	TokenNameToString["INT_LITERAL"] = "INTEGER";
	TokenNameToString["FLOAT_LITERAL"] = "FLOATNUMBER";
	TokenNameToString["STRING_LITERAL"] = "STRING_LITERAL";





	opType["+"] = "OP_ARITHMETIC";
	opType["-"] = "OP_ARITHMETIC";
	opType["*"] = "OP_ARITHMETIC";
	opType["/"] = "OP_ARITHMETIC";
	opType["//"] = "OP_ARITHMETIC";
	opType["%"] = "OP_ARITHMETIC";
	opType["**"] = "OP_ARITHMETIC";
	opType["&"] = "OP_BITWISE";
	opType["|"] = "OP_BITWISE";
	opType["^"] = "OP_BITWISE";
	opType["<<"] = "OP_BITWISE";
	opType[">>"] = "OP_BITWISE";



	opType["="] = "OP_ASSIGNMENT";
	opType["+="] = "OP_ASSIGNMENT";
	opType["-="] = "OP_ASSIGNMENT";
	opType["*="] = "OP_ASSIGNMENT";
	opType["/="] = "OP_ASSIGNMENT";
	opType["//="] = "OP_ASSIGNMENT";
	opType["%="] = "OP_ASSIGNMENT";
	opType["**="] = "OP_ASSIGNMENT";
	opType["&="] = "OP_ASSIGNMENT";
	opType["|="] = "OP_ASSIGNMENT";
	opType["^="] = "OP_ASSIGNMENT";
	opType["<<="] = "OP_ASSIGNMENT";
	opType[">>="] = "OP_ASSIGNMENT";
}