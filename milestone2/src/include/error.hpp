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
	CANDIDATE,
	TYPE_NOT_DECL_PARAM,
	KEYWORD_DECL,
	RETURN_TYPE_MISMATCH,
	NO_RET_TYPE,
	BAD_LVAL,

	// root- -> name not printed
	ERR_MIN1,
	CLASS_ATTRIBUTE_DECL_SELF,
	CLASS_ATTRIBUTE_DECL_CTOR,
	CLASS_CTOR,
	ILL_PARENT,
	NOT_GLOBAL,
	DUPLICATE_SELF,
	CLASS_NOT_GLOBAL,
	NOT_NUMBER,
	MIXED_LIST,
	NESTED_LIST,
	NO_SELF,
	FUNC_OVERLOAD,
	OP_OVERLOAD,
	EMPTY_LIST,
	LOOP,
	IMPOSSIBLE,
	NESTED_FUNCTION,
	RETURN_NO_FUNC,
	BAD_LIST,


	// root- -> type printed
	ERR_MIN2,
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