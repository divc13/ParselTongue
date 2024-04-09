#include "include/symtable.hpp"
#define REG_CNT 16
extern vector<code> threeAC;

vector<instruction> assembly;

int now = 0;
int numParams = 0;
int code_itr = 0;

typedef struct reg_struct
{
	string var = "";
	string name = "";
	int next = -1;
	bool free = true;
	void freeReg();

} reg_struct;

typedef struct var_struct
{
	string name = "";
	int death = INT_MAX;
	int reg = -1;

} var_struct;

map <string, var_struct> varMap;
reg_struct regMap[REG_CNT];

void reg_struct::freeReg()
{
	var = "";
	next = -1;
	free = true;
	return;
}

namespace x86
{
	void Move(string arg1, string arg2, string label);
}

void x86::Move(string arg1, string arg2, string label)
{
	instruction inst;
	inst.label = label;
	inst.first = "movq";
	inst.second = arg1;
	inst.third = arg2;
	assembly.push_back(inst);
}

int allocate(var_struct &var)
{
	// free the registers for died variables
	for (int i = 0; i < REG_CNT; i++)
	{
		if (regMap[i].next < now)
		{
			regMap[i].freeReg();
		}
	}

	// find any free registers
	for (int i = 0; i < REG_CNT; i++)
	{
		if (regMap[i].free)
		{
			var.reg = i;
			return i;
		}

		// load instruction here

	}

	// no free register found, spill the register
	int farthest = 0;
	for (int i = 0; i < REG_CNT; i++)
	{
		if (regMap[i].next > regMap[farthest].next)
		{
			farthest = i;
		}
	}

	// spill the farthest register

	// load instruction here
	
	/////////////////////////////////////////////////// TODO //////////////////////////////////////

	return farthest;

}

int ensure(var_struct var)
{
	for (int i = 0; i < REG_CNT; i++)
	{
		if (regMap[i].var == var.name)
		{
			return i;
		}
	}

	// variable not in register
	int reg = allocate(var);
	return reg;
}

void initialize_assembly()
{
	instruction x86;
	x86.label = ".data";
	x86.first = "";
	x86.second = "";
	x86.third = "";
	assembly.push_back(x86);

	x86.label = "integer_format:";
	x86.first = ".asciz";
	x86.second = ", \"%lld\"";
	x86.third = "";
	assembly.push_back(x86);

	x86.label = "string_format:";
	x86.first = ".asciz";
	x86.second = ", \"%s\"";
	x86.third = "";
	assembly.push_back(x86);

	x86.label = "list_start_format:";
	x86.first = ".asciz";
	x86.second = ", \"[\"";
	x86.third = "";
	assembly.push_back(x86);

	x86.label = "list_end_format:";
	x86.first = ".asciz";
	x86.second = ", \"]\"";
	x86.third = "";
	assembly.push_back(x86);

	x86.label = "comma_format:";
	x86.first = ".asciz";
	x86.second = ", \", \"";
	x86.third = "";
	assembly.push_back(x86);

	x86.label = "end_line_format:";
	x86.first = ".asciz";
	x86.second = ", \"\\n\"";
	x86.third = "";
	assembly.push_back(x86);

	x86.label = ".global";
	x86.first = "main";
	x86.second = "";
	x86.third = "";
	assembly.push_back(x86);

	x86.label = ".text";
	x86.first = "";
	x86.second = "";
	x86.third = "";
	assembly.push_back(x86);

}

void modifier(code tac)
{

	// deal with operators and dereference separately /////////////////////////////////////////

	if (tac.field_1 == "param")
	{
		int cnt = 0;
		int tmp = code_itr;
		for (;tmp < threeAC.size(); tmp++)
		{
			if (threeAC[tmp].field_1 != "param")
			{
				break;
			}
			cnt++;
		}

		for (int i=0; i < cnt; i++)
		{
			int index = code_itr + cnt - i - 1;
			x86::Move();
		}
		
	}

	if (tac.field_1 == "pop_param")
	{
		
	}

	if (tac.field_1 == "push")
	{
		
	}

	if (tac.field_1 == "begin_function")
	{
		
	}

	if (tac.field_1 == "end_function")
	{
		
	}

	if (tac.field_1 == "shift_sp")
	{
		
	}

	if (tac.field_1 == "call")
	{
		
	}

	if (tac.field_1 == "return")
	{
		
	}

	if (tac.field_1 == "if_false")
	{
		
	}

	if (tac.field_1 == "if")
	{
		
	}

	if (tac.field_2 == "=")
	{
		
	}

}

void generate_assembly()
{
	initialize_assembly();
	for (code_itr=0; code_itr< threeAC.size(); code_itr++)
	{
		modifier(threeAC[code_itr]);
	}

	// write code for changing labels ////////////////////////////////////////////////////
}
