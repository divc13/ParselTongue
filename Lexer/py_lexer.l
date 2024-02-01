%option noyywrap
%option yylineno

%{
    #include <bits/stdc++.h>
    using namespace std;    
%}

IDENTIFIER						[a-zA-Z_][a-zA-Z0-9_]*
DATATYPES						int|float|bool|str|list
KEYWORD							False|await|else|import|None|break|except|in|raise|True|class|finally|is|return|and|continue|for|lambda|tryas|def|from|nonlocal|while|assert|del|global|not|with|async|elif|if|or|yield
DUNDER							__main__|__name__|__init__
CLASS_PRIVATE_NAME				__.*

STRING_LITERAL					{STRINGPREFIX}?({SHORTSTRING}|{LONGSTRING})
STRINGPREFIX					r|u|R|U|f|F|fr|Fr|fR|FR|rf|rF|Rf|RF
SHORTSTRING						"'"{SHORTSTRING_ITEM}*"'"|'"'{SHORTSTRING_ITEM}*'"'
LONGSTRING						"'''"{LONGSTRING_ITEM}*"'''"|'"""'{LONGSTRING_ITEM}*'"""'
SHORTSTRING_ITEM				{SHORTSTRING_CHAR}|{STRING_ESCAPE_SEQ}
LONGSTRING_ITEM					{LONGSTRING_CHAR}|{STRING_ESCAPE_SEQ}
SHORTSTRING_CHAR				[^\\\n\"\']
LONGSTRING_CHAR					[^\\]
STRING_ESCAPE_SEQ				\\.

BYTESLITERAL					{BYTESPREFIX}({SHORTBYTES}|{LONGBYTES})
BYTESPREFIX						b|B|br|Br|bR|BR|rb|rB|Rb|RB
SHORTBYTES						"'"{SHORTBYTESITEM}*"'"|'"'{SHORTBYTESITEM}*'"'
LONGBYTES						"'''"{LONGBYTESITEM}*"'''"|'"""'{LONGBYTESITEM}*'"""'
SHORTBYTESITEM					{SHORTBYTESCHAR}|{BYTESESCAPESEQ}
LONGBYTESITEM					{LONGBYTESCHAR}|{BYTESESCAPESEQ}
SHORTBYTESCHAR					[^\\\n\"\']
LONGBYTESCHAR					[^\\]
BYTESESCAPESEQ					\\.

INTEGER							{DECINTEGER} | {BININTEGER} | {OCTINTEGER} | {HEXINTEGER}
DECINTEGER     					{NONZERODIGIT} ([_]? {DIGIT})* | "0"+ ([_]? "0")*
BININTEGER						"0" [bB] ([_]? {BINDIGIT})+
OCTINTEGER						"0" [oO] ([_]? {OCTDIGIT})+
HEXINTEGER						"0" [xX] ([_]? {HEXDIGIT})+
NONZERODIGIT					[1-9]
DIGIT							[0-9]
BINDIGIT						[01]
OCTDIGIT						[0-7]
HEXDIGIT						{DIGIT}|[a-fA-F]

FLOATNUMBER						{POINTFLOAT} | {EXPONENTFLOAT}
POINTFLOAT						{DIGITPART}? {FRACTION} | {DIGITPART} "."
EXPONENTFLOAT					({DIGITPART} | {POINTFLOAT}) {EXPONENT}
DIGITPART						{DIGIT} ([_]? {DIGIT})*
FRACTION						"." {DIGITPART}
EXPONENT						[eE] [+-]? {DIGITPART}

IMAGNUMBER						({FLOATNUMBER} | {DIGITPART}) ("j" | "J")

ARITHMETIC_OPERATORS		 	"+"|"-"|"*"|"/"|"//"|"%"|"**"
RELATIONAL_OPERATORS 			"=="|"!="|">"|"<"|">="|"<="
LOGICAL_OPERATORS 				and|or|not
BITWISE_OPERATORS 				"&"|"|"|"^"|"~"|"<<"|">>"
ASSIGNMENT_OPERATORS 			"="|"+="|"-="|"*="|"/="|"%="|"**="|"&="|"|="|"^="|"<<="|">>="
DELIMITER 						"("|")"|"["|"]"|"{"|"}"|","|":"|"."|";"|"@"|"->"|"//="|"@="


%%


%%

int main(void)
{
    yylex();
    return 0;
}