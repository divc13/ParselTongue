#include "include/symtable.hpp"
#include "include/error.hpp"

map<int, string> ErrorMap;
map<int, string> NoteMap;

void printErrorLine(int line, int column)
{
	cout << endl;
	cout << setw(10) << right << line << left << setw(6) << CYAN << " | " << lines[line - 1] << RESET;
	cout << setw(17) << right << MAGENTA << " | ";
	
	int i = 0;
	while(i < column - 1) 
	{
		cout << "~";
		i++;
	}

	cout << "^" << endl << RESET;
	cout << endl;
}

void init_error()
{
	// root- -> name printed
	ErrorMap[ERR::UNDECLARED] 					=		"Use of undeclared variable ";
	ErrorMap[ERR::REDIFINITION] 				=		"redefinition of ";
	ErrorMap[ERR::TYPE_REDECLARATION] 			=		"a different kind of symbol was already declared as ";
	ErrorMap[ERR::CLASS_NO_MATCH_ATTR] 			=		"None of the attributes of the class match with ";
	ErrorMap[ERR::UNKNOWN_TYPE] 				=		"Use of an unknown type ";
	ErrorMap[ERR::TYPE_MISMATCH] 				=		"mismatched types, cannot perform operation ";
	ErrorMap[ERR::TYPE_NOT_DECL_PARAM] 			=		"Type was not declared for the parameter ";


	// root- -> name not printed
	ErrorMap[ERR::CLASS_ATTRIBUTE_DECL_SELF] 	=		"attribute declaration of class other than self class is forbidden";
	ErrorMap[ERR::CLASS_ATTRIBUTE_DECL_CTOR] 	=		"attribute declaration of self class allowed only within constructor";
	ErrorMap[ERR::CLASS_CTOR] 					=		"constructor function \"__init__\" can only be defined inside a class";
	ErrorMap[ERR::ILL_PARENT] 					=		"a class can only be inherited from a class";
	ErrorMap[ERR::CLASS_NOT_GLOBAL] 			=		"a class can only be declared in a global scope";
	ErrorMap[ERR::NOT_NUMBER] 					=		"Operation is not supported for the given type";
	ErrorMap[ERR::OVERLOAD] 					=		"Operation cannot be overloaded for the given types";


	// root- -> type printed
	ErrorMap[ERR::INTEGER_EXECTED]				=		"expected an integer, received";
}

void init_note()
{
	NoteMap[NOTE::PREV_DECL] 					= 		"previous declaration of ";
	
	
	NoteMap[NOTE::SHOW_TYPE] 					= 		"found one operand of type ";
}

void raise_error(int err, TreeNode* root)
{
	if (err < ERR::CLASS_ATTRIBUTE_DECL_SELF)
		cout << BLUE << UNDERLINE << inputFile << ":" << root->lineno << ":" << root->column << ":" << RESET << " " << RED << "Error: " << ErrorMap[err] << CYAN << root->name << RESET << RED << ":" << RESET;

	else if (err < ERR::INTEGER_EXECTED)
		cout << BLUE << UNDERLINE << inputFile << ":" << root->lineno << ":" << root->column << ":" << RESET << " " << RED << "Error: " << ErrorMap[err] << ":" << RESET;

	else
		cout << BLUE << UNDERLINE << inputFile << ":" << root->lineno << ":" << root->column << ":" << RESET << " " << RED << "Error: " << ErrorMap[err] << CYAN << root->type << RESET << RED << ":" << RESET;
	
	printErrorLine(root->lineno, root->column);
}

void print_note(int note, tableRecord* record)
{

	cout << BLUE << UNDERLINE << inputFile << ":" << record->lineno << ":" << record->column << ":" << RESET << " " << BLUE << "Note: " << NoteMap[note] << CYAN << record->name  << RESET << BLUE << " :" << RESET;
	if(note < NOTE::SHOW_TYPE)
		printErrorLine(record->lineno, record->column);
}