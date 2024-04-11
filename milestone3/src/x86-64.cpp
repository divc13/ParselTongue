#include "include/symtable.hpp"
extern vector<code> threeAC;
extern symbolTable* globTable;

vector<instruction> assembly;
set<int> BB_leaders;

#define REG_START RDI

int now = 0;
int numParams = 0;
int code_itr = 0;
int string_index = 0;
int sp_shift = 0;

// top indicates the difference between rsp and rbp at curr time
int gap = 0;
int gap_bx = 0;

map <string, var_struct> varMap;
map <string, string> stringMap;
vector<reg_struct> regMap(REG_MAX);

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
	void Pop(string arg1, string label);
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
	void Jmp(string arg1, string label);
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
	gap = gap + 8;
}

void x86::Pop(string arg1, string label)
{
	instruction inst;
	inst.label = label;
	inst.first = "popq";
	inst.second = arg1;
	assembly.push_back(inst);
	gap = gap - 8;
}

void x86::Label(string label)
{
	instruction inst;
	inst.label = label + ":";
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
void x86::Jmp(string arg1, string label)
{
	instruction inst;
	inst.label = label;
	inst.first = "jmp";
	inst.second = arg1;
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
	inst.first = "jne";
	inst.second = arg1;
	assembly.push_back(inst);
}

void x86::Spill(int reg, string label)
{
	if(regMap[reg].free) return;
	cout << "SPILL LABEL: " << label << endl;
	if(!regMap[reg].allocatable) return;
	string var = regMap[reg].var;
	int offset = varMap[var].offset;

	// a temporary which is not on the stack
	if (offset == -1)
	{
		x86::Push(regMap[reg].name, label);
		varMap[var].offset = gap;
		return;
	}
	string first = to_string(-1 * offset) + "(%rbp)";
		
	x86::Move(regMap[reg].name, first, label);
	regMap[reg].freeReg();
}

int allocate(var_struct &var, string label)
{
	// free the registers for died variables
	for (int i = REG_START; i < REG_MAX; i++)
	{
		if (!regMap[i].free && varMap[regMap[i].var].death < now && (regMap[i]).allocatable)
		{
			regMap[i].freeReg();
		}
	}

	// find any free registers
	for (int i = REG_START; i < REG_MAX; i++)
	{
		if (regMap[i].free)
		{
			assert((regMap[i]).allocatable);
			var.reg = i;
			regMap[i].free = false;
			regMap[i].var = var.name;

			// a temporary on stack or a local variable
			if (var.offset != -1)
			{
				// load instruction here
				string first = to_string(-1 * var.offset) + "(%rbp)";
				x86::Move(first, regMap[i].name, label);
			}
			return i;
		}
	}

	// no free register found, spill the register
	int farthest = REG_START;
	vector<int> locs = varMap[regMap[farthest].var].locations;
	int last_next = locs[lower_bound(locs.begin(), locs.end(), now) - locs.begin()];
	for (int i = REG_START; i < REG_MAX; i++)
	{
		if(!regMap[i].allocatable) continue;

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
		string first = to_string(-1 * var.offset) + "(%rbp)";
		x86::Move(first, regMap[farthest].name, label);
	}
	return farthest;
}

int ensure(var_struct var, string label)
{
	if(is_num(var.name))
	{
		reg_struct regis;
		regis.free = false;
		regis.name = "$" + var.name;
		regis.var = "$" + var.name;
		regMap.push_back(regis);
		return regMap.size() - 1;
	}

	for (int i = REG_START; i < REG_MAX; i++)
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

	for (int i = 0; i < globTable->currentIndex; i++)
	{
		tableRecord* entry = (globTable->entries)[i];
		assert(entry);
		if (entry->recordType == recordType::CONST_STRING)
		{
			x86.label = ".section";
			x86.first = ".rodata";
			x86.second = "";
			x86.third = "";
			assembly.push_back(x86);
			x86.label = ".LC" + to_string(string_index) + ":";
			x86.first = "";
			x86.second = "";
			x86.third = "";
			assembly.push_back(x86);
			x86.label = ".string";
			x86.first = "\"" + entry->name + "\"";
			x86.second = "";
			x86.third = "";
			assembly.push_back(x86);

			stringMap["\"" + entry->name + "\""] = ".LC" + to_string(string_index);
			string_index++;
		}
	}
	
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
			int reg1, reg2, reg3;

			if((tac.field_3)[0] == '*')
			{
				string var_name = tac.field_3.substr(2, tac.field_3.length() - 3);
				reg1 = ensure(varMap[var_name], tac.label);
				x86::Move("(" + regMap[reg1].name + ")", regMap[RAX].name, tac.label);
			}
			else
			{
				reg1 = ensure(varMap[tac.field_3], tac.label);
				x86::Move(regMap[reg1].name, regMap[RAX].name, tac.label);
			}

			if((tac.field_5)[0] == '*')
			{
				string var_name = tac.field_5.substr(2, tac.field_5.length() - 3);
				reg2 = ensure(varMap[var_name], tac.label);
				x86::Add("(" + regMap[reg2].name + ")", regMap[RAX].name, tac.label);
			}
			else
			{
				reg2 = ensure(varMap[tac.field_5], tac.label);
				x86::Add(regMap[reg2].name, regMap[RAX].name, tac.label);
			}

			if((tac.field_1)[0] == '*')
			{
				string var_name = tac.field_1.substr(2, tac.field_1.length() - 3);
				reg3 = ensure(varMap[var_name], tac.label);
				x86::Move(regMap[RAX].name, "(" + regMap[reg3].name + ")", tac.label);
			}
			else
			{
				reg3 = ensure(varMap[tac.field_1], tac.label);
				x86::Move(regMap[RAX].name, regMap[reg3].name, tac.label);
			}
		}

		if (tac.field_4 == "-")
		{
			int reg1, reg2, reg3;

			if((tac.field_3)[0] == '*')
			{
				string var_name = tac.field_3.substr(2, tac.field_3.length() - 3);
				reg1 = ensure(varMap[var_name], tac.label);
				x86::Move("(" + regMap[reg1].name + ")", regMap[RAX].name, tac.label);
			}
			else
			{
				reg1 = ensure(varMap[tac.field_3], tac.label);
				x86::Move(regMap[reg1].name, regMap[RAX].name, tac.label);
			}

			if((tac.field_5)[0] == '*')
			{
				string var_name = tac.field_5.substr(2, tac.field_5.length() - 3);
				reg2 = ensure(varMap[var_name], tac.label);
				x86::Sub("(" + regMap[reg2].name + ")", regMap[RAX].name, tac.label);
			}
			else
			{
				reg2 = ensure(varMap[tac.field_5], tac.label);
				x86::Sub(regMap[reg2].name, regMap[RAX].name, tac.label);
			}

			if((tac.field_1)[0] == '*')
			{
				string var_name = tac.field_1.substr(2, tac.field_1.length() - 3);
				reg3 = ensure(varMap[var_name], tac.label);
				x86::Move(regMap[RAX].name, "(" + regMap[reg3].name + ")", tac.label);
			}
			else
			{
				reg3 = ensure(varMap[tac.field_1], tac.label);
				x86::Move(regMap[RAX].name, regMap[reg3].name, tac.label);
			}
		}

		if (tac.field_4 == "*")
		{
			int reg1, reg2, reg3;

			if((tac.field_3)[0] == '*')
			{
				string var_name = tac.field_3.substr(2, tac.field_3.length() - 3);
				reg1 = ensure(varMap[var_name], tac.label);
				x86::Move("(" + regMap[reg1].name + ")", regMap[RAX].name, tac.label);
			}
			else
			{
				reg1 = ensure(varMap[tac.field_3], tac.label);
				x86::Move(regMap[reg1].name, regMap[RAX].name, tac.label);
			}

			if((tac.field_5)[0] == '*')
			{
				string var_name = tac.field_5.substr(2, tac.field_5.length() - 3);
				reg2 = ensure(varMap[var_name], tac.label);
				x86::Mul("(" + regMap[reg2].name + ")", regMap[RAX].name, tac.label);
			}
			else
			{
				reg2 = ensure(varMap[tac.field_5], tac.label);
				x86::Mul(regMap[reg2].name, regMap[RAX].name, tac.label);
			}

			if((tac.field_1)[0] == '*')
			{
				string var_name = tac.field_1.substr(2, tac.field_1.length() - 3);
				reg3 = ensure(varMap[var_name], tac.label);
				x86::Move(regMap[RAX].name, "(" + regMap[reg3].name + ")", tac.label);
			}
			else
			{
				reg3 = ensure(varMap[tac.field_1], tac.label);
				x86::Move(regMap[RAX].name, regMap[reg3].name, tac.label);
			}
		}

		if (tac.field_4 == "/" || tac.field_4 == "//")
		{
			if (regMap[RDX].free == false)
				x86::Spill(RDX, tac.label);
			regMap[RDX].free = false;
			x86::Move("$0", regMap[RDX].name, tac.label);

			int reg1, reg2, reg3;

			if((tac.field_3)[0] == '*')
			{
				string var_name = tac.field_3.substr(2, tac.field_3.length() - 3);
				reg1 = ensure(varMap[var_name], tac.label);
				x86::Move("(" + regMap[reg1].name + ")", regMap[RAX].name, tac.label);
			}
			else
			{
				reg1 = ensure(varMap[tac.field_3], tac.label);
				x86::Move(regMap[reg1].name, regMap[RAX].name, tac.label);
			}

			x86::CLTD(tac.label);

			if((tac.field_5)[0] == '*')
			{
				string var_name = tac.field_5.substr(2, tac.field_5.length() - 3);
				reg2 = ensure(varMap[var_name], tac.label);
				x86::Div("(" + regMap[reg2].name + ")", tac.label);
			}
			else
			{
				reg2 = ensure(varMap[tac.field_5], tac.label);
				x86::Div(regMap[reg2].name, tac.label);
			}

			if((tac.field_1)[0] == '*')
			{
				string var_name = tac.field_1.substr(2, tac.field_1.length() - 3);
				reg3 = ensure(varMap[var_name], tac.label);
				x86::Move(regMap[RAX].name, "(" + regMap[reg3].name + ")", tac.label);
			}
			else
			{
				reg3 = ensure(varMap[tac.field_1], tac.label);
				x86::Move(regMap[RAX].name, regMap[reg3].name, tac.label);
			}

			regMap[RDX].free = true;
		}

		if (tac.field_4 == "%")
		{
			if (regMap[RDX].free == false)
				x86::Spill(RDX, tac.label);
			regMap[RDX].free = false;
			x86::Move("$0", regMap[RDX].name, tac.label);
			
			int reg1, reg2, reg3;

			if((tac.field_3)[0] == '*')
			{
				string var_name = tac.field_3.substr(2, tac.field_3.length() - 3);
				reg1 = ensure(varMap[var_name], tac.label);
				x86::Move("(" + regMap[reg1].name + ")", regMap[RAX].name, tac.label);
			}
			else
			{
				reg1 = ensure(varMap[tac.field_3], tac.label);
				x86::Move(regMap[reg1].name, regMap[RAX].name, tac.label);
			}

			x86::CLTD(tac.label);

			if((tac.field_5)[0] == '*')
			{
				string var_name = tac.field_5.substr(2, tac.field_5.length() - 3);
				reg2 = ensure(varMap[var_name], tac.label);
				x86::Div("(" + regMap[reg2].name + ")", tac.label);
			}
			else
			{
				reg2 = ensure(varMap[tac.field_5], tac.label);
				x86::Div(regMap[reg2].name, tac.label);
			}

			if((tac.field_1)[0] == '*')
			{
				string var_name = tac.field_1.substr(2, tac.field_1.length() - 3);
				reg3 = ensure(varMap[var_name], tac.label);
				x86::Move(regMap[RDX].name, "(" + regMap[reg3].name + ")", tac.label);
			}
			else
			{
				reg3 = ensure(varMap[tac.field_1], tac.label);
				x86::Move(regMap[RDX].name, regMap[reg3].name, tac.label);
			}

			regMap[RDX].free = true;
		}

		if (tac.field_4 == "and" || tac.field_4 == "&")
		{
			int reg1, reg2, reg3;

			if((tac.field_3)[0] == '*')
			{
				string var_name = tac.field_3.substr(2, tac.field_3.length() - 3);
				reg1 = ensure(varMap[var_name], tac.label);
				x86::Move("(" + regMap[reg1].name + ")", regMap[RAX].name, tac.label);
			}
			else
			{
				reg1 = ensure(varMap[tac.field_3], tac.label);
				x86::Move(regMap[reg1].name, regMap[RAX].name, tac.label);
			}

			if((tac.field_5)[0] == '*')
			{
				string var_name = tac.field_5.substr(2, tac.field_5.length() - 3);
				reg2 = ensure(varMap[var_name], tac.label);
				x86::And("(" + regMap[reg2].name + ")", regMap[RAX].name, tac.label);
			}
			else
			{
				reg2 = ensure(varMap[tac.field_5], tac.label);
				x86::And(regMap[reg2].name, regMap[RAX].name, tac.label);
			}

			if((tac.field_1)[0] == '*')
			{
				string var_name = tac.field_1.substr(2, tac.field_1.length() - 3);
				reg3 = ensure(varMap[var_name], tac.label);
				x86::Move(regMap[RAX].name, "(" + regMap[reg3].name + ")", tac.label);
			}
			else
			{
				reg3 = ensure(varMap[tac.field_1], tac.label);
				x86::Move(regMap[RAX].name, regMap[reg3].name, tac.label);
			}
		}

		if (tac.field_4 == "or" || tac.field_4 == "|")
		{
			int reg1, reg2, reg3;

			if((tac.field_3)[0] == '*')
			{
				string var_name = tac.field_3.substr(2, tac.field_3.length() - 3);
				reg1 = ensure(varMap[var_name], tac.label);
				x86::Move("(" + regMap[reg1].name + ")", regMap[RAX].name, tac.label);
			}
			else
			{
				reg1 = ensure(varMap[tac.field_3], tac.label);
				x86::Move(regMap[reg1].name, regMap[RAX].name, tac.label);
			}

			if((tac.field_5)[0] == '*')
			{
				string var_name = tac.field_5.substr(2, tac.field_5.length() - 3);
				reg2 = ensure(varMap[var_name], tac.label);
				x86::Or("(" + regMap[reg2].name + ")", regMap[RAX].name, tac.label);
			}
			else
			{
				reg2 = ensure(varMap[tac.field_5], tac.label);
				x86::Or(regMap[reg2].name, regMap[RAX].name, tac.label);
			}

			if((tac.field_1)[0] == '*')
			{
				string var_name = tac.field_1.substr(2, tac.field_1.length() - 3);
				reg3 = ensure(varMap[var_name], tac.label);
				x86::Move(regMap[RAX].name, "(" + regMap[reg3].name + ")", tac.label);
			}
			else
			{
				reg3 = ensure(varMap[tac.field_1], tac.label);
				x86::Move(regMap[RAX].name, regMap[reg3].name, tac.label);
			}
		}

		if (tac.field_4 == "^")
		{
			int reg1, reg2, reg3;

			if((tac.field_3)[0] == '*')
			{
				string var_name = tac.field_3.substr(2, tac.field_3.length() - 3);
				reg1 = ensure(varMap[var_name], tac.label);
				x86::Move("(" + regMap[reg1].name + ")", regMap[RAX].name, tac.label);
			}
			else
			{
				reg1 = ensure(varMap[tac.field_3], tac.label);
				x86::Move(regMap[reg1].name, regMap[RAX].name, tac.label);
			}

			if((tac.field_5)[0] == '*')
			{
				string var_name = tac.field_5.substr(2, tac.field_5.length() - 3);
				reg2 = ensure(varMap[var_name], tac.label);
				x86::Xor("(" + regMap[reg2].name + ")", regMap[RAX].name, tac.label);
			}
			else
			{
				reg2 = ensure(varMap[tac.field_5], tac.label);
				x86::Xor(regMap[reg2].name, regMap[RAX].name, tac.label);
			}

			if((tac.field_1)[0] == '*')
			{
				string var_name = tac.field_1.substr(2, tac.field_1.length() - 3);
				reg3 = ensure(varMap[var_name], tac.label);
				x86::Move(regMap[RAX].name, "(" + regMap[reg3].name + ")", tac.label);
			}
			else
			{
				reg3 = ensure(varMap[tac.field_1], tac.label);
				x86::Move(regMap[RAX].name, regMap[reg3].name, tac.label);
			}
		}

		if (tac.field_3 == "~" || tac.field_3 == "not")
		{
			int reg1, reg2, reg3;

			if((tac.field_3)[0] == '*')
			{
				string var_name = tac.field_3.substr(2, tac.field_3.length() - 3);
				reg1 = ensure(varMap[var_name], tac.label);
				x86::Move("(" + regMap[reg1].name + ")", regMap[RAX].name, tac.label);
			}
			else
			{
				reg1 = ensure(varMap[tac.field_3], tac.label);
				x86::Move(regMap[reg1].name, regMap[RAX].name, tac.label);
			}

			x86::Not(regMap[RAX].name, tac.label);

			if((tac.field_1)[0] == '*')
			{
				string var_name = tac.field_1.substr(2, tac.field_1.length() - 3);
				reg3 = ensure(varMap[var_name], tac.label);
				x86::Move(regMap[RAX].name, "(" + regMap[reg3].name + ")", tac.label);
			}
			else
			{
				reg3 = ensure(varMap[tac.field_1], tac.label);
				x86::Move(regMap[RAX].name, regMap[reg3].name, tac.label);
			}
		}

		if (tac.field_4 == "<<")
		{
			if (regMap[RCX].free == false)
				x86::Spill(RCX, tac.label);
			regMap[RCX].free = false;
			
			int reg1, reg2, reg3;

			if((tac.field_3)[0] == '*')
			{
				string var_name = tac.field_3.substr(2, tac.field_3.length() - 3);
				reg1 = ensure(varMap[var_name], tac.label);
				x86::Move("(" + regMap[reg1].name + ")", regMap[RAX].name, tac.label);
			}
			else
			{
				reg1 = ensure(varMap[tac.field_3], tac.label);
				x86::Move(regMap[reg1].name, regMap[RAX].name, tac.label);
			}

			if((tac.field_5)[0] == '*')
			{
				string var_name = tac.field_5.substr(2, tac.field_5.length() - 3);
				reg2 = ensure(varMap[var_name], tac.label);
				x86::Move("(" + regMap[reg2].name + ")", regMap[RCX].name, tac.label);
			}
			else
			{
				reg2 = ensure(varMap[tac.field_5], tac.label);
				x86::Move(regMap[reg2].name, regMap[RCX].name, tac.label);
			}

			x86::Shl("\%cl", regMap[RAX].name, tac.label);

			if((tac.field_1)[0] == '*')
			{
				string var_name = tac.field_1.substr(2, tac.field_1.length() - 3);
				reg3 = ensure(varMap[var_name], tac.label);
				x86::Move(regMap[RAX].name, "(" + regMap[reg3].name + ")", tac.label);
			}
			else
			{
				reg3 = ensure(varMap[tac.field_1], tac.label);
				x86::Move(regMap[RAX].name, regMap[reg3].name, tac.label);
			}

			regMap[RCX].free = true;
		}

		if (tac.field_4 == ">>")
		{
			if (regMap[RCX].free == false)
				x86::Spill(RCX, tac.label);
			regMap[RCX].free = false;
			
			int reg1, reg2, reg3;

			if((tac.field_3)[0] == '*')
			{
				string var_name = tac.field_3.substr(2, tac.field_3.length() - 3);
				reg1 = ensure(varMap[var_name], tac.label);
				x86::Move("(" + regMap[reg1].name + ")", regMap[RAX].name, tac.label);
			}
			else
			{
				reg1 = ensure(varMap[tac.field_3], tac.label);
				x86::Move(regMap[reg1].name, regMap[RAX].name, tac.label);
			}

			if((tac.field_5)[0] == '*')
			{
				string var_name = tac.field_5.substr(2, tac.field_5.length() - 3);
				reg2 = ensure(varMap[var_name], tac.label);
				x86::Move("(" + regMap[reg2].name + ")", regMap[RCX].name, tac.label);
			}
			else
			{
				reg2 = ensure(varMap[tac.field_5], tac.label);
				x86::Move(regMap[reg2].name, regMap[RCX].name, tac.label);
			}

			x86::Shr("\%cl", regMap[RAX].name, tac.label);

			if((tac.field_1)[0] == '*')
			{
				string var_name = tac.field_1.substr(2, tac.field_1.length() - 3);
				reg3 = ensure(varMap[var_name], tac.label);
				x86::Move(regMap[RAX].name, "(" + regMap[reg3].name + ")", tac.label);
			}
			else
			{
				reg3 = ensure(varMap[tac.field_1], tac.label);
				x86::Move(regMap[RAX].name, regMap[reg3].name, tac.label);
			}

			regMap[RCX].free = true;
		}

		if (tac.field_4 == "==")
		{
			int reg1, reg2, reg3;
			string reg_name1 = "", reg_name2 = "", reg_name3 = "";

			if((tac.field_3)[0] == '*')
			{
				string var_name = tac.field_3.substr(2, tac.field_3.length() - 3);
				reg1 = ensure(varMap[var_name], tac.label);
				reg_name1 = "(" + regMap[reg1].name + ")";
			}
			else
			{
				reg1 = ensure(varMap[tac.field_3], tac.label);
				reg_name1 = regMap[reg1].name;
			}

			if((tac.field_5)[0] == '*')
			{
				string var_name = tac.field_5.substr(2, tac.field_5.length() - 3);
				reg2 = ensure(varMap[var_name], tac.label);
				reg_name2 = "(" + regMap[reg2].name + ")";
			}
			else
			{
				reg2 = ensure(varMap[tac.field_5], tac.label);
				reg_name2 = regMap[reg2].name;
			}

			if((tac.field_1)[0] == '*')
			{
				string var_name = tac.field_1.substr(2, tac.field_1.length() - 3);
				reg3 = ensure(varMap[var_name], tac.label);
				reg_name3 = "(" + regMap[reg3].name + ")";
			}
			else
			{
				reg3 = ensure(varMap[tac.field_1], tac.label);
				reg_name3 = regMap[reg3].name;
			}

			x86::Move("$1", regMap[RAX].name, tac.label);
			x86::Move("$0", reg_name3, tac.label);
			x86::Cmp(reg_name1, reg_name2, tac.label);
			x86::Cmove(regMap[RAX].name, reg_name3, tac.label);
		}

		if (tac.field_4 == "!=")
		{
			int reg1, reg2, reg3;
			string reg_name1 = "", reg_name2 = "", reg_name3 = "";

			if((tac.field_3)[0] == '*')
			{
				string var_name = tac.field_3.substr(2, tac.field_3.length() - 3);
				reg1 = ensure(varMap[var_name], tac.label);
				reg_name1 = "(" + regMap[reg1].name + ")";
			}
			else
			{
				reg1 = ensure(varMap[tac.field_3], tac.label);
				reg_name1 = regMap[reg1].name;
			}

			if((tac.field_5)[0] == '*')
			{
				string var_name = tac.field_5.substr(2, tac.field_5.length() - 3);
				reg2 = ensure(varMap[var_name], tac.label);
				reg_name2 = "(" + regMap[reg2].name + ")";
			}
			else
			{
				reg2 = ensure(varMap[tac.field_5], tac.label);
				reg_name2 = regMap[reg2].name;
			}

			if((tac.field_1)[0] == '*')
			{
				string var_name = tac.field_1.substr(2, tac.field_1.length() - 3);
				reg3 = ensure(varMap[var_name], tac.label);
				reg_name3 = "(" + regMap[reg3].name + ")";
			}
			else
			{
				reg3 = ensure(varMap[tac.field_1], tac.label);
				reg_name3 = regMap[reg3].name;
			}

			x86::Move("$1", regMap[RAX].name, tac.label);
			x86::Move("$0", reg_name3, tac.label);
			x86::Cmp(reg_name1, reg_name2, tac.label);
			x86::Cmovne(regMap[RAX].name, reg_name3, tac.label);
		}

		if (tac.field_4 == ">")
		{
			int reg1, reg2, reg3;
			string reg_name1 = "", reg_name2 = "", reg_name3 = "";

			if((tac.field_3)[0] == '*')
			{
				string var_name = tac.field_3.substr(2, tac.field_3.length() - 3);
				reg1 = ensure(varMap[var_name], tac.label);
				reg_name1 = "(" + regMap[reg1].name + ")";
			}
			else
			{
				reg1 = ensure(varMap[tac.field_3], tac.label);
				reg_name1 = regMap[reg1].name;
			}

			if((tac.field_5)[0] == '*')
			{
				string var_name = tac.field_5.substr(2, tac.field_5.length() - 3);
				reg2 = ensure(varMap[var_name], tac.label);
				reg_name2 = "(" + regMap[reg2].name + ")";
			}
			else
			{
				reg2 = ensure(varMap[tac.field_5], tac.label);
				reg_name2 = regMap[reg2].name;
			}

			if((tac.field_1)[0] == '*')
			{
				string var_name = tac.field_1.substr(2, tac.field_1.length() - 3);
				reg3 = ensure(varMap[var_name], tac.label);
				reg_name3 = "(" + regMap[reg3].name + ")";
			}
			else
			{
				reg3 = ensure(varMap[tac.field_1], tac.label);
				reg_name3 = regMap[reg3].name;
			}

			x86::Move("$1", regMap[RAX].name, tac.label);
			x86::Move("$0", reg_name3, tac.label);
			x86::Cmp(reg_name1, reg_name2, tac.label);
			x86::Cmovg(regMap[RAX].name, reg_name3, tac.label);
		}

		if (tac.field_4 == "<")
		{
			int reg1, reg2, reg3;
			string reg_name1 = "", reg_name2 = "", reg_name3 = "";

			if((tac.field_3)[0] == '*')
			{
				string var_name = tac.field_3.substr(2, tac.field_3.length() - 3);
				reg1 = ensure(varMap[var_name], tac.label);
				reg_name1 = "(" + regMap[reg1].name + ")";
			}
			else
			{
				reg1 = ensure(varMap[tac.field_3], tac.label);
				reg_name1 = regMap[reg1].name;
			}

			if((tac.field_5)[0] == '*')
			{
				string var_name = tac.field_5.substr(2, tac.field_5.length() - 3);
				reg2 = ensure(varMap[var_name], tac.label);
				reg_name2 = "(" + regMap[reg2].name + ")";
			}
			else
			{
				reg2 = ensure(varMap[tac.field_5], tac.label);
				reg_name2 = regMap[reg2].name;
			}

			if((tac.field_1)[0] == '*')
			{
				string var_name = tac.field_1.substr(2, tac.field_1.length() - 3);
				reg3 = ensure(varMap[var_name], tac.label);
				reg_name3 = "(" + regMap[reg3].name + ")";
			}
			else
			{
				reg3 = ensure(varMap[tac.field_1], tac.label);
				reg_name3 = regMap[reg3].name;
			}

			x86::Move("$1", regMap[RAX].name, tac.label);
			x86::Move("$0", reg_name3, tac.label);
			x86::Cmp(reg_name1, reg_name2, tac.label);
			x86::Cmovl(regMap[RAX].name, reg_name3, tac.label);
		}

		if (tac.field_4 == ">=")
		{
			int reg1, reg2, reg3;
			string reg_name1 = "", reg_name2 = "", reg_name3 = "";

			if((tac.field_3)[0] == '*')
			{
				string var_name = tac.field_3.substr(2, tac.field_3.length() - 3);
				reg1 = ensure(varMap[var_name], tac.label);
				reg_name1 = "(" + regMap[reg1].name + ")";
			}
			else
			{
				reg1 = ensure(varMap[tac.field_3], tac.label);
				reg_name1 = regMap[reg1].name;
			}

			if((tac.field_5)[0] == '*')
			{
				string var_name = tac.field_5.substr(2, tac.field_5.length() - 3);
				reg2 = ensure(varMap[var_name], tac.label);
				reg_name2 = "(" + regMap[reg2].name + ")";
			}
			else
			{
				reg2 = ensure(varMap[tac.field_5], tac.label);
				reg_name2 = regMap[reg2].name;
			}

			if((tac.field_1)[0] == '*')
			{
				string var_name = tac.field_1.substr(2, tac.field_1.length() - 3);
				reg3 = ensure(varMap[var_name], tac.label);
				reg_name3 = "(" + regMap[reg3].name + ")";
			}
			else
			{
				reg3 = ensure(varMap[tac.field_1], tac.label);
				reg_name3 = regMap[reg3].name;
			}

			x86::Move("$1", regMap[RAX].name, tac.label);
			x86::Move("$0", reg_name3, tac.label);
			x86::Cmp(reg_name1, reg_name2, tac.label);
			x86::Cmovge(regMap[RAX].name, reg_name3, tac.label);
		}

		if (tac.field_4 == "<=")
		{
			int reg1, reg2, reg3;
			string reg_name1 = "", reg_name2 = "", reg_name3 = "";

			if((tac.field_3)[0] == '*')
			{
				string var_name = tac.field_3.substr(2, tac.field_3.length() - 3);
				reg1 = ensure(varMap[var_name], tac.label);
				reg_name1 = "(" + regMap[reg1].name + ")";
			}
			else
			{
				reg1 = ensure(varMap[tac.field_3], tac.label);
				reg_name1 = regMap[reg1].name;
			}

			if((tac.field_5)[0] == '*')
			{
				string var_name = tac.field_5.substr(2, tac.field_5.length() - 3);
				reg2 = ensure(varMap[var_name], tac.label);
				reg_name2 = "(" + regMap[reg2].name + ")";
			}
			else
			{
				reg2 = ensure(varMap[tac.field_5], tac.label);
				reg_name2 = regMap[reg2].name;
			}

			if((tac.field_1)[0] == '*')
			{
				string var_name = tac.field_1.substr(2, tac.field_1.length() - 3);
				reg3 = ensure(varMap[var_name], tac.label);
				reg_name3 = "(" + regMap[reg3].name + ")";
			}
			else
			{
				reg3 = ensure(varMap[tac.field_1], tac.label);
				reg_name3 = regMap[reg3].name;
			}

			x86::Move("$1", regMap[RAX].name, tac.label);
			x86::Move("$0", reg_name3, tac.label);
			x86::Cmp(reg_name1, reg_name2, tac.label);
			x86::Cmove(regMap[RAX].name, reg_name3, tac.label);
		}

		if (tac.field_4 == "")
		{

			if ((tac.field_2)[0] == '\"')
			{
				int reg1 = ensure(varMap[tac.field_3], tac.label);
				if (varMap[tac.field_3].death <= now)
				{
					regMap[reg1].freeReg();
				}
				x86::Move(regMap[reg1].name, regMap[RDI].name, tac.label);
				var_struct var = varMap[tac.field_1];
				int reg3 = ensure(var, tac.label);
				x86::Move(regMap[reg3].name, regMap[RSI].name, tac.label);


				// PUSH REGISTERS???

				x86::Call("strcpy", tac.label);
			}

			int reg1 = ensure(varMap[tac.field_3], tac.label);
			var_struct var = varMap[tac.field_1];
			int reg3 = ensure(var, tac.label);
			x86::Move(regMap[reg1].name, regMap[reg3].name, tac.label);
			if (varMap[tac.field_3].death <= now)
			{
				regMap[reg1].freeReg();
			}
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
		
		int cnt = tmp - code_itr;
		int num = min(cnt, 6);
		int index = tmp - num;
		int reg;

		cout << cnt << endl;

		for (int i = 0; i < 6; i++)
		{
			x86::Spill(RDI + i, tac.label);
			regMap[RDI + i].free = false;
			regMap[RDI + i].allocatable = false;
		}
		
		for (int i = 0; i < num; i++)
		{
			reg = ensure(varMap[threeAC[index].field_2], threeAC[index].label);
			x86::Move(regMap[reg].name, regMap[RDI + num - i - 1].name, threeAC[index].label);
			index++;
		}

		index = tmp - num;
		while(code_itr < index)
		{
			reg = ensure(varMap[threeAC[code_itr].field_2], threeAC[code_itr].label);
			x86::Push(regMap[reg].name, threeAC[code_itr].label);
			code_itr++;
		}

		for (int i = 0; i < 6; i++)
		{
			regMap[RDI + i].allocatable = true;
			if(i >= num)
				regMap[RDI + i].free = true;
		}

		code_itr = tmp - 1;
		now = code_itr + 1;
	}

	if (tac.field_1 == "pop_param")
	{
		int tmp = code_itr;
		for (;tmp < threeAC.size(); tmp++)
		{
			if (threeAC[tmp].field_1 != "pop_param")
			{
				break;
			}
		}

		int cnt = tmp - code_itr;
		int num = min(6, cnt);
		int reg;

		for (int i = 0; i < 6; i++)
		{
			x86::Push(regMap[RDI + i].name, tac.label);
			regMap[RDI + i].freeReg();
		}

		code_itr += num;
		while(code_itr < tmp)
		{
			var_struct var = varMap[threeAC[code_itr].field_2];
			reg = ensure(var, threeAC[code_itr].label);
			int offset = var.offset;
			x86::Move(to_string(-1 * offset) + "(%rbp)", regMap[reg].name, threeAC[code_itr].label);
			code_itr++;
		}

		code_itr--;
		now = code_itr + 1;

		// callee saved registers
		x86::Push(regMap[RBX].name, tac.label);
		x86::Push(regMap[R12].name, tac.label);
		x86::Push(regMap[R13].name, tac.label);
		x86::Push(regMap[R14].name, tac.label);
		x86::Push(regMap[R15].name, tac.label);
		x86::Move(regMap[RSP].name, regMap[RBX].name, tac.label);
		gap_bx = gap;

	}

	if (tac.field_1 == "push")
	{
		int reg = ensure(varMap[tac.field_2], tac.label);
		x86::Move(regMap[reg].name, regMap[RAX].name, tac.label);
	}

	if (tac.field_1 == "begin_function")
	{
		x86::Label(tac.field_2);
		x86::Push(regMap[RBP].name, tac.label);
		x86::Move(regMap[RSP].name, regMap[RBP].name, tac.label);
		gap = 0;
		if (sp_shift > 0)
		{
			x86::Sub(regMap[RSP].name, to_string(sp_shift), tac.label);
			gap = sp_shift;
			sp_shift = 0;
		}

	}

	// for taking the return values
	if (tac.field_3 == "popparam")
	{
		int reg = ensure(varMap[tac.field_1], tac.label);
		x86::Move(regMap[RAX].name, regMap[reg].name, tac.label);
	}

	if (tac.field_1 == "end_function")
	{
		if (threeAC[code_itr - 1].field_1 != "return")
		{
			x86::Move(regMap[RBX].name, regMap[RSP].name, tac.label);
			gap = gap_bx;
			x86::Pop(regMap[R15].name, tac.label);
			x86::Pop(regMap[R14].name, tac.label);
			x86::Pop(regMap[R13].name, tac.label);
			x86::Pop(regMap[R12].name, tac.label);
			x86::Pop(regMap[RBX].name, tac.label);

			// popping the register values
			for(int i = 0; i < 6; i++)
			{
				x86::Pop(regMap[R9 - i].name, tac.label);
			}

			x86::Move(regMap[RBP].name, regMap[RSP].name, tac.label);
			gap = 0;

			x86::Ret(tac.label);
		}
		gap = 0;
	}

	if (tac.field_1 == "shift_sp")
	{
		int shift = atoi((tac.field_2).c_str());
		if (shift < 0)
		{
			sp_shift = -1 * shift;
		}
	}

	if (tac.field_1 == "call")
	{
		x86::Spill(R10, tac.label);
		x86::Spill(R11, tac.label);
		x86::Call(tac.field_2, tac.label);
	}

	if (tac.field_1 == "return")
	{
		// caller saved registers
		x86::Move(regMap[RBX].name, regMap[RSP].name, tac.label);
		gap = gap_bx;
		x86::Pop(regMap[R15].name, tac.label);
		x86::Pop(regMap[R14].name, tac.label);
		x86::Pop(regMap[R13].name, tac.label);
		x86::Pop(regMap[R12].name, tac.label);
		x86::Pop(regMap[RBX].name, tac.label);

		// popping the register values
		for(int i = 0; i < 6; i++)
		{
			x86::Pop(regMap[R9 - i].name, tac.label);
		}

		x86::Move(regMap[RBP].name, regMap[RSP].name, tac.label);
		gap = 0;

		x86::Ret(tac.label);
	}

	if (tac.field_1 == "if_false")
	{
		int reg = ensure(varMap[tac.field_2], tac.label);
		x86::Cmp("$0", regMap[reg].name, tac.label);
		x86::Je(tac.field_4, tac.label);
	}

	if (tac.field_1 == "goto")
	{
		int reg = ensure(varMap[tac.field_2], tac.label);
		x86::Cmp("$0", regMap[reg].name, tac.label);
		x86::Jmp(tac.field_2, tac.label);
	}

	if (tac.field_1 == "if")
	{
		int reg = ensure(varMap[tac.field_2], tac.label);
		x86::Cmp("$0", regMap[reg].name, tac.label);
		x86::Jne(tac.field_4, tac.label);
	}

	// handle spilling of registers at end of BBs 
	if (BB_leaders.find(now) != BB_leaders.end())
	{
		for (int i=REG_START; i<REG_MAX; i++)
		{
			x86::Spill(i, tac.label);
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

	if (is_num(name))
	{
		// first encounter
		if (varMap.find(name) == varMap.end())
		{
			var_struct first;
			first.name = name;
			varMap[name] = first;
		}
		(varMap[name].locations).push_back(time);
		varMap[name].death = -1;

		return;
	}


	// variables
	int loc_percent = -1;
	int loc_dot = -1;
	for (int i=0; i< name.length(); i++)
	{
		if (name[i] == '%')
			loc_percent = i;

		if (i < name.length() - 2 && name[i] == '_' && name[i + 1] == 'C' && name[i + 2] == 'c')
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

	string real_id = name.substr(0, loc_percent);
	// variable inside a function or a class
	if (loc_percent != -1 && loc_dot == -1)
	{
		// first encounter
		if (varMap.find(name) == varMap.end())
		{
			string funcName = "";
			bool check = true;
			for(int i=loc_percent + 1;i < name.length() - 2; i++){
				if (name[i] == '_' && name[i + 1] == 'Z' && name[i + 2] == 'z')
				{
					check = false;
					break;
				}
				if (name[i] == '_' && name[i + 1] == 'N' && name[i + 2] == 'n')
				{
					check = false;
					break;
				}
				funcName += name[i];
			}

			if (check)
			{
				funcName += name[name.length() - 2]  + name[name.length() - 1];
			}

			tableRecord* entry = globTable -> lookup_table(funcName);
			assert (entry);
			assert (entry -> symTab);

			// inside class
			if (entry -> symTab -> tableType == tableType::CLASS)
			{
				entry = (entry -> symTab) -> lookup_table(real_id);
				assert (entry);
				var_struct first;
				first.name = name;
				first.offset = entry -> offset + 8;
				varMap[name] = first;
			}

			// inside function
			else
			{
				// handle function overloading
				vector<tableRecord*> params;				// donot free params, may lead to segfault
				vector<int> a_indices;
				for (int i=0; i<name.length() - 2; i++)
				{
					if (name[i] == '_' && name[i + 1] == 'Z' && name[i + 2] == 'z')
						a_indices.push_back(i + 2);
				}

				for(int i=0; i < a_indices.size(); i++)
				{
					string type;
					int last;
					if (i == a_indices.size() - 1)
					{
						last = name.length() - 5;
					}
					else 
					{
						last = a_indices[i + 1] - 3;
					}
					type = name.substr(a_indices[i] + 1, last - a_indices[i]);
					tableRecord* entry = new tableRecord();
					entry -> type = type;
					params.push_back(entry);
					cout << type << " " << type.length() << endl;
				}

				
				entry = globTable -> lookup_table(funcName, recordType::TYPE_FUNCTION, &params);
				assert (entry);
				assert (entry -> symTab);


				entry = (entry -> symTab) -> lookup_table(real_id);
				assert (entry);
				var_struct first;
				first.name = name;
				first.offset = entry -> offset + 8;
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
		bool check = true;
		string funcName = "";
		for(int i=loc_dot + 3;i < name.length() - 2; i++){
			if (name[i] == '_' && name[i + 1] == 'Z' && name[i + 2] == 'z')
			{
				check = false;
				break;
			}
			if (name[i] == '_' && name[i + 1] == 'N' && name[i + 2] == 'n')
			{
				check = false;
				break;
			}
			funcName += name[i];
		}

		if (check)
		{
			funcName += name[name.length() - 2]  + name[name.length() - 1];
		}


		// handle method overloading
		vector<tableRecord*> params;				// do not free params, may lead to segfault
		vector<int> a_indices;
		for (int i=0; i<name.length(); i++)
		{
			if (name[i] == '_' && name[i + 1] == 'Z' && name[i + 2] == 'z')
				a_indices.push_back(i + 2);
		}

		for(int i=0; i < a_indices.size(); i++)
		{
			string type;
			int last;
			if (i == a_indices.size() - 1)
			{
				last = name.length() - 5;
			}
			else 
			{
				last = a_indices[i + 1] - 3;
			}
			type = name.substr(a_indices[i] + 1, last - a_indices[i]);
			tableRecord* entry = new tableRecord();
			entry -> type = type;
			entry -> name = type;
			params.push_back(entry);
		}

		tableRecord* entry = globTable -> lookup_table(className);
		assert (entry);
		assert (entry -> symTab);

		entry = (entry -> symTab) -> lookup_table(funcName, recordType::TYPE_FUNCTION, &params);
		assert (entry);
		assert (entry -> symTab);

		entry = (entry -> symTab) -> lookup_table(real_id);
		assert (entry);
		var_struct first;
		first.name = name;
		first.offset = entry -> offset + 8;
		varMap[name] = first;
	}

	(varMap[name].locations).push_back(time);
	varMap[name].death = time;
	return;

}

bool is_name_var(string &name)
{
	cout << name << endl;
	int len = name.length();
	if (!len) return false;
	if (name[0] == '\"') return false;
	if (name[0] == '-') return false;
	if (name[0] == '~') return false;
	if (name == "popparam") return false;
	if (name == "not") return false;

	if (name[0] == '*')
	{
		name = name.substr(2, name.length() - 3);
	}
	return true;
}

void handle_var_init(code tac, int time)
{

	if (tac.field_1 == "param" || tac.field_1 == "push" || tac.field_1 == "pop_param" || tac.field_1 == "if_false" || tac.field_1 == "if")
	{
		string name = tac.field_2;
		if (is_name_var(name))
			update_var_struct(name, time);
	}

	if (tac.field_2 == "=")
	{
		string name = tac.field_3;
		if (is_name_var(name))
			update_var_struct(name, time);
		name = tac.field_5;
		if (name.length() && is_name_var(name))
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

bool is_num(string val)
{
	if (val.size() == 0)
		return false;
	for (auto i : val)
		if (i < '0' || i > '9')
			return false;
	return true;
}

void post_process_assembly()
{
	for (auto inst : assembly)
	{
		if (is_num(inst.first))
			inst.first = "$" + inst.first;
		if (is_num(inst.label))
			inst.label = "$" + inst.label;
		if (is_num(inst.second))
			inst.second = "$" + inst.second;
		if (is_num(inst.third))
			inst.third = "$" + inst.third;
	}

	string prev = "";
	for (auto &inst : assembly)
	{
		if (inst.label == prev)
			inst.label = "";
		else
			prev = inst.label;
	}

}

void generate_assembly()
{
	cout << "START" << endl;
	identify_BB();
	cout << "1" << endl;
	init_var_struct();
	cout << "2" << endl;
	pre_process_assembly();
	cout << "3" << endl;

	for (code_itr=0; code_itr< threeAC.size(); code_itr++)
	{
		cout << "LINE: "  << code_itr + 1 << endl;
		now = code_itr + 1;
		modifier(threeAC[code_itr]);
	}
	cout << "4" << endl;
	post_process_assembly();
	cout << "5" << endl;
}

/* NOTE : TAKE OFFSET ONLY FROM RBP */