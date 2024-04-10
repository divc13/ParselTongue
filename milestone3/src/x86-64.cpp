#include "include/symtable.hpp"
extern vector<code> threeAC;
extern symbolTable* globTable;

vector<instruction> assembly;
set<int> BB_leaders;

int now = 0;
int numParams = 0;
int code_itr = 0;

// indicates the difference between rsp and rbp at curr time
int gap = 0;

map <string, var_struct> varMap;
reg_struct regMap[REG_MAX];

void reg_struct::freeReg()
{
	var = "";
	free = true;
	return;
}

namespace x86
{
	void Move(string arg1, string arg2, string label);
	void Add(string arg1, string arg2, string label);
	void Sub(string arg1, string arg2, string label);
	void Cmp(string arg1, string arg2, string label);
	void Mul(string arg1, string arg2, string label);
	void Div(string arg1, string label);
	void And(string arg1, string arg2, string label);
	void Or(string arg1, string arg2, string label);
	void Xor(string arg1, string arg2, string label);
	void Not(string arg1, string label);
	void Shl(string arg1, string arg2, string label);
	void Shr(string arg1, string arg2, string label);
	void Push(string arg1, string label);
	void Label(string label);
	void Call(string arg1, string label);
	void Ret(string label);
	void CLTD(string label);
	void Cmove(string arg1, string arg2, string label);
	void Cmovne(string arg1, string arg2, string label);
	void Cmovg(string arg1, string arg2, string label);
	void Cmovl(string arg1, string arg2, string label);
	void Cmovge(string arg1, string arg2, string label);
	void Cmovle(string arg1, string arg2, string label);
	void Je(string arg1, string label);
	void Jne(string arg1, string label);
	void Spill(int reg, string label);
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

void x86::Add(string arg1, string arg2, string label)
{
	instruction inst;
	inst.label = label;
	inst.first = "addq";
	inst.second = arg1;
	inst.third = arg2;
	assembly.push_back(inst);
}

void x86::Sub(string arg1, string arg2, string label)
{
	instruction inst;
	inst.label = label;
	inst.first = "subq";
	inst.second = arg1;
	inst.third = arg2;
	assembly.push_back(inst);
}

void x86::Cmp(string arg1, string arg2, string label)
{
	instruction inst;
	inst.label = label;
	inst.first = "cmp";
	inst.second = arg1;
	inst.third = arg2;
	assembly.push_back(inst);
}

void x86::Mul(string arg1, string arg2, string label)
{
	instruction inst;
	inst.label = label;
	inst.first = "imul";
	inst.second = arg1;
	inst.third = arg2;
	assembly.push_back(inst);
}

void x86::Div(string arg1, string label)
{
	instruction inst;
	inst.label = label;
	inst.first = "idiv";
	inst.second = arg1;
	inst.third = "";
	assembly.push_back(inst);
}

void x86::And(string arg1, string arg2, string label)
{
	instruction inst;
	inst.label = label;
	inst.first = "and";
	inst.second = arg1;
	inst.third = arg2;
	assembly.push_back(inst);
}

void x86::Or(string arg1, string arg2, string label)
{
	instruction inst;
	inst.label = label;
	inst.first = "or";
	inst.second = arg1;
	inst.third = arg2;
	assembly.push_back(inst);
}

void x86::Xor(string arg1, string arg2, string label)
{
	instruction inst;
	inst.label = label;
	inst.first = "xor";
	inst.second = arg1;
	inst.third = arg2;
	assembly.push_back(inst);
}

void x86::Not(string arg1, string label)
{
	instruction inst;
	inst.label = label;
	inst.first = "not";
	inst.second = arg1;
	assembly.push_back(inst);
}

void x86::Shl(string arg1, string arg2, string label)
{
	instruction inst;
	inst.label = label;
	inst.first = "shl";
	inst.second = arg1;
	inst.third = arg2;
	assembly.push_back(inst);
}

void x86::Shr(string arg1, string arg2, string label)
{
	instruction inst;
	inst.label = label;
	inst.first = "shr";
	inst.second = arg1;
	inst.third = arg2;
	assembly.push_back(inst);
}

void x86::Push(string arg1, string label)
{
	instruction inst;
	inst.label = label;
	inst.first = "pushq";
	inst.second = arg1;
	assembly.push_back(inst);
}

void x86::Label(string label)
{
	instruction inst;
	inst.label = label;
	assembly.push_back(inst);
}

void x86::Call(string arg1, string label)
{
	instruction inst;
	inst.label = label;
	inst.first = "call";
	inst.second = arg1;
	assembly.push_back(inst);
}

void x86::Ret(string label)
{
	instruction inst;
	inst.label = label;
	inst.first = "leave";
	assembly.push_back(inst);
	inst.first = "ret";
	assembly.push_back(inst);
}

void x86::CLTD(string label)
{
	instruction inst;
	inst.label = label;
	inst.first = "cldt";
	assembly.push_back(inst);
}
void x86::Cmove(string arg1, string arg2, string label)
{
	instruction inst;
	inst.label = label;
	inst.first = "cmove";
	inst.second = arg1;
	inst.third = arg2;
	assembly.push_back(inst);
}
void x86::Cmovne(string arg1, string arg2, string label)
{
	instruction inst;
	inst.label = label;
	inst.first = "cmovne";
	inst.second = arg1;
	inst.third = arg2;
	assembly.push_back(inst);
}
void x86::Cmovg(string arg1, string arg2, string label)
{
	instruction inst;
	inst.label = label;
	inst.first = "cmovg";
	inst.second = arg1;
	inst.third = arg2;
	assembly.push_back(inst);
}
void x86::Cmovl(string arg1, string arg2, string label)
{
	instruction inst;
	inst.label = label;
	inst.first = "cmovl";
	inst.second = arg1;
	inst.third = arg2;
	assembly.push_back(inst);
}
void x86::Cmovge(string arg1, string arg2, string label)
{
	instruction inst;
	inst.label = label;
	inst.first = "cmovge";
	inst.second = arg1;
	inst.third = arg2;
	assembly.push_back(inst);
}
void x86::Cmovle(string arg1, string arg2, string label)
{
	instruction inst;
	inst.label = label;
	inst.first = "cmovle";
	inst.second = arg1;
	inst.third = arg2;
	assembly.push_back(inst);
}
void x86::Je(string arg1, string label)
{
	instruction inst;
	inst.label = label;
	inst.first = "je";
	inst.second = arg1;
	assembly.push_back(inst);
}
void x86::Jne(string arg1, string label)
{
	instruction inst;
	inst.label = label;
	inst.first = "je";
	inst.second = arg1;
	assembly.push_back(inst);
}

void x86::Spill(int reg, string label)
{
	string var = regMap[reg].var;
	int offset = varMap[var].offset;

	// a temporary which is not on the stack
	if (offset == -1)
	{
		x86::Push(regMap[reg].name, label);
		gap = gap + 8;
		varMap[var].offset = gap;
		return;
	}

	string first = "-" + to_string(offset) + "(%rbp)";
	x86::Move(regMap[reg].name, first, label);
	regMap[reg].freeReg();
}

int allocate(var_struct &var, string label)
{
	// free the registers for died variables
	for (int i = R10; i < REG_MAX; i++)
	{
		if (!regMap[i].free && varMap[regMap[i].var].death < now)
		{
			regMap[i].freeReg();
		}
	}

	// find any free registers
	for (int i = R10; i < REG_MAX; i++)
	{
		if (regMap[i].free)
		{
			var.reg = i;
			regMap[i].free = false;
			regMap[i].var = var.name;

			// a temporary on stack or a local variable
			if (var.offset != -1)
			{
				// load instruction here
				string first = "-" + to_string(var.offset) + "(%rbp)";
				x86::Move(first, regMap[i].name, label);
			}
			return i;
		}
	}

	// no free register found, spill the register
	int farthest = R10;
	vector<int> locs = varMap[regMap[farthest].var].locations;
	int last_next = locs[lower_bound(locs.begin(), locs.end(), now) - locs.begin()];
	for (int i = R10; i < REG_MAX; i++)
	{
		locs = varMap[regMap[i].var].locations;
		int next = locs[lower_bound(locs.begin(), locs.end(), now) - locs.begin()];
		if (next > last_next)
		{
			farthest = i;
			last_next = next;
		}
	}

	// spill the farthest register
	x86::Spill(farthest, label);

	// load instruction here
	var.reg = farthest;
	regMap[farthest].free = false;
	regMap[farthest].var = var.name;

	// a temporary on stack or a local variable
	if (var.offset != -1)
	{
		// load instruction here
		string first = "-" + to_string(var.offset) + "(%rbp)";
		x86::Move(first, regMap[farthest].name, label);
	}
	return farthest;

}

int ensure(var_struct var, string label)
{
	for (int i = R10; i < REG_MAX; i++)
	{
		if (regMap[i].var == var.name)
		{
			return i;
		}
	}

	// variable not in register
	int reg = allocate(var, label);
	return reg;
}

void pre_process_assembly()
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

	if(tac.field_2 == "=")
	{
		if (tac.field_4 == "+")
		{
			int reg1 = ensure(varMap[tac.field_3], tac.label);
			int reg2 = ensure(varMap[tac.field_5], tac.label);
			if (varMap[tac.field_3].death <= now)
			{
				regMap[reg1].freeReg();
			}
			if (varMap[tac.field_5].death <= now)
			{
				regMap[reg2].freeReg();
			}
			var_struct var = varMap[tac.field_1];
			int reg3 = ensure(var, tac.label);
			x86::Move(regMap[reg1].name, regMap[RAX].name, tac.label);
			x86::Add(regMap[reg2].name, regMap[RAX].name, tac.label);
			x86::Move(regMap[RAX].name, regMap[reg3].name, tac.label);
		}

		if (tac.field_4 == "-")
		{
			int reg1 = ensure(varMap[tac.field_3], tac.label);
			int reg2 = ensure(varMap[tac.field_5], tac.label);
			if (varMap[tac.field_3].death <= now)
			{
				regMap[reg1].freeReg();
			}
			if (varMap[tac.field_5].death <= now)
			{
				regMap[reg2].freeReg();
			}
			var_struct var = varMap[tac.field_1];
			int reg3 = ensure(var, tac.label);
			x86::Move(regMap[reg1].name, regMap[RAX].name, tac.label);
			x86::Sub(regMap[reg2].name, regMap[RAX].name, tac.label);
			x86::Move(regMap[RAX].name, regMap[reg3].name, tac.label);
		}

		if (tac.field_4 == "*")
		{
			int reg1 = ensure(varMap[tac.field_3], tac.label);
			int reg2 = ensure(varMap[tac.field_5], tac.label);
			if (varMap[tac.field_3].death <= now)
			{
				regMap[reg1].freeReg();
			}
			if (varMap[tac.field_5].death <= now)
			{
				regMap[reg2].freeReg();
			}
			var_struct var = varMap[tac.field_1];
			int reg3 = ensure(var, tac.label);
			x86::Move(regMap[reg1].name, regMap[RAX].name, tac.label);
			x86::Mul(regMap[reg2].name, regMap[RAX].name, tac.label);
			x86::Move(regMap[RAX].name, regMap[reg3].name, tac.label);
		}

		if (tac.field_4 == "/" || tac.field_4 == "//")
		{
			int reg1 = ensure(varMap[tac.field_3], tac.label);
			int reg2 = ensure(varMap[tac.field_5], tac.label);
			if (varMap[tac.field_3].death <= now)
			{
				regMap[reg1].freeReg();
			}
			if (varMap[tac.field_5].death <= now)
			{
				regMap[reg2].freeReg();
			}
			var_struct var = varMap[tac.field_1];
			int reg3 = ensure(var, tac.label);
			x86::Move("$0", regMap[RDX].name, tac.label);
			x86::Move(regMap[reg1].name, regMap[RAX].name, tac.label);
			x86::CLTD(tac.label);
			x86::Div(regMap[reg2].name, tac.label);
			x86::Move(regMap[RAX].name, regMap[reg3].name, tac.label);
		}

		if (tac.field_4 == "%")
		{
			int reg1 = ensure(varMap[tac.field_3], tac.label);
			int reg2 = ensure(varMap[tac.field_5], tac.label);
			if (varMap[tac.field_3].death <= now)
			{
				regMap[reg1].freeReg();
			}
			if (varMap[tac.field_5].death <= now)
			{
				regMap[reg2].freeReg();
			}
			var_struct var = varMap[tac.field_1];
			int reg3 = ensure(var, tac.label);
			x86::Move("$0", regMap[RDX].name, tac.label);
			x86::Move(regMap[reg1].name, regMap[RAX].name, tac.label);
			x86::CLTD(tac.label);
			x86::Div(regMap[reg2].name, tac.label);
			x86::Move(regMap[RDX].name, regMap[reg3].name, tac.label);
		}

		if (tac.field_4 == "and" || tac.field_4 == "&")
		{
			int reg1 = ensure(varMap[tac.field_3], tac.label);
			int reg2 = ensure(varMap[tac.field_5], tac.label);
			if (varMap[tac.field_3].death <= now)
			{
				regMap[reg1].freeReg();
			}
			if (varMap[tac.field_5].death <= now)
			{
				regMap[reg2].freeReg();
			}
			var_struct var = varMap[tac.field_1];
			int reg3 = ensure(var, tac.label);
			x86::Move(regMap[reg1].name, regMap[RAX].name, tac.label);
			x86::And(regMap[reg2].name, regMap[RAX].name, tac.label);
			x86::Move(regMap[RAX].name, regMap[reg3].name, tac.label);
		}

		if (tac.field_4 == "or" || tac.field_4 == "|")
		{
			int reg1 = ensure(varMap[tac.field_3], tac.label);
			int reg2 = ensure(varMap[tac.field_5], tac.label);
			if (varMap[tac.field_3].death <= now)
			{
				regMap[reg1].freeReg();
			}
			if (varMap[tac.field_5].death <= now)
			{
				regMap[reg2].freeReg();
			}
			var_struct var = varMap[tac.field_1];
			int reg3 = ensure(var, tac.label);
			x86::Move(regMap[reg1].name, regMap[RAX].name, tac.label);
			x86::Or(regMap[reg2].name, regMap[RAX].name, tac.label);
			x86::Move(regMap[RAX].name, regMap[reg3].name, tac.label);
		}

		if (tac.field_4 == "^")
		{
			int reg1 = ensure(varMap[tac.field_3], tac.label);
			int reg2 = ensure(varMap[tac.field_5], tac.label);
			if (varMap[tac.field_3].death <= now)
			{
				regMap[reg1].freeReg();
			}
			if (varMap[tac.field_5].death <= now)
			{
				regMap[reg2].freeReg();
			}
			var_struct var = varMap[tac.field_1];
			int reg3 = ensure(var, tac.label);
			x86::Move(regMap[reg1].name, regMap[RAX].name, tac.label);
			x86::Xor(regMap[reg2].name, regMap[RAX].name, tac.label);
			x86::Move(regMap[RAX].name, regMap[reg3].name, tac.label);
		}

		if (tac.field_3 == "~" || tac.field_3 == "not")
		{
			int reg1 = ensure(varMap[tac.field_4], tac.label);
			if (varMap[tac.field_4].death <= now)
			{
				regMap[reg1].freeReg();
			}
			var_struct var = varMap[tac.field_1];
			int reg3 = ensure(var, tac.label);
			x86::Move(regMap[reg1].name, regMap[RAX].name, tac.label);
			x86::Not(regMap[RAX].name, tac.label);
			x86::Move(regMap[RAX].name, regMap[reg3].name, tac.label);
		}

		if (tac.field_4 == "<<")
		{
			int reg1 = ensure(varMap[tac.field_3], tac.label);
			int reg2 = ensure(varMap[tac.field_5], tac.label);
			if (varMap[tac.field_3].death <= now)
			{
				regMap[reg1].freeReg();
			}
			if (varMap[tac.field_5].death <= now)
			{
				regMap[reg2].freeReg();
			}
			var_struct var = varMap[tac.field_1];
			int reg3 = ensure(var, tac.label);
			x86::Move(regMap[reg1].name, regMap[RAX].name, tac.label);
			x86::Move(regMap[reg2].name, regMap[RCX].name, tac.label);
			x86::Shl("\%cl", regMap[RAX].name, tac.label);
			x86::Move(regMap[RAX].name, regMap[reg3].name, tac.label);
		}

		if (tac.field_4 == ">>")
		{
			int reg1 = ensure(varMap[tac.field_3], tac.label);
			int reg2 = ensure(varMap[tac.field_5], tac.label);
			if (varMap[tac.field_3].death <= now)
			{
				regMap[reg1].freeReg();
			}
			if (varMap[tac.field_5].death <= now)
			{
				regMap[reg2].freeReg();
			}
			var_struct var = varMap[tac.field_1];
			int reg3 = ensure(var, tac.label);
			x86::Move(regMap[reg1].name, regMap[RAX].name, tac.label);
			x86::Move(regMap[reg2].name, regMap[RCX].name, tac.label);
			x86::Shr("\%cl", regMap[RAX].name, tac.label);
			x86::Move(regMap[RAX].name, regMap[reg3].name, tac.label);
		}

		if (tac.field_4 == "==")
		{
			int reg1 = ensure(varMap[tac.field_3], tac.label);
			int reg2 = ensure(varMap[tac.field_5], tac.label);
			if (varMap[tac.field_3].death <= now)
			{
				regMap[reg1].freeReg();
			}
			if (varMap[tac.field_5].death <= now)
			{
				regMap[reg2].freeReg();
			}
			var_struct var = varMap[tac.field_1];
			int reg3 = ensure(var, tac.label);
			x86::Move("$1", regMap[RAX].name, tac.label);
			x86::Move("$0", regMap[reg3].name, tac.label);
			x86::Cmp(regMap[reg1].name, regMap[reg2].name, tac.label);
			x86::Cmove(regMap[RAX].name, regMap[reg3].name, tac.label);
		}

		if (tac.field_4 == "!=")
		{
			int reg1 = ensure(varMap[tac.field_3], tac.label);
			int reg2 = ensure(varMap[tac.field_5], tac.label);
			if (varMap[tac.field_3].death <= now)
			{
				regMap[reg1].freeReg();
			}
			if (varMap[tac.field_5].death <= now)
			{
				regMap[reg2].freeReg();
			}
			var_struct var = varMap[tac.field_1];
			int reg3 = ensure(var, tac.label);
			x86::Move("$1", regMap[RAX].name, tac.label);
			x86::Move("$0", regMap[reg3].name, tac.label);
			x86::Cmp(regMap[reg1].name, regMap[reg2].name, tac.label);
			x86::Cmovne(regMap[RAX].name, regMap[reg3].name, tac.label);
		}

		if (tac.field_4 == ">")
		{
			int reg1 = ensure(varMap[tac.field_3], tac.label);
			int reg2 = ensure(varMap[tac.field_5], tac.label);
			if (varMap[tac.field_3].death <= now)
			{
				regMap[reg1].freeReg();
			}
			if (varMap[tac.field_5].death <= now)
			{
				regMap[reg2].freeReg();
			}
			var_struct var = varMap[tac.field_1];
			int reg3 = ensure(var, tac.label);
			x86::Move("$1", regMap[RAX].name, tac.label);
			x86::Move("$0", regMap[reg3].name, tac.label);
			x86::Cmp(regMap[reg1].name, regMap[reg2].name, tac.label);
			x86::Cmovg(regMap[RAX].name, regMap[reg3].name, tac.label);
		}

		if (tac.field_4 == "<")
		{
			int reg1 = ensure(varMap[tac.field_3], tac.label);
			int reg2 = ensure(varMap[tac.field_5], tac.label);
			if (varMap[tac.field_3].death <= now)
			{
				regMap[reg1].freeReg();
			}
			if (varMap[tac.field_5].death <= now)
			{
				regMap[reg2].freeReg();
			}
			var_struct var = varMap[tac.field_1];
			int reg3 = ensure(var, tac.label);
			x86::Move("$1", regMap[RAX].name, tac.label);
			x86::Move("$0", regMap[reg3].name, tac.label);
			x86::Cmp(regMap[reg1].name, regMap[reg2].name, tac.label);
			x86::Cmovl(regMap[RAX].name, regMap[reg3].name, tac.label);
		}

		if (tac.field_4 == ">=")
		{
			int reg1 = ensure(varMap[tac.field_3], tac.label);
			int reg2 = ensure(varMap[tac.field_5], tac.label);
			if (varMap[tac.field_3].death <= now)
			{
				regMap[reg1].freeReg();
			}
			if (varMap[tac.field_5].death <= now)
			{
				regMap[reg2].freeReg();
			}
			var_struct var = varMap[tac.field_1];
			int reg3 = ensure(var, tac.label);
			x86::Move("$1", regMap[RAX].name, tac.label);
			x86::Move("$0", regMap[reg3].name, tac.label);
			x86::Cmp(regMap[reg1].name, regMap[reg2].name, tac.label);
			x86::Cmovge(regMap[RAX].name, regMap[reg3].name, tac.label);
		}

		if (tac.field_4 == "<=")
		{
			int reg1 = ensure(varMap[tac.field_3], tac.label);
			int reg2 = ensure(varMap[tac.field_5], tac.label);
			if (varMap[tac.field_3].death <= now)
			{
				regMap[reg1].freeReg();
			}
			if (varMap[tac.field_5].death <= now)
			{
				regMap[reg2].freeReg();
			}
			var_struct var = varMap[tac.field_1];
			int reg3 = ensure(var, tac.label);
			x86::Move("$1", regMap[RAX].name, tac.label);
			x86::Move("$0", regMap[reg3].name, tac.label);
			x86::Cmp(regMap[reg1].name, regMap[reg2].name, tac.label);
			x86::Cmovle(regMap[RAX].name, regMap[reg3].name, tac.label);
		}

		if (tac.field_4 == "")
		{
			int reg1 = ensure(varMap[tac.field_3], tac.label);
			if (varMap[tac.field_3].death <= now)
			{
				regMap[reg1].freeReg();
			}
			var_struct var = varMap[tac.field_1];
			int reg3 = ensure(var, tac.label);
			x86::Move(regMap[reg1].name, regMap[reg3].name, tac.label);
		}


	}

	if (tac.field_1 == "param")
	{
		int tmp = code_itr;
		for (;tmp < threeAC.size(); tmp++)
		{
			if (threeAC[tmp].field_1 != "param")
			{
				break;
			}
		}

///////////////////////////////////////////////////////////////////TODO////////////////////////
		// Push caller saved registers into stack
		x86::Push(regMap[R10].name, tac.label);
		x86::Push(regMap[R11].name, tac.label);
		x86::Push(regMap[R12].name, tac.label);
		x86::Push(regMap[R13].name, tac.label);
		x86::Push(regMap[R14].name, tac.label);
		x86::Push(regMap[R15].name, tac.label);

		// handle 'gap' also 
///////////////////////////////////////////////////////////////////TODO////////////////////////
		
		int cnt = code_itr - tmp;
		int num = min(cnt, 6);
		int index = tmp - num;
		int reg;

		for (int i = 0; i < num; i++)
		{
			reg = ensure(varMap[threeAC[index].field_2], threeAC[index].label);
			x86::Move(regMap[reg].name, regMap[R9 - i].name, threeAC[index].label);
			index++;
		}
		
		while(code_itr <= index)
		{
			x86::Push(threeAC[code_itr].field_2, threeAC[code_itr].label);
			code_itr++;
		}

		code_itr = tmp;
		now = code_itr + 1;


		// for (int i = 0; i < cnt && i < 6; i++)
		// {
		// 	int index = code_itr + cnt - i - 1;
		// 	x86::Move();
		// }
		
	}

	if (tac.field_1 == "pop_param")
	{
		int tmp = code_itr;
		for (;tmp < threeAC.size(); tmp++)
		{
			if (threeAC[tmp].field_1 != "param")
			{
				break;
			}
		}

		x86::Push(regMap[RBP].name, tac.label);
		x86::Move(regMap[RSP].name, regMap[RBP].name, tac.label);
		x86::Push(regMap[RBX].name, tac.label);
		x86::Push(regMap[RDI].name, tac.label);
		x86::Push(regMap[RSI].name, tac.label);
		x86::Push(regMap[R12].name, tac.label);
		x86::Push(regMap[R13].name, tac.label);
		x86::Push(regMap[R14].name, tac.label);
		x86::Push(regMap[R15].name, tac.label);

	}

	if (tac.field_1 == "push")
	{
		int reg = ensure(varMap[tac.field_2], tac.label);
		x86::Move(regMap[reg].name, regMap[RAX].name, tac.label);
	}

	if (tac.field_1 == "begin_function")
	{
		x86::Label(tac.field_2);
	}

	if (tac.field_1 == "end_function")
	{
		// DO NOTHING
	}

	if (tac.field_1 == "shift_sp")
	{
		
	}

	if (tac.field_1 == "call")
	{
		x86::Call(tac.field_2, tac.label);
	}

	if (tac.field_1 == "return")
	{
		x86::Ret(tac.label);
	}

	if (tac.field_1 == "if_false")
	{
		int reg = ensure(varMap[tac.field_2], tac.label);
		x86::Cmp("$0", regMap[reg].name, tac.label);
		x86::Je(tac.field_5, tac.label);
	}

	if (tac.field_1 == "if")
	{
		int reg = ensure(varMap[tac.field_2], tac.label);
		x86::Cmp("$0", regMap[reg].name, tac.label);
		x86::Jne(tac.field_5, tac.label);
	}

	// handle spilling of registers at end of BBs 
	if (BB_leaders.find(now) != BB_leaders.end())
	{
		for (int i=R10; i<REG_MAX; i++)
		{
			if (!regMap[i].free)
			{
				x86::Spill(i, tac.label);
			}
		}
	}

}

void update_var_struct(string name, int time)
{

	// a temporaray
	if (name[0] == '$')
	{
		// first encounter
		if (varMap.find(name) == varMap.end())
		{
			var_struct first;
			first.name = name;
			varMap[name] = first;
		}
		(varMap[name].locations).push_back(time);
		varMap[name].death = time;

		return;
	}

	// variables
	int loc_percent = -1;
	int loc_dot = -1;
	for (int i=0; i< name.length(); i++)
	{
		if (name[i] == '%')
			loc_percent = i;

		if (name[i] == '.')
			loc_dot = i;
	}

	// global variable
	if (loc_percent == -1)
	{
		assert(loc_dot == -1);

		// first encounter
		if (varMap.find(name) == varMap.end())
		{
			tableRecord* entry = globTable -> lookup_table(name);
			assert (entry);
			var_struct first;
			first.name = name;
			first.offset = entry -> offset;
			varMap[name] = first;
		}

		(varMap[name].locations).push_back(time);
		varMap[name].death = time;
		return;
	}

	// variable inside a function or a class
	if (loc_percent != -1 && loc_dot == -1)
	{
		// first encounter
		if (varMap.find(name) == varMap.end())
		{
			string funcName = "";
			for(int i=loc_percent + 1;i < name.length(); i++){
				funcName += name[i];
			}

			tableRecord* entry = globTable -> lookup_table(funcName);
			assert (entry);
			assert (entry -> symTab);

			// inside class
			if (entry -> symTab -> tableType == tableType::CLASS)
			{
				entry = (entry -> symTab) -> lookup_table(name);
				assert (entry);
				var_struct first;
				first.name = name;
				first.offset = entry -> offset;
				varMap[name] = first;
			}

			// inside function
			else
			{
				// handle function overloading
				vector<tableRecord*> params;				// donot free params, may lead to segfault
				vector<int> a_indices;
				for (int i=0; i<name.length(); i++)
				{
					if (name[i] == '@')
						a_indices.push_back(i);
				}

				for(int i=0; i < a_indices.size(); i++)
				{
					string type;
					int last;
					if (i == a_indices.size() - 1)
					{
						last = name.length();
					}
					else 
					{
						last = a_indices[i + 1];
					}
					type = name.substr(a_indices[i], last - a_indices[i]);
					tableRecord* entry = new tableRecord();
					entry -> type = type;
					params.push_back(entry);
				}

				entry = globTable -> lookup_table(name, recordType::TYPE_FUNCTION, &params);
				assert (entry);
				assert (entry -> symTab);


				entry = (entry -> symTab) -> lookup_table(name);
				assert (entry);
				var_struct first;
				first.name = name;
				first.offset = entry -> offset;
				varMap[name] = first;
			}

		}

		(varMap[name].locations).push_back(time);
		varMap[name].death = time;
		return;
	}

	// only possible case -> variable inside method of a class
	if (varMap.find(name) == varMap.end())
	{
		string className = "";
		for(int i=loc_percent + 1;i < loc_dot; i++){
			className += name[i];
		}

		string funcName = "";
		for(int i=loc_dot + 1;i < name.length(); i++){
			funcName += name[i];
		}


		// handle method overloading
		vector<tableRecord*> params;				// donot free params, may lead to segfault
		vector<int> a_indices;
		for (int i=0; i<name.length(); i++)
		{
			if (name[i] == '@')
				a_indices.push_back(i);
		}

		for(int i=0; i < a_indices.size(); i++)
		{
			string type;
			int last;
			if (i == a_indices.size() - 1)
			{
				last = name.length();
			}
			else 
			{
				last = a_indices[i + 1];
			}
			type = name.substr(a_indices[i], last - a_indices[i]);
			tableRecord* entry = new tableRecord();
			entry -> type = type;
			params.push_back(entry);
		}

		tableRecord* entry = globTable -> lookup_table(className);
		assert (entry);
		assert (entry -> symTab);

		entry = (entry -> symTab) -> lookup_table(name, recordType::TYPE_FUNCTION, &params);
		assert (entry);
		assert (entry -> symTab);

		entry = (entry -> symTab) -> lookup_table(name);
		assert (entry);
		var_struct first;
		first.name = name;
		first.offset = entry -> offset;
		varMap[name] = first;
	}

	(varMap[name].locations).push_back(time);
	varMap[name].death = time;
	return;

}

void handle_var_init(code tac, int time)
{

	if (tac.field_1 == "param" || tac.field_1 == "push" || tac.field_1 == "pop_param" || tac.field_1 == "if_false" || tac.field_1 == "if")
	{
		string name = tac.field_2;
		update_var_struct(name, time);
	}

	if (tac.field_2 == "=")
	{
		string name = tac.field_3;
		update_var_struct(name, time);
		name = tac.field_5;
		if (name.length())
			update_var_struct(name, time);
	}

}

// initializes the varstruct for each variable and temporary
void init_var_struct()
{
	for (int i=0; i<threeAC.size(); i++)
	{
		handle_var_init(threeAC[i], i+1);
	}
}

// identifies the basic block leaders
void identify_BB()
{
	for (int i=0; i< threeAC.size(); i++)
	{
		code inst = threeAC[i];
		if (inst.field_1 == "goto")
		{
			int lab = atoi((inst.field_2).c_str());
			BB_leaders.insert(i+2);
			BB_leaders.insert(lab);
		}

		if (inst.field_3 == "goto")
		{
			int lab = atoi((inst.field_4).c_str());
			BB_leaders.insert(i+2);
			BB_leaders.insert(lab);
		}
	}
}

// handle label management and $ for constants
void post_process_assembly()
{

}

void generate_assembly()
{
	identify_BB();
	init_var_struct();
	pre_process_assembly();
	for (code_itr=0; code_itr< threeAC.size(); code_itr++)
	{
		now = code_itr + 1;
		modifier(threeAC[code_itr]);
	}
	post_process_assembly();
}

/* NOTE : TAKE OFFSET ONLY FROM RBP */