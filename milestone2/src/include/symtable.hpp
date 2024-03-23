#include<bits/stdc++.h>
using namespace std;

struct symTable;

typedef struct symRecord {
	string name;
	string type;
	int size;
	int lineno;
	int column;
	int index;
	struct symTable* symTab;
	symRecord(string __name = NULL, string __type = NULL, int __size = NULL, int __lineno = NULL, int __column = NULL, struct symTable* __symTab = NULL);
	void generateCSV(ofstream &CSV);

} tableRecord;

typedef struct symTable {
	string name;
	map <string, vector<int>> name_to_indices;
	map<int, tableRecord*> entries;
    vector<int> childIndices;
	struct symTable* parentSymtable;
	int offset = 0;
    int currentIndex = 0;
		
	symTable(string __name, struct symTable* __parentSymtable);
	tableRecord* lookup(string name);
	tableRecord* lookup(string name, vector<tableRecord*> &params);
	int insert(string name, string type, int size, int lineno, int column);
	int insert(string name, string type, int size, int lineno, int column, struct symTable* funcTable);
	void generateCSV(ofstream &CSV);

} symbolTable;