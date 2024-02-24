#include<bits/stdc++.h>
#include "parser.tab.h"
using namespace std;

map<string, int> symbol_to_name;
map<int, string> name_to_symbol;

void init_symbol_name_tables()
{
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

    for (auto i = symbol_to_name.begin(); i != symbol_to_name.end(); i++)
    {
        name_to_symbol[i->second] = i->first;
    }    
}