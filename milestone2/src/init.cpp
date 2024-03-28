#include "include/symtable.hpp"
#include "include/error.hpp"


extern symbolTable *globTable;
extern map<int, string> recordTypeMap;
extern map<string, int> typeMap;

extern map<int, string> ErrorMap;
extern map<int, string> NoteMap;

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
	ErrorMap[ERR::NOT_ITERATOR] 				=		"iterator cannot be made from object ";
	ErrorMap[ERR::TYPE_NOT_DECL_PARAM] 			=		"Type was not declared for the parameter ";
	ErrorMap[ERR::KEYWORD_DECL] 				=		"Variable cannot be declared with the name as keyword ";


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


	// root- -> type printed
	ErrorMap[ERR::INTEGER_EXPECTED]				=		"expected an integer, received ";
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
	tempRecord = new tableRecord("print", "None", SIZE_INT, 0, 0, recordType::TYPE_FUNCTION);
	tempRecord_int -> recordType = recordType::VARIABLE;
	print_int->insert(tempRecord_int);
	globTable -> insert(tempRecord, print_int);

	symbolTable* print_float = new symbolTable("print", globTable);
	print_float -> numParams = 1;
	print_float -> tableType = tableType::FUNCTION;
	tempRecord -> size = SIZE_FLOAT;
	tempRecord_float -> recordType = recordType::VARIABLE;
	print_float -> insert(tempRecord_float);
	globTable -> insert(tempRecord, print_float);

	symbolTable* print_bool = new symbolTable("print", globTable);
	print_bool -> numParams = 1;
	print_bool -> tableType = tableType::FUNCTION;
	tempRecord -> size = SIZE_BOOL;
	tempRecord_bool -> recordType = recordType::VARIABLE;
	print_bool -> insert(tempRecord_bool);
	globTable -> insert(tempRecord, print_bool);

	symbolTable* print_str = new symbolTable("print", globTable);
	print_str -> numParams = 1;
	print_str -> tableType = tableType::FUNCTION;
	tempRecord -> size = SIZE_PTR;
	tempRecord_str -> recordType = recordType::VARIABLE;
	print_str -> insert(tempRecord_str);
	globTable -> insert(tempRecord, print_str);


	tempRecord -> name = "range";
	tempRecord -> type = "list[int]";
	tempRecord -> size = SIZE_INT;

	symbolTable* range_two_args = new symbolTable("range", globTable);
	range_two_args -> numParams = 2;
	range_two_args -> tableType = tableType::FUNCTION;
	range_two_args -> insert(tempRecord_int);
	tempRecord_int -> name = "#var2";
	range_two_args -> insert(tempRecord_int);
	globTable -> insert(tempRecord, range_two_args);

	symbolTable* range_one_args = new symbolTable("range", globTable);
	range_one_args -> numParams = 1;
	range_one_args -> tableType = tableType::FUNCTION;
	range_one_args -> insert(tempRecord_int);
	globTable -> insert(tempRecord, range_one_args);

	free(tempRecord_int);
	free(tempRecord_float);
	free(tempRecord_bool);
	free(tempRecord_str);
	free(tempRecord);
	
}
