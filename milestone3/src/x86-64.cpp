#include "include/code.hpp"
extern vector<code> threeAC;
extern symbolTable* globTable;

vector<instruction> assembly;
set<int> BB_leaders;

#define REG_START RDI

int now = 0;
int numParams = 0;
int code_itr = 0;

// top indicates the difference between rsp and rbp at curr time
int gap = 0;
int gap_bx = 0;

map <string, var_struct> varMap;
extern map <string, string> stringMap;
extern map<string, tableRecord*> Temp_to_record;
vector<reg_struct> regMap(REG_MAX);

void reg_struct::freeReg()
{
	var = "";
	free = true;
	return;
}

namespace x86
{
	void Move(string arg1, string arg2, string label, string comment = "");
	void Add(string arg1, string arg2, string label, string comment = "");
	void Sub(string arg1, string arg2, string label, string comment = "");
	void Cmp(string arg1, string arg2, string label, string comment = "");
	void Mul(string arg1, string arg2, string label, string comment = "");
	void Div(string arg1, string label, string comment = "");
	void And(string arg1, string arg2, string label, string comment = "");
	void Or(string arg1, string arg2, string label, string comment = "");
	void Xor(string arg1, string arg2, string label, string comment = "");
	void Not(string arg1, string label, string comment = "");
	void Shl(string arg1, string arg2, string label, string comment = "");
	void Shr(string arg1, string arg2, string label, string comment = "");
	void Push(string arg1, string label, string comment = "");
	void Pop(string arg1, string label, string comment = "");
	void Label(string label, string comment = "");
	void Call(string arg1, string label, string comment = "");
	void Ret(string label, string comment = "");
	void CLTD(string label, string comment = "");
	void Cmove(string arg1, string arg2, string label, string comment = "");
	void Cmovne(string arg1, string arg2, string label, string comment = "");
	void Cmovg(string arg1, string arg2, string label, string comment = "");
	void Cmovl(string arg1, string arg2, string label, string comment = "");
	void Cmovge(string arg1, string arg2, string label, string comment = "");
	void Cmovle(string arg1, string arg2, string label, string comment = "");
	void Je(string arg1, string label, string comment = "");
	void Jmp(string arg1, string label, string comment = "");
	void Jne(string arg1, string label, string comment = "");
	void Lea(string arg1, string arg2, string label, string comment = "");
	void Spill(int reg, string label, string comment = "");
}

void x86::Move(string arg1, string arg2, string label, string comment)
{
	instruction inst;
	inst.label = label;
	inst.first = "movq";
	inst.second = arg1;
	inst.third = arg2;
	inst.comment = comment;
	assembly.push_back(inst);
}

void x86::Add(string arg1, string arg2, string label, string comment)
{
	instruction inst;
	inst.label = label;
	inst.first = "addq";
	inst.second = arg1;
	inst.third = arg2;
	inst.comment = comment;
	assembly.push_back(inst);
}

void x86::Sub(string arg1, string arg2, string label, string comment)
{
	instruction inst;
	inst.label = label;
	inst.first = "subq";
	inst.second = arg1;
	inst.third = arg2;
	inst.comment = comment;
	assembly.push_back(inst);
}

void x86::Cmp(string arg1, string arg2, string label, string comment)
{
	instruction inst;
	inst.label = label;
	inst.first = "cmp";
	inst.second = arg1;
	inst.third = arg2;
	inst.comment = comment;
	assembly.push_back(inst);
}

void x86::Mul(string arg1, string arg2, string label, string comment)
{
	instruction inst;
	inst.label = label;
	inst.first = "imul";
	inst.second = arg1;
	inst.third = arg2;
	inst.comment = comment;
	assembly.push_back(inst);
}

void x86::Div(string arg1, string label, string comment)
{
	instruction inst;
	inst.label = label;
	inst.first = "idiv";
	inst.second = arg1;
	inst.third = "";
	inst.comment = comment;
	assembly.push_back(inst);
}

void x86::And(string arg1, string arg2, string label, string comment)
{
	instruction inst;
	inst.label = label;
	inst.first = "and";
	inst.second = arg1;
	inst.third = arg2;
	inst.comment = comment;
	assembly.push_back(inst);
}

void x86::Or(string arg1, string arg2, string label, string comment)
{
	instruction inst;
	inst.label = label;
	inst.first = "or";
	inst.second = arg1;
	inst.third = arg2;
	inst.comment = comment;
	assembly.push_back(inst);
}

void x86::Xor(string arg1, string arg2, string label, string comment)
{
	instruction inst;
	inst.label = label;
	inst.first = "xor";
	inst.second = arg1;
	inst.third = arg2;
	inst.comment = comment;
	assembly.push_back(inst);
}

void x86::Not(string arg1, string label, string comment)
{
	instruction inst;
	inst.label = label;
	inst.first = "not";
	inst.second = arg1;
	inst.comment = comment;
	assembly.push_back(inst);
}

void x86::Shl(string arg1, string arg2, string label, string comment)
{
	instruction inst;
	inst.label = label;
	inst.first = "shl";
	inst.second = arg1;
	inst.third = arg2;
	inst.comment = comment;
	assembly.push_back(inst);
}

void x86::Shr(string arg1, string arg2, string label, string comment)
{
	instruction inst;
	inst.label = label;
	inst.first = "shr";
	inst.second = arg1;
	inst.third = arg2;
	inst.comment = comment;
	assembly.push_back(inst);
}

void x86::Push(string arg1, string label, string comment)
{
	instruction inst;
	inst.label = label;
	inst.first = "pushq";
	inst.second = arg1;
	inst.comment = comment;
	assembly.push_back(inst);
	gap = gap + 8;
}

void x86::Pop(string arg1, string label, string comment)
{
	instruction inst;
	inst.label = label;
	inst.first = "popq";
	inst.second = arg1;
	inst.comment = comment;
	assembly.push_back(inst);
	gap = gap - 8;
}

void x86::Label(string label, string comment)
{
	if(label == "#main") label = "main";
	instruction inst;
	inst.label = label + ":";
	inst.comment = comment;
	assembly.push_back(inst);
}

void x86::Call(string arg1, string label, string comment)
{
	Xor(regMap[RAX].name, regMap[RAX].name, label);
	instruction inst;
	inst.label = label;
	inst.first = "callq";
	inst.second = arg1;
	inst.comment = comment;
	assembly.push_back(inst);
}

void x86::Ret(string label, string comment)
{
	instruction inst;
	inst.label = label;
	inst.first = "leave";
	inst.comment = comment;
	assembly.push_back(inst);
	inst.first = "ret";
	inst.comment = comment;
	assembly.push_back(inst);
}

void x86::CLTD(string label, string comment)
{
	instruction inst;
	inst.label = label;
	inst.first = "cldt";
	inst.comment = comment;
	assembly.push_back(inst);
}
void x86::Cmove(string arg1, string arg2, string label, string comment)
{
	instruction inst;
	inst.label = label;
	inst.first = "cmove";
	inst.second = arg1;
	inst.third = arg2;
	inst.comment = comment;
	assembly.push_back(inst);
}
void x86::Cmovne(string arg1, string arg2, string label, string comment)
{
	instruction inst;
	inst.label = label;
	inst.first = "cmovne";
	inst.second = arg1;
	inst.third = arg2;
	inst.comment = comment;
	assembly.push_back(inst);
}
void x86::Cmovg(string arg1, string arg2, string label, string comment)
{
	instruction inst;
	inst.label = label;
	inst.first = "cmovg";
	inst.second = arg1;
	inst.third = arg2;
	inst.comment = comment;
	assembly.push_back(inst);
}
void x86::Cmovl(string arg1, string arg2, string label, string comment)
{
	instruction inst;
	inst.label = label;
	inst.first = "cmovl";
	inst.second = arg1;
	inst.third = arg2;
	inst.comment = comment;
	assembly.push_back(inst);
}
void x86::Cmovge(string arg1, string arg2, string label, string comment)
{
	instruction inst;
	inst.label = label;
	inst.first = "cmovge";
	inst.second = arg1;
	inst.third = arg2;
	inst.comment = comment;
	assembly.push_back(inst);
}
void x86::Cmovle(string arg1, string arg2, string label, string comment)
{
	instruction inst;
	inst.label = label;
	inst.first = "cmovle";
	inst.second = arg1;
	inst.third = arg2;
	inst.comment = comment;
	assembly.push_back(inst);
}
void x86::Jmp(string arg1, string label, string comment)
{
	instruction inst;
	inst.label = label;
	inst.first = "jmp";
	inst.second = ".L" + arg1;
	inst.comment = comment;
	assembly.push_back(inst);
}

void x86::Je(string arg1, string label, string comment)
{
	instruction inst;
	inst.label = label;
	inst.first = "je";
	inst.second = ".L" + arg1;
	inst.comment = comment;
	assembly.push_back(inst);
}

void x86::Jne(string arg1, string label, string comment)
{
	instruction inst;
	inst.label = label;
	inst.first = "jne";
	inst.second = ".L" + arg1;
	inst.comment = comment;
	assembly.push_back(inst);
}

void x86::Lea(string arg1, string arg2, string label, string comment)
{
	instruction inst;
	inst.label = label;
	inst.first = "leaq";
	inst.second = arg1 + "(%rip)";
	inst.third = arg2;
	inst.comment = comment;
	assembly.push_back(inst);
}

void x86::Spill(int reg, string label, string comment)
{
	if(regMap[reg].free) return;
	if(!regMap[reg].allocatable) return;
	string var = regMap[reg].var;
	int offset = varMap[var].offset;

	// a temporary which is not on the stack
	if (offset == -1)
	{
		assert(false);
		x86::Push(regMap[reg].name, label, "SPILL 1  " + var);
		varMap[var].offset = gap;
		return;
	}
	string first = to_string(-1 * offset) + "(%rbp)";
		
	x86::Move(regMap[reg].name, first, label, "SPILL " + var);
	regMap[reg].freeReg();
}

int allocate(var_struct &var, string label)
{
	// free the registers for died variables
	for (int i = REG_START; i < REG_MAX; i++)
	{
		if (!regMap[i].free && varMap[regMap[i].var].death < now - 1 && (regMap[i]).allocatable)
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
				x86::Move(first, regMap[i].name, label, "ALLOCATE " + var.name);
			}
			else assert(false);
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
	else assert(false);
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

void formString(string &temp)
{
	size_t pos = 0;
	while((pos = temp.find("\\\r\n", pos)) != string::npos)
	{
		temp.replace(pos, 3, "\\\\n");
		pos+=3;
	}

	pos = 0;
	while((pos = temp.find("\\\r", pos)) != string::npos)
	{
		temp.replace(pos, 2, "\\\\n");
		pos+=3;
	}

	pos = 0;
	while((pos = temp.find("\\\n", pos)) != string::npos)
	{
		temp.replace(pos, 2, "\\\\n");
		pos+=3;
	}

	pos = 0;
	while((pos = temp.find("\n", pos)) != string::npos)
	{
		temp.replace(pos, 1, "\\n");
		pos+=2;
	}
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

	x86.label = "float_format:";
	x86.first = ".asciz";
	x86.second = ", \"%lf\"";
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

	int string_index = 0;
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
			string strtmp = "\"" + entry->name + "\"";
			formString(strtmp);
			x86.first = strtmp;
			x86.second = "";
			x86.third = "";
			assembly.push_back(x86);
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

		if (tac.field_3 == "~" || tac.field_3 == "not" || tac.field_3 == "-")
		{
			int reg1, reg2, reg3;

			if((tac.field_4)[0] == '*')
			{
				string var_name = tac.field_4.substr(2, tac.field_4.length() - 3);
				reg1 = ensure(varMap[var_name], tac.label);
				x86::Move("(" + regMap[reg1].name + ")", regMap[RAX].name, tac.label);
			}
			else
			{
				reg1 = ensure(varMap[tac.field_4], tac.label);
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
			x86::Cmp(reg_name2, reg_name1, tac.label);
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
			x86::Cmp(reg_name2, reg_name1, tac.label);
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
			x86::Cmp(reg_name2, reg_name1, tac.label);
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
			x86::Cmp(reg_name2, reg_name1, tac.label);
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
			x86::Cmp(reg_name2, reg_name1, tac.label);
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
			x86::Cmp(reg_name2, reg_name1, tac.label);
			x86::Cmove(regMap[RAX].name, reg_name3, tac.label);
		}

		if (tac.field_4 == "")
		{
			int reg1, reg3;
			string reg_name1, reg_name3;
			if((tac.field_1)[0] == '*')
			{
				string var_name = tac.field_1.substr(2, tac.field_1.length() - 3);
				reg1 = ensure(varMap[var_name], tac.label);
				reg_name1 = "(" + regMap[reg1].name + ")";
			}
			else
			{
				reg1 = ensure(varMap[tac.field_1], tac.label);
				reg_name1 = regMap[reg1].name;
			}

			if (tac.field_3 == "popparam")
			{
				x86::Move(regMap[RAX].name, reg_name1, tac.label);
			}

			else{

				if((tac.field_3)[0] == '*')
				{
					string var_name = tac.field_3.substr(2, tac.field_3.length() - 3);
					reg3 = ensure(varMap[var_name], tac.label);
					reg_name3 = "(" + regMap[reg3].name + ")";
				}
				else
				{
					reg3 = ensure(varMap[tac.field_3], tac.label);
					reg_name3 = regMap[reg3].name;
				}

				x86::Move(reg_name3, reg_name1, tac.label);
			}

		}

	}

	if (tac.field_3 == "findAddress")
	{
		int reg1 = ensure(varMap[tac.field_1], tac.label);
		x86::Lea(tac.field_4, regMap[reg1].name, tac.label);
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

		for (int i = 0; i < 6; i++)
		{
			x86::Spill(RDI + i, tac.label);
			regMap[RDI + i].free = false;
			regMap[RDI + i].var = "";
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
			regMap[reg].freeReg();
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
		// do nothing, handled by begin_function
	}

	if (tac.field_1 == "push")
	{
		if((tac.field_2)[0] == '*')
		{
			string var_name = tac.field_2.substr(2, tac.field_2.length() - 3);
			int reg = ensure(varMap[var_name], tac.label);
			x86::Move("(" + regMap[reg].name + ")", regMap[RAX].name, tac.label);
		}
		else
		{
			int reg = ensure(varMap[tac.field_2], tac.label);
			x86::Move(regMap[reg].name, regMap[RAX].name, tac.label);
		}
	}

	if (tac.field_1 == "begin_function")
	{
		x86::Label(tac.field_2);
		x86::Push(regMap[RBP].name, tac.label);
		x86::Move(regMap[RSP].name, regMap[RBP].name, tac.label);
		gap = 0;

		string funcName = tac.field_2;
		int last = funcName.length();
		vector<int> param_indices;
		vector <tableRecord*> params;
		string class_name = "";
		string function = funcName;
		symTable* table = globTable;
		tableRecord* entry;

		for(int i=0; i<funcName.length() - 2; i++)
		{
			if(funcName.substr(i, 3) == "_Cc")
			{
				class_name = funcName.substr(0, i);
			}
			if(funcName.substr(i, 3) == "_Zz")
			{
				param_indices.push_back(i);
			}
			if(funcName.substr(i, 3) == "_Nn")
			{
				last = i;
			}
		}

		for(int i=0; i<param_indices.size(); i++)
		{
			int next = last;
			if (i != param_indices.size() - 1) next = param_indices[i+1];
			string type = funcName.substr(param_indices[i] + 3, next - param_indices[i] - 3);
			tableRecord* entry = new tableRecord();
			entry -> type = type;
			params.push_back(entry);
		}

		if(param_indices.size())
		{
			function = function.substr(0, param_indices[0]); 
		}

		else
		{
			function = function.substr(0, last); 
		}
		
		if(class_name.length())
		{
			int size = class_name.length() + 3;
			table = globTable;
			entry = table -> lookup_table(class_name);
			assert(entry);
			table = entry -> symTab;
			function = function.substr(size, function.length() - size);
		}

		entry = table -> lookup_table(function, recordType::TYPE_FUNCTION, &params);
		if(funcName == "#main") entry = globTable -> lookup_table(funcName);
		assert(entry);

		table = entry -> symTab;

		int sp_shift = table -> size;

		if (sp_shift > 0)
		{
			x86::Sub("$" + to_string(sp_shift), regMap[RSP].name, tac.label);
			gap += sp_shift;
			sp_shift = 0;
		}

		code_itr ++;
		now ++;
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

		// callee saved registers
		x86::Push(regMap[RBX].name, tac.label);
		regMap[RBX].freeReg();
		x86::Push(regMap[R12].name, tac.label);
		regMap[R12].freeReg();
		x86::Push(regMap[R13].name, tac.label);
		regMap[R13].freeReg();
		x86::Push(regMap[R14].name, tac.label);
		regMap[R14].freeReg();
		x86::Push(regMap[R15].name, tac.label);
		regMap[R15].freeReg();
		x86::Move(regMap[RSP].name, regMap[RBX].name, tac.label);
		gap_bx = gap;

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

	if (tac.field_1 == "call")
	{

		x86::Spill(R10, tac.label);
		x86::Spill(R11, tac.label);

		if (gap % 16)
		{
			x86::Push("$0", tac.label);
		}

		x86::Call(tac.field_2, tac.label);

		if (gap % 16)
		{
			x86::Pop("$0", tac.label);
		}

		// post call (free the used regs)
		for (int i = 0; i < 6; i++)
		{
			regMap[RDI + i].allocatable = true;
			regMap[RDI + i].free = true;
		}

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
		for (int i=REG_START; i<REG_MAX; i++)
		{
			x86::Spill(i, tac.label);
		}
		// value wont be updated for a register during spill
		x86::Cmp("$0", regMap[reg].name, tac.label);
		x86::Je(tac.field_4, tac.label);
	}

	if (tac.field_1 == "goto")
	{
		int reg = ensure(varMap[tac.field_2], tac.label);
		for (int i=REG_START; i<REG_MAX; i++)
		{
			x86::Spill(i, tac.label);
		}
		// value wont be updated for a register during spill
		x86::Cmp("$0", regMap[reg].name, tac.label);
		x86::Jmp(tac.field_2, tac.label);
	}

	if (tac.field_1 == "if")
	{
		int reg = ensure(varMap[tac.field_2], tac.label);
		for (int i=REG_START; i<REG_MAX; i++)
		{
			x86::Spill(i, tac.label);
		}
		// value wont be updated for a register during spill
		x86::Cmp("$0", regMap[reg].name, tac.label);
		x86::Jne(tac.field_4, tac.label);
	}

	// handle spilling of registers at end of BBs 
	if (BB_leaders.find(now + 1) != BB_leaders.end())
	{
		for (int i=REG_START; i<REG_MAX; i++)
		{
			x86::Spill(i, tac.label);
		}
	}

}

void update_var_struct(string name, int time)
{
	cout << name << endl;
	// a temporary
	if (name[0] == '$')
	{
		// first encounter
		if (varMap.find(name) == varMap.end())
		{
			var_struct first;
			tableRecord* record = Temp_to_record[name];
			assert (record);
			first.offset = record -> offset + 8;
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
			first.offset = entry -> offset + 8;
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
	int len = name.length();
	if (!len) return false;
	if (name[0] == '\"') return false;
	if (name == "-") return false;
	if (name == "~") return false;
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

	if (tac.field_3 == "findAddress")
	{
		string name = tac.field_1;
		if (is_name_var(name))
			update_var_struct(name, time);
		return;
	}

	if (tac.field_2 == "=")
	{
		string name = tac.field_1;
		if (is_name_var(name))
			update_var_struct(name, time);
		name = tac.field_3;
		if (is_name_var(name))
			update_var_struct(name, time);
		if (name == "-" || name == "not" || name == "~")
		{
			name = tac.field_4;
			if (is_name_var(name))
				update_var_struct(name, time);
		}
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
	if (val[0] == '-')
		val = val.substr(1, val.length() -1);
	for (auto i : val)
		if (i < '0' || i > '9')
			return false;
	return true;
}

void post_process_assembly()
{
	// for (auto inst : assembly)
	// {
	// 	if (is_num(inst.first))
	// 		inst.first = "$" + inst.first;
	// 	if (is_num(inst.label))
	// 		inst.label = "$" + inst.label;
	// 	if (is_num(inst.second))
	// 		inst.second = "$" + inst.second;
	// 	if (is_num(inst.third))
	// 		inst.third = "$" + inst.third;
	// }

	string prev = "";
	for (auto &inst : assembly)
	{
		if (inst.label == prev)
			inst.label = "";
		else
		{
			prev = inst.label;
			if (is_num((inst.label).substr(0, (inst.label).length() - 1)))
				inst.label = ".L" + inst.label;
		}
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
		// cout << "LINE: "  << code_itr + 1 << endl;
		now = code_itr + 1;
		modifier(threeAC[code_itr]);
	}
	cout << "4" << endl;
	post_process_assembly();
	cout << "5" << endl;
}

/* NOTE : TAKE OFFSET ONLY FROM RBP */