#include "symtable.hpp"

typedef struct reg_struct
{
	string var = "";
	string name = "";
	bool free = true;
	bool allocatable = true;
	void freeReg();

} reg_struct;

typedef struct var_struct
{
	string name = "";
	int death = INT_MAX;
	int reg = -1;
	vector<int> locations;
	int offset = -1;

} var_struct;

bool is_num(string val);
void dumpAC(string file);
void generate_assembly();
void init_tac();
void dump_x86_64(string file);
string newLabel();
string newTmp();
string tableHash(symbolTable* curr);
void allocate_mem(string size);