#include "node.hpp"

#define RED "\033[1;31m"
#define RESET "\033[0m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define UNDERLINE "\033[4m"
#define MAGENTA "\033[1;35m"
#define CYAN "\033[1;36m"

enum tableType{
	
	FUNCTION,
	CLASS,
	GLOBAL,

};

enum recordType{
	
	TYPE_FUNCTION,
	TYPE_CLASS,
	CONST_STRING,
	CLASS_ATTRIBUTE,
	CLASS_OBJECT,
	OBJECT_ATTRIBUTE,
	VARIABLE,
};

struct symTable;

typedef struct symRecord {
	string name;
	string type;
	int size;
	int lineno;
	int column;
	int index;
	int recordType;
	int offset;
	struct symTable* symTab;
	symRecord(string __name = "", string __type = "", int __size = 0, int __lineno = 0, int __column = 0, int recordType = recordType::VARIABLE);
	void dumpCSV(ofstream &CSV);

} tableRecord;

typedef struct symTable {
	string name;
	map <string, vector<int>> name_to_indices;
	map<int, tableRecord*> entries;
    vector<int> childIndices;
	struct symTable* parentSymtable;

	/* defined by enum of tableType */
	int tableType;

	/* for ease, denotes size of function or class */
	int size = 0;
	int numParams = 0;
    int currentIndex = 0;

	/* for static variables of a class */
	set<int> staticIndices;

	/* ctor */
	symTable(string __name, struct symTable* __parentSymtable);

	/* here lineno and column are those of the entity that is found now and looked up,
	err shows whether to print error message
	returns the first matched entry if multiple entries found */
	tableRecord* lookup(string name, int recordType = recordType::VARIABLE, vector<tableRecord*> *params = NULL);
	
	/* lookup only inside the corresponding table, donot go up in the heirarchy,
	returns the first matched entry if multiple entries found */
	tableRecord* lookup_table(string name, int recordType = recordType::VARIABLE, vector<tableRecord*> *params = NULL);

	int insert(tableRecord* inputRecord, struct symTable* funcTable = NULL);
	void dumpCSV(ofstream &CSV);

} symbolTable;

/* genrates the symbol table */
int symTable_Maker(TreeNode *root);
