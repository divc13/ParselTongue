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

// template<typename T, typename... Args>
// void printError(T first, Args... args);

// template<typename T, typename... Args>
// void printErrorMsg(int lineno, int col, T first, Args... args);

void printErrorLine(int line, int column)
{
	cout << setw(10) << right << line << left << setw(6) << CYAN << " | " << lines[line - 1] << RESET;
	cout << setw(17) << right << MAGENTA << " | ";
	
	int i = 0;
	while(i < column - 1) 
	{
		cout << "~";
		i++;
	}

	cout << "^" << endl << RESET;
}

// template void printError();
// template void printErrorMsg();

template<typename T, typename... Args>
void printError(T first, Args... args) {
	cout << first;
	printError(args...);
}

template<typename T, typename... Args>
void printErrorMsg(int lineno, int col, T first, Args... args)
{
	printError(BLUE, UNDERLINE, inputFile, ": ", lineno, ": ", col, ":", RESET, " ", first, args...);
	printErrorLine(lineno, col);
	return;
}
