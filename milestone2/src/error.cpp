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
	ErrorMap[ERR::UNDECLARED] 					=		"Use of undeclared variable ";
	ErrorMap[ERR::REDIFINITION] 				=		"redefinition of ";
	ErrorMap[ERR::TYPE_REDECLARATION] 			=		"a different kind of symbol was already declared as ";
	ErrorMap[ERR::CLASS_NO_MATCH_ATTR] 			=		"None of the attributes of the class match with ";
	ErrorMap[ERR::UNKNOWN_TYPE] 				=		"Use of an unknown type ";
	ErrorMap[ERR::CLASS_ATTRIBUTE_DECL_SELF] 	=		"attribute declaration of class other than self class is forbidden";
	ErrorMap[ERR::CLASS_ATTRIBUTE_DECL_CTOR] 	=		"attribute declaration of self class allowed only within constructor";
	ErrorMap[ERR::CLASS_CTOR] 					=		"constructor function \"__init__\" can only be defined inside a class";
	ErrorMap[ERR::ILL_PARENT] 					=		"a class can only be inherited from a class";
	ErrorMap[ERR::CLASS_NOT_GLOBAL] 			=		"a class can only be declared in a global scope";
}

void init_note()
{
	NoteMap[NOTE::PREV_DECL] 					= 		"previous declaration of ";
}

void raise_error(int err, tableRecord* record)
{
	if(err < ERR::CLASS_ATTRIBUTE_DECL_SELF)
		cout << BLUE << UNDERLINE << inputFile << ":" << record->lineno << ":" << record->column << ":" << RESET << " " << RED << ErrorMap[err] << record->name << ":" << RESET;
	else
		cout << BLUE << UNDERLINE << inputFile << ":" << record->lineno << ":" << record->column << ":" << RESET << " " << RED << ErrorMap[err] << ":" << RESET;
	printErrorLine(record->lineno, record->column);
}

void print_note(int note, tableRecord* record)
{
	cout << BLUE << UNDERLINE << inputFile << ":" << record->lineno << ":" << record->column << ":" << RESET << " " << BLUE << NoteMap[note] << record->name << ":" << RESET;
	printErrorLine(record->lineno, record->column);
}