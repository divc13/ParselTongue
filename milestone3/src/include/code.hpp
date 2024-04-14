#include "symtable.hpp"

enum regs{
	RAX,
	RBX,
	RBP,
	RSP,
	RDI,
	RSI,
	RDX,
	RCX,
	R8,
	R9,
	R10,
	R11,
	R12,
	R13,
	R14,
	R15,
	REG_MAX,
};

typedef struct address_code {

	string label = "";
	string field_1 = "";
	string field_2 = "";
	string field_3 = "";
	string field_4 = "";
	string field_5 = "";

} code;

typedef struct x86_code
{
	string label = "";
	string first = "";
	string second = "";
	string third = "";
	string comment = "";

} instruction;

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
bool test(string name);