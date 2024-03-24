#include<bits/stdc++.h>
using namespace std;
// #include "include/error.hpp"

#define RED "\033[1;31m"
#define RESET "\033[0m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define UNDERLINE "\033[4m"
#define MAGENTA "\033[1;35m"
#define CYAN "\033[1;36m"

struct symTable;

typedef struct symRecord {
	string name;
	string type;
	int size;
	int lineno;
	int column;
	int index;
	struct symTable* symTab;
	symRecord(string __name = "", string __type = "", int __size = 0, int __lineno = 0, int __column = 0, struct symTable* __symTab = NULL);
	void generateCSV(ofstream &CSV);

} tableRecord;

typedef struct symTable {
	string name;
	map <string, vector<int>> name_to_indices;
	map<int, tableRecord*> entries;
    vector<int> childIndices;
	struct symTable* parentSymtable;
	int size = 0;
	int offset = 0;
    int currentIndex = 0;
		
	int UpdateRecord(tableRecord* newRecord);
	symTable(string __name, struct symTable* __parentSymtable);

	// here linno and column are those of the entity that is found now and looked up, err shows whether to print error message
	tableRecord* lookup(string name, int line_no, int column, bool err);

	// here linno and column are those of the entity that is found now and looked up, err shows whether to print error message
	tableRecord* lookup(string name, vector<tableRecord*> &params, int line_no, int column, bool err);

	int insert(tableRecord* inputRecord, struct symTable* funcTable);
	void generateCSV(ofstream &CSV);

} symbolTable;

