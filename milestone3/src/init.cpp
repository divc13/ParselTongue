#include "include/code.hpp"
#include "include/error.hpp"
#define REG_CNT 16


extern symbolTable *globTable;
extern map<int, string> recordTypeMap;
extern map<string, int> typeMap;

extern map<int, string> ErrorMap;
extern map<int, string> NoteMap;

extern map<string, int> StringToToken;
extern map<string, string> TokenNameToString;
extern map<string, string> opType;

extern vector<code> threeAC;
extern string MemRg;
extern map<string, string> tempType;
extern vector<reg_struct> regMap;

void init_error()
{
	// root- -> name printed
	ErrorMap[ERR::UNDECLARED] 					=		"Use of undeclared variable ";
	ErrorMap[ERR::REDIFINITION] 				=		"redefinition of ";
	ErrorMap[ERR::TYPE_REDECLARATION] 			=		"a different kind of symbol was already declared as ";
	ErrorMap[ERR::CLASS_NO_MATCH_ATTR] 			=		"None of the attributes of the class match with ";
	ErrorMap[ERR::UNKNOWN_TYPE] 				=		"Use of an unknown type ";
	ErrorMap[ERR::TYPE_MISMATCH] 				=		"mismatched types, cannot perform operation ";
	ErrorMap[ERR::NOT_ITERABLE] 				=		"object not iterable ";
	ErrorMap[ERR::NOT_ITERATOR] 				=		"iterator cannot be made from constant ";
	ErrorMap[ERR::TYPE_NOT_DECL_PARAM] 			=		"Type was not declared for the parameter ";
	ErrorMap[ERR::KEYWORD_DECL] 				=		"Variable cannot be declared with the name as keyword ";
	ErrorMap[ERR::NOT_GLOBAL]	 				=		"No global declaration found for ";
	ErrorMap[ERR::NO_INIT]		 				=		"No initializer found for the class ";
	ErrorMap[ERR::CANDIDATE]	 				=		"ambiguity found in call of overloaded ";
	ErrorMap[ERR::RETURN_TYPE_MISMATCH]	 		=		"return type does not match the function definition ";
	ErrorMap[ERR::NO_RET_TYPE]	 				=		"no return type specified for the function ";
	ErrorMap[ERR::BAD_LVAL]	 					=		"value cannot be assigned to ";
// ErrorMap[ERR::STRAY_CODE]	 				=		"Stray code in class body is not supported such as <variable> ";


	// root- -> name not printed
	ErrorMap[ERR::CLASS_ATTRIBUTE_DECL_SELF] 	=		"attribute declaration of class other than self class is forbidden";
	ErrorMap[ERR::CLASS_ATTRIBUTE_DECL_CTOR] 	=		"attribute declaration of self class allowed only within constructor";
	ErrorMap[ERR::CLASS_CTOR] 					=		"constructor function \"__init__\" can only be defined inside a class";
	ErrorMap[ERR::ILL_PARENT] 					=		"a class can only be inherited from a class";
	ErrorMap[ERR::DUPLICATE_SELF] 				=		"Duplicate argument \'self\' in function definition";
	ErrorMap[ERR::CLASS_NOT_GLOBAL] 			=		"a class can only be declared in a global scope";
	ErrorMap[ERR::NOT_NUMBER] 					=		"Operation is not supported for the given type";
	ErrorMap[ERR::MIXED_LIST] 					=		"Type not compatible for the mixed list";
	ErrorMap[ERR::NESTED_LIST] 					=		"Nested List is not supported for this compiler";
	ErrorMap[ERR::FUNC_OVERLOAD] 				=		"Function cannot be overloaded for the given types";
	ErrorMap[ERR::OP_OVERLOAD] 					=		"Operator cannot be overloaded for the given types";
	ErrorMap[ERR::EMPTY_LIST] 					=		"Empty lists are not supported";
	ErrorMap[ERR::NO_SELF] 						=		"class methods must have self as the first parameter";
	ErrorMap[ERR::LOOP] 						=		"break and continue can only be used when inside a loop";
	ErrorMap[ERR::IMPOSSIBLE] 					=		"you somehow achieved what I thought was impossible";
	ErrorMap[ERR::RETURN_NO_FUNC] 				=		"return statements can only be used when inside a function";
	ErrorMap[ERR::NO_RETURN] 					=		"No return statement in a non-void function";
	ErrorMap[ERR::NESTED_FUNCTION] 				=		"Nesting of functions is not yet supported";
	ErrorMap[ERR::BAD_LIST] 					=		"cannot dereference an element of a variable which is not a list";


	// root- -> type printed
	ErrorMap[ERR::EXPECTED_INT]					=		"expected an integer, received ";
	ErrorMap[ERR::EXPECTED_BOOL]				=		"expected a boolean expression, received ";
}

void init_note()
{
	NoteMap[NOTE::PREV_DECL] 					= 		"previous declaration of ";	
	NoteMap[NOTE::SHOW_TYPE] 					= 		"found one operand of type ";
}



void initTypes()
{
	typeMap["None"]			= 0;



	recordTypeMap[recordType::TYPE_FUNCTION]		=		 "TYPE_FUNCTION";
	recordTypeMap[recordType::TYPE_CLASS]		 	=		 "TYPE_CLASS";
	recordTypeMap[recordType::CONST_STRING]		 	=		 "CONST_STRING";
	recordTypeMap[recordType::CONST_INT]		 	=		 "CONST_INT";
	recordTypeMap[recordType::CONST_FLOAT]		 	=		 "CONST_FLOAT";
	recordTypeMap[recordType::CONST_BOOL]		 	=		 "CONST_BOOL";
	recordTypeMap[recordType::CLASS_ATTRIBUTE]		=		 "CLASS_ATTRIBUTE";
	recordTypeMap[recordType::CLASS_OBJECT]		 	=		 "CLASS_OBJECT";
	recordTypeMap[recordType::CLASS_CONSTRUCTOR]	=		 "CLASS_CONSTRUCTOR";
	recordTypeMap[recordType::OBJECT_ATTRIBUTE]		=		 "OBJECT_ATTRIBUTE";
	recordTypeMap[recordType::VARIABLE]		 		=		 "VARIABLE";
	recordTypeMap[recordType::KEYWORD]		 		=		 "KEYWORD";
	recordTypeMap[recordType::TEMPORARY]		 	=		 "TEMPORARY";


	tableRecord* tempRecord = new tableRecord("False", "", 0, 0, 0, recordType::KEYWORD);
	globTable -> insert(tempRecord, globTable);
	tempRecord -> name = "await";
	globTable -> insert(tempRecord, globTable);
	tempRecord -> name = "else";
	globTable -> insert(tempRecord, globTable);
    tempRecord -> name = "import";
	globTable -> insert(tempRecord, globTable);
    tempRecord -> name = "None";
	globTable -> insert(tempRecord, globTable);
    tempRecord -> name = "break";
	globTable -> insert(tempRecord, globTable);
    tempRecord -> name = "except";
	globTable -> insert(tempRecord, globTable);
    tempRecord -> name = "in";
	globTable -> insert(tempRecord, globTable);
    tempRecord -> name = "raise";
	globTable -> insert(tempRecord, globTable);
    tempRecord -> name = "True";
	globTable -> insert(tempRecord, globTable);
    tempRecord -> name = "class";
	globTable -> insert(tempRecord, globTable);
    tempRecord -> name = "finally";
	globTable -> insert(tempRecord, globTable);
    tempRecord -> name = "is";
	globTable -> insert(tempRecord, globTable);
    tempRecord -> name = "return";
	globTable -> insert(tempRecord, globTable);
    tempRecord -> name = "continue";
	globTable -> insert(tempRecord, globTable);
    tempRecord -> name = "for";
	globTable -> insert(tempRecord, globTable);
    tempRecord -> name = "lambda";
	globTable -> insert(tempRecord, globTable);
    tempRecord -> name = "tryas";
	globTable -> insert(tempRecord, globTable);
    tempRecord -> name = "def";
	globTable -> insert(tempRecord, globTable);
    tempRecord -> name = "from";
	globTable -> insert(tempRecord, globTable);
    tempRecord -> name = "nonlocal";
	globTable -> insert(tempRecord, globTable);
    tempRecord -> name = "while";
	globTable -> insert(tempRecord, globTable);
    tempRecord -> name = "assert";
	globTable -> insert(tempRecord, globTable);
    tempRecord -> name = "del";
	globTable -> insert(tempRecord, globTable);
    tempRecord -> name = "global";
	globTable -> insert(tempRecord, globTable);
    tempRecord -> name = "with";
	globTable -> insert(tempRecord, globTable);
    tempRecord -> name = "async";
	globTable -> insert(tempRecord, globTable);
    tempRecord -> name = "elif";
	globTable -> insert(tempRecord, globTable);
    tempRecord -> name = "if";
	globTable -> insert(tempRecord, globTable);
    tempRecord -> name = "yield";
	globTable -> insert(tempRecord, globTable);
	tempRecord -> name = "__name__";
	tempRecord -> type = "str";
	globTable -> insert(tempRecord, globTable);

	tempRecord -> name = "%lld";
	tempRecord -> recordType = recordType::CONST_STRING;
	globTable -> insert(tempRecord, globTable);
	free(tempRecord);



	
	tableRecord* tempRecord_int = new tableRecord("int", "int", SIZE_INT, 0, 0, recordType::TYPE_CLASS);
	globTable -> insert(tempRecord_int, globTable);
	tableRecord* tempRecord_float = new tableRecord("float", "float", SIZE_FLOAT, 0, 0, recordType::TYPE_CLASS);
	globTable -> insert(tempRecord_float, globTable);
	tableRecord* tempRecord_bool = new tableRecord("bool", "bool", SIZE_BOOL, 0, 0, recordType::TYPE_CLASS);
	globTable -> insert(tempRecord_bool, globTable);
	tableRecord* tempRecord_str = new tableRecord("str", "str", SIZE_PTR, 0, 0, recordType::TYPE_CLASS);
	globTable -> insert(tempRecord_str, globTable);


	tempRecord_int -> name = "#var1";
	tempRecord_float -> name = "#var1";
	tempRecord_bool -> name = "#var1";
	tempRecord_str -> name = "#var1";
	symbolTable* print_int = new symbolTable("print", globTable);
	print_int -> numParams = 1;
	print_int -> tableType = tableType::FUNCTION;
	tempRecord = new tableRecord("print", "None", 0, 0, 0, recordType::TYPE_FUNCTION);
	tempRecord_int -> recordType = recordType::VARIABLE;
	print_int->insert(tempRecord_int);
	print_int->size = 0;
	globTable -> insert(tempRecord, print_int);

	symbolTable* print_float = new symbolTable("print", globTable);
	print_float -> numParams = 1;
	print_float -> tableType = tableType::FUNCTION;
	tempRecord_float -> recordType = recordType::VARIABLE;
	print_float -> insert(tempRecord_float);
	print_float->size = 0;
	globTable -> insert(tempRecord, print_float);

	symbolTable* print_bool = new symbolTable("print", globTable);
	print_bool -> numParams = 1;
	print_bool -> tableType = tableType::FUNCTION;
	tempRecord_bool -> recordType = recordType::VARIABLE;
	print_bool -> insert(tempRecord_bool);
	print_bool->size = 0;
	globTable -> insert(tempRecord, print_bool);

	symbolTable* print_str = new symbolTable("print", globTable);
	print_str -> numParams = 1;
	print_str -> tableType = tableType::FUNCTION;
	tempRecord_str -> recordType = recordType::VARIABLE;
	print_str -> insert(tempRecord_str);
	print_str->size = 0;
	globTable -> insert(tempRecord, print_str);



	
	symbolTable* print_int_list = new symbolTable("print", globTable);
	print_int_list -> numParams = 1;
	print_int_list -> tableType = tableType::FUNCTION;
	tempRecord_int -> type = "list_int_";
	print_int_list -> insert(tempRecord_int);
	print_int_list->size = 0;
	globTable -> insert(tempRecord, print_int_list);

	symbolTable* print_float_list = new symbolTable("print", globTable);
	print_float_list -> numParams = 1;
	print_float_list -> tableType = tableType::FUNCTION;
	tempRecord_float -> type = "list_float_";
	print_float_list -> insert(tempRecord_float);
	print_float_list->size = 0;
	globTable -> insert(tempRecord, print_float_list);

	symbolTable* print_bool_list = new symbolTable("print", globTable);
	print_bool_list -> numParams = 1;
	print_bool_list -> tableType = tableType::FUNCTION;
	tempRecord_bool -> type = "list_bool_";
	print_bool_list -> insert(tempRecord_bool);
	print_bool_list->size = 0;
	globTable -> insert(tempRecord, print_bool_list);

	symbolTable* print_str_list = new symbolTable("print", globTable);
	print_str_list -> numParams = 1;
	print_str_list -> tableType = tableType::FUNCTION;
	tempRecord_str -> type = "list_str_";
	print_str_list -> insert(tempRecord_str);
	print_str_list -> size = 0;
	globTable -> insert(tempRecord, print_str_list);



	tempRecord -> name = "range";
	tempRecord -> type = "list_int_";
	tempRecord_int -> type = "int";

	symbolTable* range_two_args = new symbolTable("range", globTable);
	range_two_args -> numParams = 2;
	range_two_args -> tableType = tableType::FUNCTION;
	range_two_args -> insert(tempRecord_int);
	tempRecord_int -> name = "#var2";
	range_two_args -> insert(tempRecord_int);
	range_two_args -> size = 0;
	globTable -> insert(tempRecord, range_two_args);

	symbolTable* range_one_args = new symbolTable("range", globTable);
	range_one_args -> numParams = 1;
	range_one_args -> tableType = tableType::FUNCTION;
	range_one_args -> insert(tempRecord_int);
	range_one_args -> size = 0;
	globTable -> insert(tempRecord, range_one_args);


	tempRecord -> name = "len";
	tempRecord -> type = "int";
	tempRecord_str -> type = "str";

	symbolTable* lenTable = new symbolTable("len", globTable);
	lenTable -> numParams = 1;
	lenTable -> tableType = tableType::FUNCTION;
	lenTable -> insert(tempRecord_str);
	lenTable -> size = 0;
	globTable -> insert(tempRecord, lenTable);

	tempRecord -> name = "#main";
	tempRecord -> type = "None";
	tempRecord -> recordType = recordType::TYPE_FUNCTION;
	symbolTable* mainTab = new symbolTable("#main", globTable);
	mainTab -> numParams = 0;
	mainTab -> tableType = tableType::FUNCTION;
	mainTab -> size = 0;
	globTable -> insert(tempRecord, mainTab);

	free(tempRecord_int);
	free(tempRecord_float);
	free(tempRecord_bool);
	free(tempRecord_str);
	free(tempRecord);
	
}



#include "parser.tab.h"

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

	
	regMap[RAX].name = "%rax";
	regMap[RAX].free = false;
	regMap[RAX].allocatable = false;
	regMap[RBX].name = "%rbx";
	regMap[RBX].free = false;
	regMap[RBX].allocatable = false;
	regMap[RCX].name = "%rcx";
	regMap[RDX].name = "%rdx";
	regMap[RBP].name = "%rbp";
	regMap[RBP].free = false;
	regMap[RBP].allocatable = false;
	regMap[RSP].name = "%rsp";
	regMap[RSP].free = false;
	regMap[RSP].allocatable = false;
	regMap[RDI].name = "%rdi";
	regMap[RSI].name = "%rsi";
	regMap[R8].name = "%r8";
	regMap[R9].name = "%r9";
	regMap[R10].name = "%r10";
	regMap[R11].name = "%r11";
	regMap[R12].name = "%r12";
	regMap[R13].name = "%r13";
	regMap[R14].name = "%r14";
	regMap[R15].name = "%r15";

}

void init_tac()
{
	/* print_int */

	code inst;

	inst.field_1 = "begin_function";
	inst.field_2 = tableHash(globTable) + "print_Zzint_Nn1";
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	string print_val = "#var1\%print_Zzint_Nn1";
	inst.field_1 = "pop_param";
	inst.field_2 = print_val;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	string a1 = newTmp();
	tempType[a1] = "str";
	inst.field_1 = a1;
	inst.field_2 = "=";
	inst.field_3 = "findAddress";
	inst.field_4 = "integer_format";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "param";
	inst.field_2 = print_val;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "param";
	inst.field_2 = a1;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "call";
	inst.field_2 = "printf";
	inst.field_3 = "2";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);
	
	a1 = newTmp();
	tempType[a1] = "str";
	inst.field_1 = a1;
	inst.field_2 = "=";
	inst.field_3 = "findAddress";
	inst.field_4 = "end_line_format";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "param";
	inst.field_2 = a1;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "call";
	inst.field_2 = "printf";
	inst.field_3 = "1";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "return";
	inst.field_2 = "";
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "end_function";
	inst.field_2 = "";
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	/* print_float */

	inst.field_1 = "begin_function";
	inst.field_2 = tableHash(globTable) + "print_Zzfloat_Nn1";
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	print_val = "#var1\%print_Zzfloat_Nn1";
	inst.field_1 = "pop_param";
	inst.field_2 = print_val;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	a1 = newTmp();
	tempType[a1] = "str";
	inst.field_1 = a1;
	inst.field_2 = "=";
	inst.field_3 = "findAddress";
	inst.field_4 = "float_format";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "param";
	inst.field_2 = print_val;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "param";
	inst.field_2 = a1;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "call";
	inst.field_2 = "printf";
	inst.field_3 = "2";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);
	
	a1 = newTmp();
	tempType[a1] = "str";
	inst.field_1 = a1;
	inst.field_2 = "=";
	inst.field_3 = "findAddress";
	inst.field_4 = "end_line_format";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "param";
	inst.field_2 = a1;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "call";
	inst.field_2 = "printf";
	inst.field_3 = "1";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "return";
	inst.field_2 = "";
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "end_function";
	inst.field_2 = "";
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);



	/* print_str */

	inst.field_1 = "begin_function";
	inst.field_2 = tableHash(globTable) + "print_Zzstr_Nn1";
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	print_val = "#var1\%print_Zzstr_Nn1";
	inst.field_1 = "pop_param";
	inst.field_2 = print_val;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	a1 = newTmp();
	tempType[a1] = "str";
	inst.field_1 = a1;
	inst.field_2 = "=";
	inst.field_3 = "findAddress";
	inst.field_4 = "string_format";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "param";
	inst.field_2 = print_val;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "param";
	inst.field_2 = a1;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "call";
	inst.field_2 = "printf";
	inst.field_3 = "2";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	a1 = newTmp();
	tempType[a1] = "str";
	inst.field_1 = a1;
	inst.field_2 = "=";
	inst.field_3 = "findAddress";
	inst.field_4 = "end_line_format";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "param";
	inst.field_2 = a1;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "call";
	inst.field_2 = "printf";
	inst.field_3 = "1";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "return";
	inst.field_2 = "";
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "end_function";
	inst.field_2 = "";
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	/* print_bool */

	inst.field_1 = "begin_function";
	inst.field_2 = tableHash(globTable) + "print_Zzbool_Nn1";
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	print_val = "#var1\%print_Zzbool_Nn1";
	inst.field_1 = "pop_param";
	inst.field_2 = print_val;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	a1 = newTmp();
	tempType[a1] = "str";
	inst.field_1 = a1;
	inst.field_2 = "=";
	inst.field_3 = "findAddress";
	inst.field_4 = "integer_format";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "param";
	inst.field_2 = print_val;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "param";
	inst.field_2 = a1;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "call";
	inst.field_2 = "printf";
	inst.field_3 = "2";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	a1 = newTmp();
	tempType[a1] = "str";
	inst.field_1 = a1;
	inst.field_2 = "=";
	inst.field_3 = "findAddress";
	inst.field_4 = "end_line_format";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "param";
	inst.field_2 = a1;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "call";
	inst.field_2 = "printf";
	inst.field_3 = "1";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "return";
	inst.field_2 = "";
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "end_function";
	inst.field_2 = "";
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);


	/* print_list[int] */

	inst.field_1 = "begin_function";
	inst.field_2 = tableHash(globTable) + "print_Zzlist_int__Nn1";
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	print_val = "#var1\%print_Zzlist_int__Nn1";
	inst.field_1 = "pop_param";
	inst.field_2 = print_val;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	string print1 = newTmp();
	tempType[print1] = "int";
	inst.field_1 = print1;
	inst.field_2 = "=";
	inst.field_3 = "*(" + print_val + ")";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	string print2 = newTmp();
	tempType[print2] = "int";
	inst.field_1 = print2;
	inst.field_2 = "=";
	inst.field_3 = "1";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	a1 = newTmp();
	tempType[a1] = "str";
	inst.field_1 = a1;
	inst.field_2 = "=";
	inst.field_3 = "findAddress";
	inst.field_4 = "list_start_format";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "param";
	inst.field_2 = a1;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "call";
	inst.field_2 = "printf";
	inst.field_3 = "1";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	string printLab = newLabel();
	string print5 = newTmp();
	tempType[print5] = "int";
	inst.field_1 = print5;
	inst.field_2 = "=";
	inst.field_3 = print2;
	inst.field_4 = "*";
	inst.field_5 = "8";
	inst.label = printLab;
	threeAC.push_back(inst);

	string print3 = newTmp();
	tempType[print3] = "list_int_";
	tempType["*(" + print3 + ")"] = "int";
	inst.field_1 = print3;
	inst.field_2 = "=";
	inst.field_3 = print_val;
	inst.field_4 = "+";
	inst.field_5 = print5;
	inst.label = newLabel();
	threeAC.push_back(inst);

	string print4 = newTmp();
	tempType[print4] = "int";
	inst.field_1 = print4;
	inst.field_2 = "=";
	inst.field_3 = "*(" + print3 + ")";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	a1 = newTmp();
	tempType[a1] = "str";
	inst.field_1 = a1;
	inst.field_2 = "=";
	inst.field_3 = "findAddress";
	inst.field_4 = "integer_format";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "param";
	inst.field_2 = print4;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "param";
	inst.field_2 = a1;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "call";
	inst.field_2 = "printf";
	inst.field_3 = "2";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	a1 = newTmp();
	tempType[a1] = "str";
	inst.field_1 = a1;
	inst.field_2 = "=";
	inst.field_3 = "findAddress";
	inst.field_4 = "comma_format";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "param";
	inst.field_2 = a1;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "call";
	inst.field_2 = "printf";
	inst.field_3 = "1";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = print2;
	inst.field_2 = "=";
	inst.field_3 = print2;
	inst.field_4 = "+";
	inst.field_5 = "1";
	inst.label = newLabel();
	threeAC.push_back(inst);

	string print7 = newTmp();
	tempType[print7] = "int";
	inst.field_1 = print7;
	inst.field_2 = "=";
	inst.field_3 = print1;
	inst.field_4 = "+";
	inst.field_5 = "1";
	inst.label = newLabel();
	threeAC.push_back(inst);

	string print6 = newTmp();
	tempType[print6] = "bool";
	inst.field_1 = print6;
	inst.field_2 = "=";
	inst.field_3 = print2;
	inst.field_4 = "<";
	inst.field_5 = print7;
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "if";
	inst.field_2 = print6;
	inst.field_3 = "goto";
	inst.field_4 = printLab;
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	a1 = newTmp();
	tempType[a1] = "str";
	inst.field_1 = a1;
	inst.field_2 = "=";
	inst.field_3 = "findAddress";
	inst.field_4 = "list_end_format";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);
	
	inst.field_1 = "param";
	inst.field_2 = a1;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "call";
	inst.field_2 = "printf";
	inst.field_3 = "1";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	a1 = newTmp();
	tempType[a1] = "str";
	inst.field_1 = a1;
	inst.field_2 = "=";
	inst.field_3 = "findAddress";
	inst.field_4 = "end_line_format";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "param";
	inst.field_2 = a1;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "call";
	inst.field_2 = "printf";
	inst.field_3 = "1";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "return";
	inst.field_2 = "";
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "end_function";
	inst.field_2 = "";
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);


	/* print_list[str] */

	inst.field_1 = "begin_function";
	inst.field_2 = tableHash(globTable) + "print_Zzlist_str__Nn1";
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	print_val = "#var1\%print_Zzlist_str__Nn1";
	inst.field_1 = "pop_param";
	inst.field_2 = print_val;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	print1 = newTmp();
	tempType[print1] = "str";
	inst.field_1 = print1;
	inst.field_2 = "=";
	inst.field_3 = "*(" + print_val + ")";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	print2 = newTmp();
	tempType[print2] = "int";
	inst.field_1 = print2;
	inst.field_2 = "=";
	inst.field_3 = "1";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);


	a1 = newTmp();
	tempType[a1] = "str";
	inst.field_1 = a1;
	inst.field_2 = "=";
	inst.field_3 = "findAddress";
	inst.field_4 = "list_start_format";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "param";
	inst.field_2 = a1;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "call";
	inst.field_2 = "printf";
	inst.field_3 = "1";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	printLab = newLabel();
	print5 = newTmp();
	tempType[print5] = "int";
	inst.field_1 = print5;
	inst.field_2 = "=";
	inst.field_3 = print2;
	inst.field_4 = "*";
	inst.field_5 = "8";
	inst.label = printLab;
	threeAC.push_back(inst);

	print3 = newTmp();
	tempType[print3] = "list_str_";
	tempType["*(" + print3 + ")"] = "str";
	inst.field_1 = print3;
	inst.field_2 = "=";
	inst.field_3 = print_val;
	inst.field_4 = "+";
	inst.field_5 = print5;
	inst.label = newLabel();
	threeAC.push_back(inst);

	print4 = newTmp();
	tempType[print4] = "str";
	inst.field_1 = print4;
	inst.field_2 = "=";
	inst.field_3 = "*(" + print3 + ")";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	a1 = newTmp();
	tempType[a1] = "str";
	inst.field_1 = a1;
	inst.field_2 = "=";
	inst.field_3 = "findAddress";
	inst.field_4 = "string_format";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "param";
	inst.field_2 = print4;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "param";
	inst.field_2 = a1;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "call";
	inst.field_2 = "printf";
	inst.field_3 = "2";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	a1 = newTmp();
	tempType[a1] = "str";
	inst.field_1 = a1;
	inst.field_2 = "=";
	inst.field_3 = "findAddress";
	inst.field_4 = "comma_format";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);
	
	inst.field_1 = "param";
	inst.field_2 = a1;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "call";
	inst.field_2 = "printf";
	inst.field_3 = "1";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = print2;
	inst.field_2 = "=";
	inst.field_3 = print2;
	inst.field_4 = "+";
	inst.field_5 = "1";
	inst.label = newLabel();
	threeAC.push_back(inst);

	print7 = newTmp();
	tempType[print7] = "int";
	inst.field_1 = print7;
	inst.field_2 = "=";
	inst.field_3 = print1;
	inst.field_4 = "+";
	inst.field_5 = "1";
	inst.label = newLabel();
	threeAC.push_back(inst);

	print6 = newTmp();
	tempType[print6] = "bool";
	inst.field_1 = print6;
	inst.field_2 = "=";
	inst.field_3 = print2;
	inst.field_4 = "<";
	inst.field_5 = print7;
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "if";
	inst.field_2 = print6;
	inst.field_3 = "goto";
	inst.field_4 = printLab;
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);
	
	a1 = newTmp();
	tempType[a1] = "str";
	inst.field_1 = a1;
	inst.field_2 = "=";
	inst.field_3 = "findAddress";
	inst.field_4 = "list_end_format";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "param";
	inst.field_2 = a1;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "call";
	inst.field_2 = "printf";
	inst.field_3 = "1";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	a1 = newTmp();
	tempType[a1] = "str";
	inst.field_1 = a1;
	inst.field_2 = "=";
	inst.field_3 = "findAddress";
	inst.field_4 = "end_line_format";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "param";
	inst.field_2 = a1;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "call";
	inst.field_2 = "printf";
	inst.field_3 = "1";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "return";
	inst.field_2 = "";
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "end_function";
	inst.field_2 = "";
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);



	/* print_list[float] */

	inst.field_1 = "begin_function";
	inst.field_2 = tableHash(globTable) + "print_Zzlist_float__Nn1";
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	print_val = "#var1\%print_Zzlist_float__Nn1";
	inst.field_1 = "pop_param";
	inst.field_2 = print_val;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	print1 = newTmp();
	tempType[print1] = "float";
	inst.field_1 = print1;
	inst.field_2 = "=";
	inst.field_3 = "*(" + print_val + ")";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	print2 = newTmp();
	tempType[print2] = "int";
	inst.field_1 = print2;
	inst.field_2 = "=";
	inst.field_3 = "1";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);


	a1 = newTmp();
	tempType[a1] = "str";
	inst.field_1 = a1;
	inst.field_2 = "=";
	inst.field_3 = "findAddress";
	inst.field_4 = "list_start_format";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "param";
	inst.field_2 = a1;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "call";
	inst.field_2 = "printf";
	inst.field_3 = "1";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	printLab = newLabel();
	print5 = newTmp();
	tempType[print5] = "int";
	inst.field_1 = print5;
	inst.field_2 = "=";
	inst.field_3 = print2;
	inst.field_4 = "*";
	inst.field_5 = "8";
	inst.label = printLab;
	threeAC.push_back(inst);

	print3 = newTmp();
	tempType[print3] = "list_str_";
	tempType["*(" + print3 + ")"] = "float";
	inst.field_1 = print3;
	inst.field_2 = "=";
	inst.field_3 = print_val;
	inst.field_4 = "+";
	inst.field_5 = print5;
	inst.label = newLabel();
	threeAC.push_back(inst);

	print4 = newTmp();
	tempType[print4] = "float";
	inst.field_1 = print4;
	inst.field_2 = "=";
	inst.field_3 = "*(" + print3 + ")";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	a1 = newTmp();
	tempType[a1] = "str";
	inst.field_1 = a1;
	inst.field_2 = "=";
	inst.field_3 = "findAddress";
	inst.field_4 = "float_format";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "param";
	inst.field_2 = print4;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "param";
	inst.field_2 = a1;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "call";
	inst.field_2 = "printf";
	inst.field_3 = "2";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	a1 = newTmp();
	tempType[a1] = "str";
	inst.field_1 = a1;
	inst.field_2 = "=";
	inst.field_3 = "findAddress";
	inst.field_4 = "comma_format";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);
	
	inst.field_1 = "param";
	inst.field_2 = a1;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "call";
	inst.field_2 = "printf";
	inst.field_3 = "1";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = print2;
	inst.field_2 = "=";
	inst.field_3 = print2;
	inst.field_4 = "+";
	inst.field_5 = "1";
	inst.label = newLabel();
	threeAC.push_back(inst);

	print7 = newTmp();
	tempType[print7] = "int";
	inst.field_1 = print7;
	inst.field_2 = "=";
	inst.field_3 = print1;
	inst.field_4 = "+";
	inst.field_5 = "1";
	inst.label = newLabel();
	threeAC.push_back(inst);

	print6 = newTmp();
	tempType[print6] = "bool";
	inst.field_1 = print6;
	inst.field_2 = "=";
	inst.field_3 = print2;
	inst.field_4 = "<";
	inst.field_5 = print7;
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "if";
	inst.field_2 = print6;
	inst.field_3 = "goto";
	inst.field_4 = printLab;
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);
	
	a1 = newTmp();
	tempType[a1] = "str";
	inst.field_1 = a1;
	inst.field_2 = "=";
	inst.field_3 = "findAddress";
	inst.field_4 = "list_end_format";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "param";
	inst.field_2 = a1;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "call";
	inst.field_2 = "printf";
	inst.field_3 = "1";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	a1 = newTmp();
	tempType[a1] = "str";
	inst.field_1 = a1;
	inst.field_2 = "=";
	inst.field_3 = "findAddress";
	inst.field_4 = "end_line_format";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "param";
	inst.field_2 = a1;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "call";
	inst.field_2 = "printf";
	inst.field_3 = "1";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "return";
	inst.field_2 = "";
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "end_function";
	inst.field_2 = "";
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);


	/* print_list[bool] */

	inst.field_1 = "begin_function";
	inst.field_2 = tableHash(globTable) + "print_Zzlist_bool__Nn1";
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	print_val = "#var1\%print_Zzlist_bool__Nn1";
	inst.field_1 = "pop_param";
	inst.field_2 = print_val;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	print1 = newTmp();
	tempType[print1] = "bool";
	inst.field_1 = print1;
	inst.field_2 = "=";
	inst.field_3 = "*(" + print_val + ")";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	print2 = newTmp();
	tempType[print2] = "int";
	inst.field_1 = print2;
	inst.field_2 = "=";
	inst.field_3 = "1";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);


	a1 = newTmp();
	tempType[a1] = "str";
	inst.field_1 = a1;
	inst.field_2 = "=";
	inst.field_3 = "findAddress";
	inst.field_4 = "list_start_format";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "param";
	inst.field_2 = a1;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "call";
	inst.field_2 = "printf";
	inst.field_3 = "1";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	printLab = newLabel();
	print5 = newTmp();
	tempType[print5] = "int";
	inst.field_1 = print5;
	inst.field_2 = "=";
	inst.field_3 = print2;
	inst.field_4 = "*";
	inst.field_5 = "8";
	inst.label = printLab;
	threeAC.push_back(inst);

	print3 = newTmp();
	tempType[print3] = "list_bool_";
	tempType["*(" + print3 + ")"] = "bool";
	inst.field_1 = print3;
	inst.field_2 = "=";
	inst.field_3 = print_val;
	inst.field_4 = "+";
	inst.field_5 = print5;
	inst.label = newLabel();
	threeAC.push_back(inst);

	print4 = newTmp();
	tempType[print4] = "bool";
	inst.field_1 = print4;
	inst.field_2 = "=";
	inst.field_3 = "*(" + print3 + ")";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	a1 = newTmp();
	tempType[a1] = "str";
	inst.field_1 = a1;
	inst.field_2 = "=";
	inst.field_3 = "findAddress";
	inst.field_4 = "integer_format";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "param";
	inst.field_2 = print4;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "param";
	inst.field_2 = a1;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "call";
	inst.field_2 = "printf";
	inst.field_3 = "2";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	a1 = newTmp();
	tempType[a1] = "str";
	inst.field_1 = a1;
	inst.field_2 = "=";
	inst.field_3 = "findAddress";
	inst.field_4 = "comma_format";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);
	
	inst.field_1 = "param";
	inst.field_2 = a1;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "call";
	inst.field_2 = "printf";
	inst.field_3 = "1";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = print2;
	inst.field_2 = "=";
	inst.field_3 = print2;
	inst.field_4 = "+";
	inst.field_5 = "1";
	inst.label = newLabel();
	threeAC.push_back(inst);

	print7 = newTmp();
	tempType[print7] = "int";
	inst.field_1 = print7;
	inst.field_2 = "=";
	inst.field_3 = print1;
	inst.field_4 = "+";
	inst.field_5 = "1";
	inst.label = newLabel();
	threeAC.push_back(inst);

	print6 = newTmp();
	tempType[print6] = "bool";
	inst.field_1 = print6;
	inst.field_2 = "=";
	inst.field_3 = print2;
	inst.field_4 = "<";
	inst.field_5 = print7;
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "if";
	inst.field_2 = print6;
	inst.field_3 = "goto";
	inst.field_4 = printLab;
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);
	
	a1 = newTmp();
	tempType[a1] = "str";
	inst.field_1 = a1;
	inst.field_2 = "=";
	inst.field_3 = "findAddress";
	inst.field_4 = "list_end_format";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "param";
	inst.field_2 = a1;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "call";
	inst.field_2 = "printf";
	inst.field_3 = "1";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	a1 = newTmp();
	tempType[a1] = "str";
	inst.field_1 = a1;
	inst.field_2 = "=";
	inst.field_3 = "findAddress";
	inst.field_4 = "end_line_format";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "param";
	inst.field_2 = a1;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "call";
	inst.field_2 = "printf";
	inst.field_3 = "1";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "return";
	inst.field_2 = "";
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "end_function";
	inst.field_2 = "";
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);


	/* range_int */

	inst.field_1 = "begin_function";
	inst.field_2 = tableHash(globTable) + "range_Zzint_Nn1";
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	string val = "#var2\%range_Zzint_Nn1";
	inst.field_1 = "pop_param";
	inst.field_2 = val;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	string tmpry = newTmp();
	tempType[tmpry] = "int";
	inst.field_1 = tmpry;
	inst.field_2 = "=";
	inst.field_3 = val;
	inst.field_4 = "+";
	inst.field_5 = "1";
	inst.label = newLabel();
	threeAC.push_back(inst);

	string tmpry_2 = newTmp();
	tempType[tmpry_2] = "int";
	inst.field_1 = tmpry_2;
	inst.field_2 = "=";
	inst.field_3 = tmpry;
	inst.field_4 = "*";
	inst.field_5 = "8";
	inst.label = newLabel();
	threeAC.push_back(inst);

	allocate_mem(tmpry_2);

	string l1 = newLabel();
	string t1 = newTmp();
	tempType[t1] = "list_int_";
	tempType["*(" + t1 + ")"] = "int";
	string t2 = newTmp();
	tempType[t2] = "int";
	string t3 = newTmp();
	tempType[t3] = "bool";
	string t4 = newTmp();
	tempType[t4] = "int";
	t1 = MemRg;

	inst.field_1 = "*(" + t1 + ")";
	inst.field_2 = "=";
	inst.field_3 = val;
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = t2;
	inst.field_2 = "=";
	inst.field_3 = "1";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = t4;
	inst.field_2 = "=";
	inst.field_3 = t2;
	inst.field_4 = "*";
	inst.field_5 = "8";
	inst.label = l1;
	threeAC.push_back(inst);

	string loc = newTmp();
	tempType[loc] = "list_int_";
	tempType["*(" + loc + ")"] = "int";
	inst.field_1 = loc;
	inst.field_2 = "=";
	inst.field_3 = t1;
	inst.field_4 = "+";
	inst.field_5 = t4;
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "*(" + loc + ")";
	inst.field_2 = "=";
	inst.field_3 = t2;
	inst.field_4 = "-";
	inst.field_5 = "1";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = t2;
	inst.field_2 = "=";
	inst.field_3 = t2;
	inst.field_4 = "+";
	inst.field_5 = "1";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = t3;
	inst.field_2 = "=";
	inst.field_3 = t2;
	inst.field_4 = "<";
	inst.field_5 = tmpry;
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "if";
	inst.field_2 = t3;
	inst.field_3 = "goto";
	inst.field_4 = l1;
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "push";
	inst.field_2 = t1;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "return";
	inst.field_2 = "";
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "end_function";
	inst.field_2 = "";
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);



	/* range (int x, int y) */
	
	inst.field_1 = "begin_function";
	inst.field_2 = tableHash(globTable) + "range_Zzint_Zzint_Nn2";
	inst.label = newLabel();
	threeAC.push_back(inst);

	string val1 = "#var1\%range_Zzint_Zzint_Nn2";
	string val2 = "#var2\%range_Zzint_Zzint_Nn2";
	string val3 = newTmp();
	tempType[val3] = "int";
	tmpry_2 = newTmp();
	tempType[tmpry_2] = "int";
	string tmpry_3 = newTmp();
	tempType[tmpry_3] = "int";
	string tmpry_4 = newTmp();
	tempType[tmpry_4] = "int";

	inst.field_1 = "pop_param";
	inst.field_2 = val1;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "pop_param";
	inst.field_2 = val2;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = val3;
	inst.field_2 = "=";
	inst.field_3 = val2;
	inst.field_4 = "-";
	inst.field_5 = val1;
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = tmpry_2;
	inst.field_2 = "=";
	inst.field_3 = val3;
	inst.field_4 = "+";
	inst.field_5 = "1";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = tmpry_3;
	inst.field_2 = "=";
	inst.field_3 = tmpry_2;
	inst.field_4 = "*";
	inst.field_5 = "8";
	inst.label = newLabel();
	threeAC.push_back(inst);

	string l2 = newLabel();
	string t5 = newTmp();
	tempType[t5] = "list_int_";
	tempType["*(" + t5 +  ")"] = "list_int_";
	string t6 = newTmp();
	tempType[t6] = "int";
	string t7 = newTmp();
	tempType[t7] = "bool";
	string t8 = newTmp();
	tempType[t8] = "int";
	string t9 = newTmp();
	tempType[t9] = "int";
	string t10 = newTmp();
	tempType[t10] = "list_int_";
	tempType["*(" + t10 +  ")"] = "list_int_";

	allocate_mem(tmpry_3);
	t5 = MemRg;

	inst.field_1 = t6;
	inst.field_2 = "=";
	inst.field_3 = val1;
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "*(" + t5 +  ")";
	inst.field_2 = "=";
	inst.field_3 = val3;
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = t9;
	inst.field_2 = "=";
	inst.field_3 = t6;
	inst.field_4 = "-";
	inst.field_5 = val1;
	inst.label = l2;
	threeAC.push_back(inst);

	inst.field_1 = tmpry_4;
	inst.field_2 = "=";
	inst.field_3 = t9;
	inst.field_4 = "+";
	inst.field_5 = "1";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = t8;
	inst.field_2 = "=";
	inst.field_3 = tmpry_4;
	inst.field_4 = "*";
	inst.field_5 = "8";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = t10;
	inst.field_2 = "=";
	inst.field_3 = t5;
	inst.field_4 = "+";
	inst.field_5 = t8;
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "*(" + t10 + ")";
	inst.field_2 = "=";
	inst.field_3 = t6;
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = t6;
	inst.field_2 = "=";
	inst.field_3 = t6;
	inst.field_4 = "+";
	inst.field_5 = "1";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = t7;
	inst.field_2 = "=";
	inst.field_3 = t6;
	inst.field_4 = "<";
	inst.field_5 = val2;
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "if";
	inst.field_2 = t7;
	inst.field_3 = "goto";
	inst.field_4 = l2;
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "push";
	inst.field_2 = t5;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "return";
	inst.field_2 = "";
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "end_function";
	inst.field_2 = "";
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);


	/* len (str) */

	inst.field_1 = "begin_function";
	inst.field_2 = tableHash(globTable) + "len_Zzstr_Nn1";
	inst.label = newLabel();
	threeAC.push_back(inst);

	string val7 = "#var1\%len_Zzstr_Nn1";
	string val4 = newTmp();
	tempType[val4] = "int";

	inst.field_1 = "pop_param";
	inst.field_2 = val7;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = val7;
	inst.field_2 = "=";
	inst.field_3 = val7;
	inst.field_4 = "-";
	inst.field_5 = "8";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = val4;
	inst.field_2 = "=";
	inst.field_3 = "*(" + val7 + ")";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "push";
	inst.field_2 = val4;
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "return";
	inst.field_2 = "";
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "end_function";
	inst.field_2 = "";
	inst.field_3 = "";
	inst.field_4 = "";
	inst.field_5 = "";
	inst.label = newLabel();
	threeAC.push_back(inst);

	for(auto i: typeMap)
	{
		if((i.first).compare(0, 4, "list") == 0)
		{
			string type = i.first;

			inst.field_1 = "begin_function";
			inst.field_2 = tableHash(globTable) + "len_Zz" + type + "_Nn1";
			inst.label = newLabel();
			threeAC.push_back(inst);

			string val5 = "#len\%len_Zz" + type + "_Nn1";
			string val6 = newTmp();
			tempType[t10] = type;

			inst.field_1 = "pop_param";
			inst.field_2 = val5;
			inst.field_3 = "";
			inst.field_4 = "";
			inst.field_5 = "";
			inst.label = newLabel();
			threeAC.push_back(inst);

			inst.field_1 = val6;
			inst.field_2 = "=";
			inst.field_3 = "*(" + val5 + ")";
			inst.field_4 = "";
			inst.field_5 = "";
			inst.label = newLabel();
			threeAC.push_back(inst);

			inst.field_1 = "push";
			inst.field_2 = val6;
			inst.field_3 = "";
			inst.field_4 = "";
			inst.field_5 = "";
			inst.label = newLabel();
			threeAC.push_back(inst);

			inst.field_1 = "return";
			inst.field_2 = "";
			inst.field_3 = "";
			inst.field_4 = "";
			inst.field_5 = "";
			inst.label = newLabel();
			threeAC.push_back(inst);

			inst.field_1 = "end_function";
			inst.field_2 = "";
			inst.field_3 = "";
			inst.field_4 = "";
			inst.field_5 = "";
			inst.label = newLabel();
			threeAC.push_back(inst);

		}
	}
}