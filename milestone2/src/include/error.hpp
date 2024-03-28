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

enum ERR {
	// root- -> name printed
	UNDECLARED,
	REDIFINITION,
	TYPE_REDECLARATION,
	CLASS_NO_MATCH_ATTR,
	UNKNOWN_TYPE,
	TYPE_MISMATCH,
	NOT_ITERABLE,
	NOT_ITERATOR,
	TYPE_NOT_DECL_PARAM,
	KEYWORD_DECL,


	// root- -> name not printed
	CLASS_ATTRIBUTE_DECL_SELF,
	CLASS_ATTRIBUTE_DECL_CTOR,
	CLASS_CTOR,
	ILL_PARENT,
	DUPLICATE_SELF,
	CLASS_NOT_GLOBAL,
	NOT_NUMBER,
	MIXED_LIST,
	NESTED_LIST,
	FUNC_OVERLOAD,
	OP_OVERLOAD,
	EMPTY_LIST,


	// root- -> type printed
	EXPECTED_INT,
	EXPECTED_BOOL,
	
};

enum NOTE {

	PREV_DECL,
	SHOW_TYPE,

};

void init_error();

void init_note();

void raise_error(int err, TreeNode* root);

void print_note(int note, tableRecord* record);