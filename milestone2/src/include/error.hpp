#include <bits/stdc++.h>
using namespace std;

#define RED "\033[1;31m"
#define RESET "\033[0m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define UNDERLINE "\033[4m"
#define MAGENTA "\033[1;35m"
#define CYAN "\033[1;36m"

extern string inputFile;
extern vector<string> lines;

void printErrorLine(int line, int column);

void printError() {cout << endl;};
void printErrorMsg() {cout << endl;};

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

// template<typename T, typename... Args>
// void printError(T first, Args... args) {
// 	cout << first;
// 	printError(args...);
// }

// template<typename T, typename... Args>
// void printErrorMsg(int lineno, int col, T first, Args... args)
// {
// 	printError(BLUE, UNDERLINE, inputFile, ":", lineno, ":", col, ":", RESET, " ", first, args...);
// 	printErrorLine(lineno, col);
// 	return;
// }

map<int, string> ErrorMap;

void init_error()
{
	ErrorMap[0] = "no previous definition found for ";
	ErrorMap[1] = "redefinition of ";
	ErrorMap[2] = "a different kind of symbol was declared as ";
	ErrorMap[3] = "Use of undeclared variable ";
	ErrorMap[4] = "a class type must be declared as ";
	ErrorMap[5] = "attribute declaration of class other than self class is forbidden";
	ErrorMap[6] = "attribute declaration of self class allowed only within constructor";
	ErrorMap[7] = "constructor function \"__init__\" can only be defined inside a class";
	ErrorMap[8] = "None of the attributes of the class match with ";
}

void init_note()
{
	ErrorMap[0] = "previous definition of ";
	ErrorMap[1] = "previous declaration of ";
}

// enum ERR {



// };

// enum NOTE {



// };

void raise_error(int err, tableRecord* record)
{
	cout << BLUE << UNDERLINE << inputFile << ":" << record->lineno << ":" << record->column << ":" << RESET << " " << RED << ErrorMap[err] << record->name << ":" << RESET;
	printErrorLine(record->lineno, record->column);
}

void raise_note(int err, tableRecord* record)
{
	cout << BLUE << UNDERLINE << inputFile << ":" << record->lineno << ":" << record->column << ":" << RESET << " " << BLUE << ErrorMap[err] << record->name << ":" << RESET;
	printErrorLine(record->lineno, record->column);
}