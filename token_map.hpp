#include<bits/stdc++.h>
#include "parser.tab.h"
using namespace std;

map<string, int> symbol_to_name;
map<string, string> name_to_symbol;

void init_symbol_name_tables()
{

    // symbol to name 
    symbol_to_name["+"] = OP_ATH_ADD;
    symbol_to_name["-"] = OP_ATH_SUB;
    symbol_to_name["*"] = OP_ATH_MUL;
    symbol_to_name["/"] = OP_ATH_DIV;
    symbol_to_name["//"] = OP_ATH_FDIV;
    symbol_to_name["%"] = OP_ATH_MOD;
    symbol_to_name["**"] = OP_ATH_POW;

    symbol_to_name["=="] = OP_REL_EQ;
    symbol_to_name["!="] = OP_REL_NEQ;
    symbol_to_name[">"] = OP_REL_GT;
    symbol_to_name["<"] = OP_REL_LT;
    symbol_to_name[">="] = OP_REL_GTE;
    symbol_to_name["<="] = OP_REL_LTE;
    
    symbol_to_name["and"] = OP_LOG_AND;
    symbol_to_name["or"] = OP_LOG_OR;
    symbol_to_name["not"] = OP_LOG_NOT;
    
    symbol_to_name["&"] = OP_BIT_AND;
    symbol_to_name["|"] = OP_BIT_OR;
    symbol_to_name["^"] = OP_BIT_XOR;
    symbol_to_name["~"] = OP_BIT_NEG;
    symbol_to_name["<<"] = OP_BIT_LS;
    symbol_to_name[">>"] = OP_BIT_RS;
    
    symbol_to_name["="] = OP_ASN_ASN;
    symbol_to_name["+="] = OP_ASN_ADD;
    symbol_to_name["-="] = OP_ASN_SUB;
    symbol_to_name["*="] = OP_ASN_MUL;
    symbol_to_name["/="] = OP_ASN_DIV;
    symbol_to_name["//="] = OP_ASN_FDIV;
    symbol_to_name["%="] = OP_ASN_MOD;
    symbol_to_name["**="] = OP_ASN_POW;
    symbol_to_name["&="] = OP_ASN_AND;
    symbol_to_name["|="] = OP_ASN_OR;
    symbol_to_name["^="] = OP_ASN_XOR;
    symbol_to_name["<<="] = OP_ASN_LS;
    symbol_to_name[">>="] = OP_ASN_RS;

    symbol_to_name["("] = DLM_LFT_PRN;
    symbol_to_name[")"] = DLM_RGT_PRN;
    symbol_to_name["["] = DLM_LFT_SQ;
    symbol_to_name["]"] = DLM_RGT_SQ;
    symbol_to_name["{"] = DLM_LFT_CRLY;
    symbol_to_name["}"] = DLM_RGT_CRLY;
    symbol_to_name[","] = DLM_COMMA;
    symbol_to_name[":"] = DLM_COLON;
    symbol_to_name["."] = DLM_DOT;
    symbol_to_name[";"] = DLM_SM_COL;
    symbol_to_name["@"] = DLM_AT;
    symbol_to_name["->"] = DLM_TO;

    symbol_to_name["False"] = KW_False;
    symbol_to_name["await"] = KW_await;
    symbol_to_name["else"] = KW_else;
    symbol_to_name["import"] = KW_import;
    symbol_to_name["None"] = KW_None;
    symbol_to_name["break"] = KW_break;
    symbol_to_name["except"] = KW_except;
    symbol_to_name["in"] = KW_in;
    symbol_to_name["raise"] = KW_raise;
    symbol_to_name["True"] = KW_True;
    symbol_to_name["class"] = KW_class;
    symbol_to_name["finally"] = KW_finally;
    symbol_to_name["is"] = KW_is;
    symbol_to_name["return"] = KW_return;
    symbol_to_name["continue"] = KW_continue;
    symbol_to_name["for"] = KW_for;
    symbol_to_name["lambda"] = KW_lambda;
    symbol_to_name["tryas"] = KW_tryas;
    symbol_to_name["def"] = KW_def;
    symbol_to_name["from"] = KW_from;
    symbol_to_name["nonlocal"] = KW_nonlocal;
    symbol_to_name["while"] = KW_while;
    symbol_to_name["assert"] = KW_assert;
    symbol_to_name["del"] = KW_del;
    symbol_to_name["global"] = KW_global;
    symbol_to_name["with"] = KW_with;
    symbol_to_name["async"] = KW_async;
    symbol_to_name["elif"] = KW_elif;
    symbol_to_name["if"] = KW_if;
    symbol_to_name["yield"] = KW_yield;

    symbol_to_name["int"] = DT_int;
    symbol_to_name["float"] = DT_float;
    symbol_to_name["bool"] = DT_bool;
    symbol_to_name["str"] = DT_str;

      
    // name to symbol 
    name_to_symbol["OP_BIT_AND"] = "&";
    name_to_symbol["OP_BIT_OR"] = "|";
    name_to_symbol["OP_BIT_XOR"] = "^";
    name_to_symbol["OP_BIT_NEG"] = "~";
    name_to_symbol["OP_BIT_LS"] = "<<";
    name_to_symbol["OP_BIT_RS"] = ">>";

    name_to_symbol["OP_ATH_ADD"] = "+";
    name_to_symbol["OP_ATH_SUB"] = "-";
    name_to_symbol["OP_ATH_MUL"] = "*";
    name_to_symbol["OP_ATH_DIV"] = "/";
    name_to_symbol["OP_ATH_FDIV"] = "//";
    name_to_symbol["OP_ATH_MOD"] = "%";
    name_to_symbol["OP_ATH_POW"] = "**";

    name_to_symbol["OP_REL_EQ"] = "==";
    name_to_symbol["OP_REL_NEQ"] = "!=";
    name_to_symbol["OP_REL_GT"] = ">";
    name_to_symbol["OP_REL_LT"] = "<";
    name_to_symbol["OP_REL_GTE"] = ">=";
    name_to_symbol["OP_REL_LTE"] = "<=";
    
    name_to_symbol["OP_ASN_ASN"] = "=";
    name_to_symbol["OP_ASN_ADD"] = "+=";
    name_to_symbol["OP_ASN_SUB"] = "-=";
    name_to_symbol["OP_ASN_MUL"] = "*=";
    name_to_symbol["OP_ASN_DIV"] = "/=";
    name_to_symbol["OP_ASN_FDIV"] = "//=";
    name_to_symbol["OP_ASN_MOD"] = "%=";
    name_to_symbol["OP_ASN_POW"] = "**=";
    name_to_symbol["OP_ASN_AND"] = "&=";
    name_to_symbol["OP_ASN_OR"] = "|=";
    name_to_symbol["OP_ASN_XOR"] = "^=";
    name_to_symbol["OP_ASN_LS"] = "<<=";
    name_to_symbol["OP_ASN_RS"] = ">>=";

    name_to_symbol["DLM_LFT_PRN"] = "(";
    name_to_symbol["DLM_RGT_PRN"] = ")";
    name_to_symbol["DLM_LFT_SQ"] = "[";
    name_to_symbol["DLM_RGT_SQ"] = "]";
    name_to_symbol["DLM_LFT_CRLY"] = "{";
    name_to_symbol["DLM_RGT_CRLY"] = "}";
    name_to_symbol["DLM_COMMA"] = ",";
    name_to_symbol["DLM_COLON"] = ":";
    name_to_symbol["DLM_DOT"] = ".";
    name_to_symbol["DLM_SM_COL"] = ";";
    name_to_symbol["DLM_AT"] = "@";
    name_to_symbol["DLM_TO"] = "->";

    name_to_symbol["OP_LOG_AND"] = "and";
    name_to_symbol["OP_LOG_OR"] = "or";
    name_to_symbol["OP_LOG_NOT"] = "not";

    name_to_symbol["KW_False"] = "False";
    name_to_symbol["KW_await"] = "await";
    name_to_symbol["KW_else"] = "else";
    name_to_symbol["KW_import"] = "import";
    name_to_symbol["KW_None"] = "None";
    name_to_symbol["KW_break"] = "break";
    name_to_symbol["KW_except"] = "except";
    name_to_symbol["KW_in"] = "in";
    name_to_symbol["KW_raise"] = "raise";
    name_to_symbol["KW_True"] = "True";
    name_to_symbol["KW_class"] = "class";
    name_to_symbol["KW_finally"] = "finally";
    name_to_symbol["KW_is"] = "is";
    name_to_symbol["KW_return"] = "return";
    name_to_symbol["KW_continue"] = "continue";
    name_to_symbol["KW_for"] = "for";
    name_to_symbol["KW_lambda"] = "lambda";
    name_to_symbol["KW_tryas"] = "tryas";
    name_to_symbol["KW_def"] = "def";
    name_to_symbol["KW_from"] = "from";
    name_to_symbol["KW_nonlocal"] = "nonlocal";
    name_to_symbol["KW_while"] = "while";
    name_to_symbol["KW_assert"] = "assert";
    name_to_symbol["KW_del"] = "del";
    name_to_symbol["KW_global"] = "global";
    name_to_symbol["KW_with"] = "with";
    name_to_symbol["KW_async"] = "async";
    name_to_symbol["KW_elif"] = "elif";
    name_to_symbol["KW_if"] = "if";
    name_to_symbol["KW_yield"] = "yield";

    name_to_symbol["DT_int"] = "int";
    name_to_symbol["DT_float"] = "float";
    name_to_symbol["DT_bool"] = "bool";
    name_to_symbol["DT_str"] = "str";
    

    name_to_symbol["INDENT"] = "INDENT";
    name_to_symbol["DEDENT"] = "DEDENT";
}