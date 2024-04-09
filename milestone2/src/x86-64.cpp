#include "include/symtable.hpp"
#define REG_CNT 16
extern vector<code> threeAC;

vector<instruction> assembly;

map<string, vector<int>> var_next;

int now = 0;
int numParams = 0;
int code_itr = 0;

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

void x86::Push(string label)
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

void x86::Push(string label)
{
	instruction inst;
	inst.label = label;
	inst.first = "leave";
	assembly.push_back(inst);
	inst.first = "ret";
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
			var.death = var_next[var.name].back();
			regMap[i].free = false;
			regMap[i].var = var.name;
			regMap[i].next = find(var_next[var.name].begin(), var_next[var.name].end(), now)[1];
			// load instruction here
			return i;
		}
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
			regMap[i].next = find(var_next[var.name].begin(), var_next[var.name].end(), now)[1];
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

	if(tac.field_2 == "=")
	{
		if (tac.field_4 == "+")
		{
			int reg1 = ensure(varMap[tac.field_3]);
			int reg2 = ensure(varMap[tac.field_5]);
			if (varMap[tac.field_3].death < now)
			{
				regMap[reg1].freeReg();
			}
			if (varMap[tac.field_5].death < now)
			{
				regMap[reg2].freeReg();
			}
			var_struct var;
			var.name = tac.field_1;
			int reg3 = allocate(var);
			x86::Move(regMap[reg1].name, regMap[RAX].name, tac.label);
			x86::Move(regMap[reg2].name, regMap[RDX].name, tac.label);
			x86::Add(regMap[RDX].name, regMap[RAX].name, tac.label);
			x86::Move(regMap[RAX].name, regMap[reg3].name, tac.label);
		}

		if (tac.field_4 == "-")
		{
			int reg1 = ensure(varMap[tac.field_3]);
			int reg2 = ensure(varMap[tac.field_5]);
			if (varMap[tac.field_3].death < now)
			{
				regMap[reg1].freeReg();
			}
			if (varMap[tac.field_5].death < now)
			{
				regMap[reg2].freeReg();
			}
			var_struct var;
			var.name = tac.field_1;
			int reg3 = allocate(var);
			x86::Move(regMap[reg1].name, regMap[RAX].name, tac.label);
			x86::Move(regMap[reg2].name, regMap[RDX].name, tac.label);
			x86::Sub(regMap[RAX].name, regMap[RDX].name, tac.label);
			x86::Move(regMap[RAX].name, regMap[reg3].name, tac.label);
		}

		if (tac.field_4 == "*")
		{
			int reg1 = ensure(varMap[tac.field_3]);
			int reg2 = ensure(varMap[tac.field_5]);
			if (varMap[tac.field_3].death < now)
			{
				regMap[reg1].freeReg();
			}
			if (varMap[tac.field_5].death < now)
			{
				regMap[reg2].freeReg();
			}
			var_struct var;
			var.name = tac.field_1;
			int reg3 = allocate(var);
			x86::Move(regMap[reg1].name, regMap[RAX].name, tac.label);
			x86::Move(regMap[reg2].name, regMap[RDX].name, tac.label);
			x86::Mul(regMap[RDX].name, regMap[RAX].name, tac.label);
			x86::Move(regMap[RAX].name, regMap[reg3].name, tac.label);
		}

		if (tac.field_4 == "/" || tac.field_4 == "//")
		{
			int reg1 = ensure(varMap[tac.field_3]);
			int reg2 = ensure(varMap[tac.field_5]);
			if (varMap[tac.field_3].death < now)
			{
				regMap[reg1].freeReg();
			}
			if (varMap[tac.field_5].death < now)
			{
				regMap[reg2].freeReg();
			}
			var_struct var;
			var.name = tac.field_1;
			int reg3 = allocate(var);
			x86::Move(regMap[reg1].name, regMap[RAX].name, tac.label);
			x86::Div(regMap[reg2].name, tac.label);
			x86::Move(regMap[RAX].name, regMap[reg3].name, tac.label);
		}

		if (tac.field_4 == "%")
		{
			int reg1 = ensure(varMap[tac.field_3]);
			int reg2 = ensure(varMap[tac.field_5]);
			if (varMap[tac.field_3].death < now)
			{
				regMap[reg1].freeReg();
			}
			if (varMap[tac.field_5].death < now)
			{
				regMap[reg2].freeReg();
			}
			var_struct var;
			var.name = tac.field_1;
			int reg3 = allocate(var);
			x86::Move(regMap[reg1].name, regMap[RAX].name, tac.label);
			x86::Div(regMap[reg2].name, tac.label);
			x86::Move(regMap[RDX].name, regMap[reg3].name, tac.label);
		}



		// loop in ** or use pow with xmm?


		if (tac.field_4 == "and" || tac.field_4 == "&")
		{
			int reg1 = ensure(varMap[tac.field_3]);
			int reg2 = ensure(varMap[tac.field_5]);
			if (varMap[tac.field_3].death < now)
			{
				regMap[reg1].freeReg();
			}
			if (varMap[tac.field_5].death < now)
			{
				regMap[reg2].freeReg();
			}
			var_struct var;
			var.name = tac.field_1;
			int reg3 = allocate(var);
			x86::Move(regMap[reg1].name, regMap[RAX].name, tac.label);
			x86::Move(regMap[reg2].name, regMap[RDX].name, tac.label);
			x86::And(regMap[RAX].name, regMap[RDX].name, tac.label);
			x86::Move(regMap[RAX].name, regMap[reg3].name, tac.label);
		}

		if (tac.field_4 == "or" || tac.field_4 == "|")
		{
			int reg1 = ensure(varMap[tac.field_3]);
			int reg2 = ensure(varMap[tac.field_5]);
			if (varMap[tac.field_3].death < now)
			{
				regMap[reg1].freeReg();
			}
			if (varMap[tac.field_5].death < now)
			{
				regMap[reg2].freeReg();
			}
			var_struct var;
			var.name = tac.field_1;
			int reg3 = allocate(var);
			x86::Move(regMap[reg1].name, regMap[RAX].name, tac.label);
			x86::Move(regMap[reg2].name, regMap[RDX].name, tac.label);
			x86::Or(regMap[RAX].name, regMap[RDX].name, tac.label);
			x86::Move(regMap[RAX].name, regMap[reg3].name, tac.label);
		}

		if (tac.field_4 == "^")
		{
			int reg1 = ensure(varMap[tac.field_3]);
			int reg2 = ensure(varMap[tac.field_5]);
			if (varMap[tac.field_3].death < now)
			{
				regMap[reg1].freeReg();
			}
			if (varMap[tac.field_5].death < now)
			{
				regMap[reg2].freeReg();
			}
			var_struct var;
			var.name = tac.field_1;
			int reg3 = allocate(var);
			x86::Move(regMap[reg1].name, regMap[RAX].name, tac.label);
			x86::Move(regMap[reg2].name, regMap[RDX].name, tac.label);
			x86::Xor(regMap[RAX].name, regMap[RDX].name, tac.label);
			x86::Move(regMap[RAX].name, regMap[reg3].name, tac.label);
		}

		if (tac.field_3 == "~" || tac.field_3 == "not")
		{
			int reg1 = ensure(varMap[tac.field_4]);
			if (varMap[tac.field_4].death < now)
			{
				regMap[reg1].freeReg();
			}
			var_struct var;
			var.name = tac.field_1;
			int reg3 = allocate(var);
			x86::Move(regMap[reg1].name, regMap[RAX].name, tac.label);
			x86::Not(regMap[RAX].name, tac.label);
			x86::Move(regMap[RAX].name, regMap[reg3].name, tac.label);
		}

		if (tac.field_4 == "<<")
		{
			int reg1 = ensure(varMap[tac.field_3]);
			int reg2 = ensure(varMap[tac.field_5]);
			if (varMap[tac.field_3].death < now)
			{
				regMap[reg1].freeReg();
			}
			if (varMap[tac.field_5].death < now)
			{
				regMap[reg2].freeReg();
			}
			var_struct var;
			var.name = tac.field_1;
			int reg3 = allocate(var);
			x86::Move(regMap[reg1].name, regMap[RAX].name, tac.label);
			x86::Move(regMap[reg2].name, regMap[RCX].name, tac.label);
			x86::Shl(regMap[RAX].name, "%cl", tac.label);
			x86::Move(regMap[RAX].name, regMap[reg3].name, tac.label);
		}

		if (tac.field_4 == ">>")
		{
			int reg1 = ensure(varMap[tac.field_3]);
			int reg2 = ensure(varMap[tac.field_5]);
			if (varMap[tac.field_3].death < now)
			{
				regMap[reg1].freeReg();
			}
			if (varMap[tac.field_5].death < now)
			{
				regMap[reg2].freeReg();
			}
			var_struct var;
			var.name = tac.field_1;
			int reg3 = allocate(var);
			x86::Move(regMap[reg1].name, regMap[RAX].name, tac.label);
			x86::Move(regMap[reg2].name, regMap[RCX].name, tac.label);
			x86::Shr(regMap[RAX].name, "%cl", tac.label);
			x86::Move(regMap[RAX].name, regMap[reg3].name, tac.label);
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

		// Push everything (registers) into stack

		x86::Push(regMap[RAX].name, tac.label);
		x86::Push(regMap[RCX].name, tac.label);
		x86::Push(regMap[RDX].name, tac.label);
		x86::Push(regMap[R8].name, tac.label);
		x86::Push(regMap[R9].name, tac.label);
		x86::Push(regMap[R10].name, tac.label);
		x86::Push(regMap[R11].name, tac.label);

		int cnt = code_itr - tmp;
		int i = 0;
		int index = tmp - 1;

		int reg;

		if (i < cnt)
		{
			reg = ensure(varMap[threeAC[index].field_2]);
			x86::Move(regMap[reg].name, regMap[RDI].name, threeAC[index].label);
			i++, index--;
		}

		if (i < cnt)
		{
			reg = ensure(varMap[threeAC[index].field_2]);
			x86::Move(regMap[reg].name, regMap[RSI].name, threeAC[index].label);
			i++, index--;
		}

		if (i < cnt)
		{
			reg = ensure(varMap[threeAC[index].field_2]);
			x86::Move(regMap[reg].name, regMap[RDX].name, threeAC[index].label);
			i++, index--;
		}
		
		if (i < cnt)
		{
			reg = ensure(varMap[threeAC[index].field_2]);
			x86::Move(regMap[reg].name, regMap[RCX].name, threeAC[index].label);
			i++, index--;
		}
		
		if (i < cnt)
		{
			reg = ensure(varMap[threeAC[index].field_2]);
			x86::Move(regMap[reg].name, regMap[R8].name, threeAC[index].label);
			i++, index--;
		}
		
		if (i < cnt)
		{
			reg = ensure(varMap[threeAC[index].field_2]);
			x86::Move(regMap[reg].name, regMap[R9].name, threeAC[index].label);
			i++, index--;
		}

		while(code_itr <= index)
		{
			x86::Push(threeAC[code_itr].field_2, threeAC[code_itr].label);
			code_itr++;
		}

		code_itr = tmp;


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

	if (tac.field_3 == "popparam")
	{
		int reg = ensure(varMap[tac.field_1]);
		x86::Move(regMap[RAX].name, regMap[reg].name, tac.label);
	}

	if (tac.field_1 == "push")
	{
		int reg = ensure(varMap[tac.field_2]);
		x86::Move(regMap[reg].name, regMap[RAX].name, tac.label);
	}

	if (tac.field_1 == "begin_function")
	{
		x86::Label(tac.field_2);
	}

	if (tac.field_1 == "end_function")
	{
		
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
		
	}

	if (tac.field_1 == "if")
	{
		
	}

	if (tac.field_2 == "=")
	{
		
	}

}

bool is_var(string name)
{
	if (name != "" && name != "if" && name != "if_false" && name != "return" && name != "call" && name != "shift_sp" && name != "end_function" && name != "begin_function" && name != "push" && name != "pop_param" && name != "param")
	{
		if (name[0] == '$')
		{
			return true;
		}
		// for (size_t index = 0; index < name.size(); ++index) {
		// 	if (!std::isdigit(name[index])) {
		// 		return false;
		// 	}
		// }
		
		// return true;

	}
	return false;
}

void init_next()
{
	for (auto tac : threeAC)
	{
		if (is_var(tac.field_1))
		{
			var_next[tac.field_1].push_back(atoi((tac.label).c_str()));
		}
		if (is_var(tac.field_2))
		{
			var_next[tac.field_2].push_back(atoi((tac.label).c_str()));
		}
		if (is_var(tac.field_3))
		{
			var_next[tac.field_3].push_back(atoi((tac.label).c_str()));
		}
		if (is_var(tac.field_4))
		{
			var_next[tac.field_4].push_back(atoi((tac.label).c_str()));
		}
		if (is_var(tac.field_5))
		{
			var_next[tac.field_5].push_back(atoi((tac.label).c_str()));
		}
	}
}

void generate_assembly()
{
	init_next();
	initialize_assembly();
	for (code_itr=0; code_itr< threeAC.size(); code_itr++)
	{
		modifier(threeAC[code_itr]);
	}

	// write code for changing labels ////////////////////////////////////////////////////
}
