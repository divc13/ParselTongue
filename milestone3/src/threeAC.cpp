#include "include/symtable.hpp"

extern symbolTable* globTable;
extern map<string, int> typeMap;

int label = 0;
int tmp = 0;
map <string, string> stringMap;
symbolTable* table = globTable;
symbolTable* dotTable = NULL;
tableRecord* dotRecord = NULL;
tableRecord* dotRecord_1 = NULL;
stack<pair<string, string>> labelStack;
vector<code> threeAC;
map <string, string> Temporaries;
string MemRg = "";
bool allocate = false;
vector<string> tempExprs;
string isReturn = "";
vector<pair<string, string>> filler;

void fill_stringMap()
{
	int string_index = 0;
	for (int i = 0; i < globTable->currentIndex; i++)
	{
		tableRecord* entry = (globTable->entries)[i];
		assert(entry);
		if (entry->recordType == recordType::CONST_STRING)
		{
			stringMap["\"" + entry->name + "\""] = ".LC" + to_string(string_index);
			string_index++;
		}
	}
}

string newLabel()
{
	return "#L#" + to_string(++label);
}

string newTmp()
{
	return "$t" + to_string(++tmp);
}

string tableHash(symbolTable* curr)
{
	string name = "";
	if(curr -> name != "__GLOBAL__")
	{
		name = curr -> name;
		if (curr -> tableType == tableType::FUNCTION)
		{
			for(int i=0; i<curr->numParams; i++)
			{
				name += "_Zz" + ((curr -> entries)[i])-> type;
			}
			name += "_Nn" + to_string(curr->numParams);
		}
	}
	while(curr -> parentSymtable && curr -> parentSymtable -> name != "__GLOBAL__")
	{
		curr = curr -> parentSymtable;
		name = curr -> name + "_Cc" + name;
	}
	return name;
}

// only for variables and not for functions
string mangle(string name)
{
	if (name[0] == '$') return name;
	tableRecord* entry = table -> lookup(name);
	assert(entry);
	string t1 = tableHash(entry -> symTab);
	if (t1.length()) t1 = "%" + t1;
	string temp = name + t1;
	return temp;
}

void Parasite::formFirstLast()
{

	if (type == "NON_TERMINAL" && (name == "for_stmt" || name == "while_stmt"))
	{
		labelStack.push({first, last});
	}

	if (type == "NON_TERMINAL" && (name == "simple_stmt"))
	{
		if (children[0] -> name == "continue")
		{
			filler.push_back({children[0]->tmp, (labelStack.top()).first});
		}

		if (children[0] -> name == "break")
		{
			filler.push_back({children[0]->tmp, (labelStack.top()).second});
		}
	}

	for (auto &i : children)
		i -> formFirstLast();

	if (type == "NON_TERMINAL" && (name == "for_stmt" || name == "while_stmt"))
	{
		labelStack.pop();
	}
}

void allocate_mem(string size)
{
	code inst;
	inst.field_1 = "param";
	inst.field_2 = size;
	inst.label = newLabel();
	threeAC.push_back(inst);

	inst.field_1 = "call";
	inst.field_2 = "malloc";
	inst.field_3 = "1";
	inst.label = newLabel();
	threeAC.push_back(inst);

	MemRg = newTmp();
	inst.field_1 = MemRg;
	inst.field_2 = "=";
	inst.field_3 = "popparam";
	inst.label = newLabel();
	threeAC.push_back(inst);
}

void Parasite::genAC()
{
	if (type == "NON_TERMINAL")
		if (threeAC.size())
			first = threeAC[threeAC.size() - 1].label;

	if (type != "IDENTIFIER" && name == "file")
	{
					
		/*

			file: ENDMARK 
				| NEWLINE ENDMARK 
				| statements ENDMARK 

		*/
		
		code inst;

		/* print_int */

		inst.field_1 = "begin_function";
		inst.field_2 = tableHash(globTable) + "print_Zzint_Nn1";
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		string print_val = "#var1\%print_Zzint_Nn1";
		inst.field_1 = "pop_param";
		inst.field_2 = print_val;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		string a1 = newTmp();
		inst.field_1 = a1;
		inst.field_2 = "=";
		inst.field_3 = "findAddress";
		inst.field_4 = "integer_format";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "param";
		inst.field_2 = a1;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "param";
		inst.field_2 = print_val;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "call";
		inst.field_2 = "printf";
		inst.field_3 = "2";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "return";
		inst.field_2 = "";
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "end_function";
		inst.field_2 = "";
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		/* print_float */

		inst.field_1 = "begin_function";
		inst.field_2 = tableHash(globTable) + "print_Zzfloat_Nn1";
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		print_val = "#var1\%print_Zzfloat_Nn1";
		inst.field_1 = "pop_param";
		inst.field_2 = print_val;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		a1 = newTmp();
		inst.field_1 = a1;
		inst.field_2 = "=";
		inst.field_3 = "findAddress";
		inst.field_4 = "float_format";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "param";
		inst.field_2 = a1;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "param";
		inst.field_2 = print_val;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "call";
		inst.field_2 = "printf";
		inst.field_3 = "2";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "return";
		inst.field_2 = "";
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "end_function";
		inst.field_2 = "";
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);



		/* print_str */

		inst.field_1 = "begin_function";
		inst.field_2 = tableHash(globTable) + "print_Zzstr_Nn1";
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		print_val = "#var1\%print_Zzstr_Nn1";
		inst.field_1 = "pop_param";
		inst.field_2 = print_val;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		a1 = newTmp();
		inst.field_1 = a1;
		inst.field_2 = "=";
		inst.field_3 = "findAddress";
		inst.field_4 = "string_format";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "param";
		inst.field_2 = a1;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "param";
		inst.field_2 = print_val;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "call";
		inst.field_2 = "printf";
		inst.field_3 = "2";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "return";
		inst.field_2 = "";
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "end_function";
		inst.field_2 = "";
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		/* print_bool */

		inst.field_1 = "begin_function";
		inst.field_2 = tableHash(globTable) + "print_Zzbool_Nn1";
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		print_val = "#var1\%print_Zzbool_Nn1";
		inst.field_1 = "pop_param";
		inst.field_2 = print_val;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		a1 = newTmp();
		inst.field_1 = a1;
		inst.field_2 = "=";
		inst.field_3 = "findAddress";
		inst.field_4 = "integer_format";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "param";
		inst.field_2 = a1;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "param";
		inst.field_2 = print_val;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "call";
		inst.field_2 = "printf";
		inst.field_3 = "2";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "return";
		inst.field_2 = "";
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "end_function";
		inst.field_2 = "";
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);


		/* print_list[int] */

		inst.field_1 = "begin_function";
		inst.field_2 = tableHash(globTable) + "print_Zzlist_int__Nn1";
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		print_val = "#var1\%print_Zzlist_int__Nn1";
		inst.field_1 = "pop_param";
		inst.field_2 = print_val;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		string print1 = newTmp();
		inst.field_1 = print1;
		inst.field_2 = "=";
		inst.field_3 = "*(" + print_val + ")";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		string print2 = newTmp();
		inst.field_1 = print2;
		inst.field_2 = "=";
		inst.field_3 = "1";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		a1 = newTmp();
		inst.field_1 = a1;
		inst.field_2 = "=";
		inst.field_3 = "findAddress";
		inst.field_4 = "list_start_format";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "param";
		inst.field_2 = a1;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "call";
		inst.field_2 = "printf";
		inst.field_3 = "1";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		string printLab = newLabel();
		string print5 = newTmp();
		inst.field_1 = print5;
		inst.field_2 = "=";
		inst.field_3 = print2;
		inst.field_4 = "*";
		inst.field_5 = "8";
		inst.label = printLab;
		threeAC.push_back(inst);

		string print3 = newTmp();
		inst.field_1 = print3;
		inst.field_2 = "=";
		inst.field_3 = print_val;
		inst.field_4 = "+";
		inst.field_5 = print5;
		inst.label = newLabel();
		threeAC.push_back(inst);

		string print4 = newTmp();
		inst.field_1 = print4;
		inst.field_2 = "=";
		inst.field_3 = "*(" + print3 + ")";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "param";
		inst.field_2 = print4;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "call";
		inst.field_2 = "print_Zzint_Nn1";
		inst.field_3 = "1";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		a1 = newTmp();
		inst.field_1 = a1;
		inst.field_2 = "=";
		inst.field_3 = "findAddress";
		inst.field_4 = "comma_format";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "param";
		inst.field_2 = a1;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "call";
		inst.field_2 = "printf";
		inst.field_3 = "1";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = print2;
		inst.field_2 = "=";
		inst.field_3 = print2;
		inst.field_4 = "+";
		inst.field_5 = "1";
		inst.label = newLabel();
		threeAC.push_back(inst);

		string print6 = newTmp();
		inst.field_1 = print6;
		inst.field_2 = "=";
		inst.field_3 = print2;
		inst.field_4 = "<";
		inst.field_5 = print_val;
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "if";
		inst.field_2 = print6;
		inst.field_3 = "goto";
		inst.field_4 = printLab;
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		a1 = newTmp();
		inst.field_1 = a1;
		inst.field_2 = "=";
		inst.field_3 = "findAddress";
		inst.field_4 = "list_end_format";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);
		
		inst.field_1 = "param";
		inst.field_2 = a1;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "call";
		inst.field_2 = "printf";
		inst.field_3 = "1";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "return";
		inst.field_2 = "";
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "end_function";
		inst.field_2 = "";
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);


		/* print_list[str] */

		inst.field_1 = "begin_function";
		inst.field_2 = tableHash(globTable) + "print_Zzlist_str__Nn1";
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		print_val = "#var1\%print_Zzlist_str__Nn1";
		inst.field_1 = "pop_param";
		inst.field_2 = print_val;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		print1 = newTmp();
		inst.field_1 = print1;
		inst.field_2 = "=";
		inst.field_3 = "*(" + print_val + ")";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		print2 = newTmp();
		inst.field_1 = print2;
		inst.field_2 = "=";
		inst.field_3 = "1";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);


		a1 = newTmp();
		inst.field_1 = a1;
		inst.field_2 = "=";
		inst.field_3 = "findAddress";
		inst.field_4 = "list_start_format";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "param";
		inst.field_2 = a1;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "call";
		inst.field_2 = "printf";
		inst.field_3 = "1";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		printLab = newLabel();
		print5 = newTmp();
		inst.field_1 = print5;
		inst.field_2 = "=";
		inst.field_3 = print2;
		inst.field_4 = "*";
		inst.field_5 = "8";
		inst.label = printLab;
		threeAC.push_back(inst);

		print3 = newTmp();
		inst.field_1 = print3;
		inst.field_2 = "=";
		inst.field_3 = print_val;
		inst.field_4 = "+";
		inst.field_5 = print5;
		inst.label = newLabel();
		threeAC.push_back(inst);

		print4 = newTmp();
		inst.field_1 = print4;
		inst.field_2 = "=";
		inst.field_3 = "*(" + print3 + ")";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "param";
		inst.field_2 = print4;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "call";
		inst.field_2 = "print_Zzstr_Nn1";
		inst.field_3 = "1";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		a1 = newTmp();
		inst.field_1 = a1;
		inst.field_2 = "=";
		inst.field_3 = "findAddress";
		inst.field_4 = "comma_format";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);
		
		inst.field_1 = "param";
		inst.field_2 = a1;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "call";
		inst.field_2 = "printf";
		inst.field_3 = "1";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = print2;
		inst.field_2 = "=";
		inst.field_3 = print2;
		inst.field_4 = "+";
		inst.field_5 = "1";
		inst.label = newLabel();
		threeAC.push_back(inst);

		print6 = newTmp();
		inst.field_1 = print6;
		inst.field_2 = "=";
		inst.field_3 = print2;
		inst.field_4 = "<";
		inst.field_5 = print_val;
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "if";
		inst.field_2 = print6;
		inst.field_3 = "goto";
		inst.field_4 = printLab;
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);
		
		a1 = newTmp();
		inst.field_1 = a1;
		inst.field_2 = "=";
		inst.field_3 = "findAddress";
		inst.field_4 = "list_end_format";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "param";
		inst.field_2 = a1;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "call";
		inst.field_2 = "printf";
		inst.field_3 = "1";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "return";
		inst.field_2 = "";
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "end_function";
		inst.field_2 = "";
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);



		/* print_list[float] */

		inst.field_1 = "begin_function";
		inst.field_2 = tableHash(globTable) + "print_Zzlist_float__Nn1";
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		print_val = "#var1\%print_Zzlist_float__Nn1";
		inst.field_1 = "pop_param";
		inst.field_2 = print_val;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		print1 = newTmp();
		inst.field_1 = print1;
		inst.field_2 = "=";
		inst.field_3 = "*(" + print_val + ")";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		print2 = newTmp();
		inst.field_1 = print2;
		inst.field_2 = "=";
		inst.field_3 = "1";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);


		a1 = newTmp();
		inst.field_1 = a1;
		inst.field_2 = "=";
		inst.field_3 = "findAddress";
		inst.field_4 = "list_start_format";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "param";
		inst.field_2 = a1;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "call";
		inst.field_2 = "printf";
		inst.field_3 = "1";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		printLab = newLabel();
		print5 = newTmp();
		inst.field_1 = print5;
		inst.field_2 = "=";
		inst.field_3 = print2;
		inst.field_4 = "*";
		inst.field_5 = "8";
		inst.label = printLab;
		threeAC.push_back(inst);

		print3 = newTmp();
		inst.field_1 = print3;
		inst.field_2 = "=";
		inst.field_3 = print_val;
		inst.field_4 = "+";
		inst.field_5 = print5;
		inst.label = newLabel();
		threeAC.push_back(inst);

		print4 = newTmp();
		inst.field_1 = print4;
		inst.field_2 = "=";
		inst.field_3 = "*(" + print3 + ")";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "param";
		inst.field_2 = print4;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "call";
		inst.field_2 = "print_Zzfloat_Nn1";
		inst.field_3 = "1";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		a1 = newTmp();
		inst.field_1 = a1;
		inst.field_2 = "=";
		inst.field_3 = "findAddress";
		inst.field_4 = "comma_format";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);
		
		inst.field_1 = "param";
		inst.field_2 = a1;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "call";
		inst.field_2 = "printf";
		inst.field_3 = "1";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = print2;
		inst.field_2 = "=";
		inst.field_3 = print2;
		inst.field_4 = "+";
		inst.field_5 = "1";
		inst.label = newLabel();
		threeAC.push_back(inst);

		print6 = newTmp();
		inst.field_1 = print6;
		inst.field_2 = "=";
		inst.field_3 = print2;
		inst.field_4 = "<";
		inst.field_5 = print_val;
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "if";
		inst.field_2 = print6;
		inst.field_3 = "goto";
		inst.field_4 = printLab;
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);
		
		a1 = newTmp();
		inst.field_1 = a1;
		inst.field_2 = "=";
		inst.field_3 = "findAddress";
		inst.field_4 = "list_end_format";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "param";
		inst.field_2 = a1;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "call";
		inst.field_2 = "printf";
		inst.field_3 = "1";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "return";
		inst.field_2 = "";
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "end_function";
		inst.field_2 = "";
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);


		/* print_list[float] */

		inst.field_1 = "begin_function";
		inst.field_2 = tableHash(globTable) + "print_Zzlist_bool__Nn1";
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		print_val = "#var1\%print_Zzlist_bool__Nn1";
		inst.field_1 = "pop_param";
		inst.field_2 = print_val;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		print1 = newTmp();
		inst.field_1 = print1;
		inst.field_2 = "=";
		inst.field_3 = "*(" + print_val + ")";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		print2 = newTmp();
		inst.field_1 = print2;
		inst.field_2 = "=";
		inst.field_3 = "1";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);


		a1 = newTmp();
		inst.field_1 = a1;
		inst.field_2 = "=";
		inst.field_3 = "findAddress";
		inst.field_4 = "list_start_format";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "param";
		inst.field_2 = a1;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "call";
		inst.field_2 = "printf";
		inst.field_3 = "1";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		printLab = newLabel();
		print5 = newTmp();
		inst.field_1 = print5;
		inst.field_2 = "=";
		inst.field_3 = print2;
		inst.field_4 = "*";
		inst.field_5 = "8";
		inst.label = printLab;
		threeAC.push_back(inst);

		print3 = newTmp();
		inst.field_1 = print3;
		inst.field_2 = "=";
		inst.field_3 = print_val;
		inst.field_4 = "+";
		inst.field_5 = print5;
		inst.label = newLabel();
		threeAC.push_back(inst);

		print4 = newTmp();
		inst.field_1 = print4;
		inst.field_2 = "=";
		inst.field_3 = "*(" + print3 + ")";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "param";
		inst.field_2 = print4;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "call";
		inst.field_2 = "print_Zzbool_Nn1";
		inst.field_3 = "1";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		a1 = newTmp();
		inst.field_1 = a1;
		inst.field_2 = "=";
		inst.field_3 = "findAddress";
		inst.field_4 = "comma_format";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);
		
		inst.field_1 = "param";
		inst.field_2 = a1;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "call";
		inst.field_2 = "printf";
		inst.field_3 = "1";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = print2;
		inst.field_2 = "=";
		inst.field_3 = print2;
		inst.field_4 = "+";
		inst.field_5 = "1";
		inst.label = newLabel();
		threeAC.push_back(inst);

		print6 = newTmp();
		inst.field_1 = print6;
		inst.field_2 = "=";
		inst.field_3 = print2;
		inst.field_4 = "<";
		inst.field_5 = print_val;
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "if";
		inst.field_2 = print6;
		inst.field_3 = "goto";
		inst.field_4 = printLab;
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);
		
		a1 = newTmp();
		inst.field_1 = a1;
		inst.field_2 = "=";
		inst.field_3 = "findAddress";
		inst.field_4 = "list_end_format";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "param";
		inst.field_2 = a1;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "call";
		inst.field_2 = "printf";
		inst.field_3 = "1";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "return";
		inst.field_2 = "";
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "end_function";
		inst.field_2 = "";
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);


		inst.field_1 = "begin_function";
		inst.field_2 = tableHash(globTable) + "range_Zzint_Nn1";
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		string val = "#var2\%range_Zzint_Nn1";
		inst.field_1 = "pop_param";
		inst.field_2 = val;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		string tmpry = newTmp();
		inst.field_1 = tmpry;
		inst.field_2 = "=";
		inst.field_3 = val;
		inst.field_4 = "+";
		inst.field_5 = "1";
		inst.label = newLabel();
		threeAC.push_back(inst);

		string tmpry_2 = newTmp();
		inst.field_1 = tmpry_2;
		inst.field_2 = "=";
		inst.field_3 = tmpry;
		inst.field_4 = "*";
		inst.field_5 = "8";
		inst.label = newLabel();
		threeAC.push_back(inst);

		allocate_mem(tmpry_2);

		string l1 = newLabel();
		string t1 = newTmp();
		string t2 = newTmp();
		string t3 = newTmp();
		string t4 = newTmp();
		t1 = MemRg;

		inst.field_1 = "*(" + t1 + ")";
		inst.field_2 = "=";
		inst.field_3 = val;
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = t2;
		inst.field_2 = "=";
		inst.field_3 = "1";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = t4;
		inst.field_2 = "=";
		inst.field_3 = t2;
		inst.field_4 = "*";
		inst.field_5 = "8";
		inst.label = l1;
		threeAC.push_back(inst);

		string loc = newTmp();
		inst.field_1 = loc;
		inst.field_2 = "=";
		inst.field_3 = t1;
		inst.field_4 = "+";
		inst.field_5 = t4;
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "*(" + loc + ")";
		inst.field_2 = "=";
		inst.field_3 = t2;
		inst.field_4 = "-";
		inst.field_5 = "1";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = t2;
		inst.field_2 = "=";
		inst.field_3 = t2;
		inst.field_4 = "+";
		inst.field_5 = "1";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = t3;
		inst.field_2 = "=";
		inst.field_3 = t2;
		inst.field_4 = "<";
		inst.field_5 = tmpry;
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "if";
		inst.field_2 = t3;
		inst.field_3 = "goto";
		inst.field_4 = l1;
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "push";
		inst.field_2 = t1;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "return";
		inst.field_2 = "";
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "end_function";
		inst.field_2 = "";
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);



		/* range (int x, int y) */
		
		inst.field_1 = "begin_function";
		inst.field_2 = tableHash(globTable) + "range_Zzint_Zzint_Nn2";
		inst.label = newLabel();
		threeAC.push_back(inst);

		string val1 = "#var1\%range_Zzint_Zzint_Nn2";
		string val2 = "#var2\%range_Zzint_Zzint_Nn2";
		string val3 = newTmp();
		tmpry_2 = newTmp();
		string tmpry_3 = newTmp();
		string tmpry_4 = newTmp();

		inst.field_1 = "pop_param";
		inst.field_2 = val1;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "pop_param";
		inst.field_2 = val2;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = val3;
		inst.field_2 = "=";
		inst.field_3 = val2;
		inst.field_4 = "-";
		inst.field_5 = val1;
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = tmpry_2;
		inst.field_2 = "=";
		inst.field_3 = val3;
		inst.field_4 = "+";
		inst.field_5 = "1";
		inst.label = newLabel();
		threeAC.push_back(inst);

		tmpry_3 = newTmp();
		inst.field_1 = tmpry_3;
		inst.field_2 = "=";
		inst.field_3 = tmpry_2;
		inst.field_4 = "*";
		inst.field_5 = "8";
		inst.label = newLabel();
		threeAC.push_back(inst);

		string l2 = newLabel();
		string t5 = newTmp();
		string t6 = newTmp();
		string t7 = newTmp();
		string t8 = newTmp();
		string t9 = newTmp();
		string t10 = newTmp();

		allocate_mem(tmpry_3);
		t5 = MemRg;

		inst.field_1 = t6;
		inst.field_2 = "=";
		inst.field_3 = val1;
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "*(" + t5 +  ")";
		inst.field_2 = "=";
		inst.field_3 = val3;
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = t9;
		inst.field_2 = "=";
		inst.field_3 = t6;
		inst.field_4 = "-";
		inst.field_5 = val1;
		inst.label = l2;
		threeAC.push_back(inst);

		inst.field_1 = tmpry_4;
		inst.field_2 = "=";
		inst.field_3 = t9;
		inst.field_4 = "+";
		inst.field_5 = "1";
		inst.label = l2;
		threeAC.push_back(inst);

		inst.field_1 = t8;
		inst.field_2 = "=";
		inst.field_3 = tmpry_4;
		inst.field_4 = "*";
		inst.field_5 = "8";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = t10;
		inst.field_2 = "=";
		inst.field_3 = t5;
		inst.field_4 = "+";
		inst.field_5 = t8;
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "*(" + t10 + ")";
		inst.field_2 = "=";
		inst.field_3 = t6;
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = t6;
		inst.field_2 = "=";
		inst.field_3 = t6;
		inst.field_4 = "+";
		inst.field_5 = "1";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = t7;
		inst.field_2 = "=";
		inst.field_3 = t6;
		inst.field_4 = "<";
		inst.field_5 = val2;
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "if";
		inst.field_2 = t7;
		inst.field_3 = "goto";
		inst.field_4 = l2;
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "push";
		inst.field_2 = t5;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "return";
		inst.field_2 = "";
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "end_function";
		inst.field_2 = "";
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);


		/* len (str) */

		inst.field_1 = "begin_function";
		inst.field_2 = tableHash(globTable) + "len_Zzstr_Nn1";
		inst.label = newLabel();
		threeAC.push_back(inst);

		string val7 = "#var1\%len_Zzstr_Nn1";
		string val4 = newTmp();

		inst.field_1 = "pop_param";
		inst.field_2 = val7;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = val7;
		inst.field_2 = "=";
		inst.field_3 = val7;
		inst.field_4 = "-";
		inst.field_5 = "8";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = val4;
		inst.field_2 = "=";
		inst.field_3 = "*(" + val7 + ")";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "push";
		inst.field_2 = val4;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "return";
		inst.field_2 = "";
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = "end_function";
		inst.field_2 = "";
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		for(auto i: typeMap)
		{
			if((i.first).compare(0, 4, "list") == 0)
			{
				string type = i.first;

				inst.field_1 = "begin_function";
				inst.field_2 = tableHash(globTable) + "len_Zz" + type + "_Nn1";
				inst.label = newLabel();
				threeAC.push_back(inst);

				string val5 = "#len\%len_Zz" + type + "_Nn1";
				string val6 = newTmp();

				inst.field_1 = "pop_param";
				inst.field_2 = val5;
				inst.field_3 = "";
				inst.field_4 = "";
				inst.field_5 = "";
				inst.label = newLabel();
				threeAC.push_back(inst);

				inst.field_1 = val6;
				inst.field_2 = "=";
				inst.field_3 = "*(" + val5 + ")";
				inst.field_4 = "";
				inst.field_5 = "";
				inst.label = newLabel();
				threeAC.push_back(inst);

				inst.field_1 = "push";
				inst.field_2 = val6;
				inst.field_3 = "";
				inst.field_4 = "";
				inst.field_5 = "";
				inst.label = newLabel();
				threeAC.push_back(inst);

				inst.field_1 = "return";
				inst.field_2 = "";
				inst.field_3 = "";
				inst.field_4 = "";
				inst.field_5 = "";
				inst.label = newLabel();
				threeAC.push_back(inst);

				inst.field_1 = "end_function";
				inst.field_2 = "";
				inst.field_3 = "";
				inst.field_4 = "";
				inst.field_5 = "";
				inst.label = newLabel();
				threeAC.push_back(inst);

			}
		}
	   
	}


	if (type != "IDENTIFIER" && name == "statements")
	{
					
		/*

			statements: statements statement
				| statement

		*/

	}


	if (type != "IDENTIFIER" && name == "statement")
	{
					
		/*

			statement: compound_stmt 
				| simple_stmts

		*/

		/* nothing to do */

	}


	if (type != "IDENTIFIER" && name == "simple_stmts")
	{
					
		/*

			simple_stmts: simple1 simple2 NEWLINE

		*/
	
	}


	if (type != "IDENTIFIER" && name == "simple1")
	{
					
		/*

			simple1: simple_stmt
				| simple1 DLM_SM_COL simple_stmt

		*/

	}


	if (type != "IDENTIFIER" && name == "simple2")
	{
					
		/*

			simple2: 
				| DLM_SM_COL

		*/
	
		/* nothing to do */

	}


	if (type != "IDENTIFIER" && name == "simple_stmt")
	{
					
		/*

			simple_stmt: assignment
				| expressions 
				| return_stmt
				| KW_break 
				| KW_continue 

		*/

	}


	if (type != "IDENTIFIER" && name == "compound_stmt")
	{
					
		/*

			compound_stmt: function_def
				| if_stmt
				| class_def
				| for_stmt
				| while_stmt

		*/
	}


	if (type != "IDENTIFIER" && name == "typedecl")
	{
					
		/*

			typedecl: l_primary DLM_COLON expression

		*/
		
	}


	if (type != "IDENTIFIER" && name == "assignment")
	{
					
		/*

			assignment: typedecl
				| typedecl OP_ASN_ASN expression 
				| l_primary augassign expression 
				| multi_targets_assgn   

		*/

	}


	if (type != "IDENTIFIER" && name == "multi_targets_assgn")
	{
					
		/*

			multi_targets_assgn: l_primary OP_ASN_ASN expression
				| l_primary OP_ASN_ASN multi_targets_assgn

		*/

	}


	if (type != "IDENTIFIER" && name == "augassign")
	{
					
		/*

			augassign: OP_ASN_ADD
				| OP_ASN_SUB
				| OP_ASN_MUL
				| OP_ASN_DIV
				| OP_ASN_FDIV
				| OP_ASN_MOD
				| OP_ASN_POW
				| OP_ASN_AND
				| OP_ASN_OR 
				| OP_ASN_XOR 
				| OP_ASN_LS 
				| OP_ASN_RS

		*/

		/* nothing to do */

	}


	if (type != "IDENTIFIER" && name == "return_stmt")
	{
					
		/*

			return_stmt: KW_return expression
				| KW_return 

		*/

	}


	if (type != "IDENTIFIER" && name == "block")
	{
					
		/*

			block: NEWLINE INDENT statements DEDENT 
				| simple_stmts

		*/

	}


	if (type != "IDENTIFIER" && name == "class_def")
	{
					
		/*

			class_def: KW_class NAME is_arguments DLM_COLON block

		*/

		tableRecord* entry = table -> lookup(children[1] -> name, recordType::TYPE_CLASS);
		assert (entry);
		assert (entry -> symTab);
		table = entry -> symTab;

	}


	if (type != "IDENTIFIER" && name == "function_def")
	{
					
		/*

			function_def: KW_def NAME DLM_LFT_PRN params DLM_RGT_PRN is_fn_expression DLM_COLON block

		*/

		/* class constructors are not an issue here */

		string funcName = tableHash(table);
		if (funcName.length()) funcName += "_Cc";
		funcName += children[1] -> name;

		tableRecord* entry = table -> lookup(children[1] -> name);
		
		assert (entry);
		assert (entry -> recordType == recordType::TYPE_FUNCTION);
		assert (entry -> symTab);
		table = entry -> symTab;

		for (int i = 0; i < table -> numParams; i++)
		{
			funcName += "_Zz" + (table -> entries)[i] -> type;
		}

		funcName += "_Nn" + to_string(table -> numParams);

		cout << "FUNCNAME: " << funcName << endl;

		if(table -> name == "main") funcName = "main";

		code inst;
		inst.field_1 = "begin_function";
		inst.field_2 = funcName;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

	}


	if (type != "IDENTIFIER" && name == "is_arguments")
	{
					
		/*

			is_arguments:
				| DLM_LFT_PRN DLM_RGT_PRN
				| DLM_LFT_PRN arguments DLM_RGT_PRN

		*/

	}


	if (type != "IDENTIFIER" && name == "is_fn_expression")
	{
					
		/*

			is_fn_expression:
				| DLM_TO expression

		*/

	}


	if (type != "IDENTIFIER" && name == "params")
	{
					
		/*

			params: 
				| param param_nd 

		*/
	}


	if (type != "IDENTIFIER" && name == "param_nd")
	{
					
		/*

			param_nd: 
				| param_no_default param_nd
				| DLM_COMMA

		*/

	}


	if (type != "IDENTIFIER" && name == "param_no_default")
	{
					
		/*

			param_no_default: DLM_COMMA param 

		*/
	}


	if (type != "IDENTIFIER" && name == "param")
	{
					
		/*

			param: NAME 
				| annotation

		*/

	}


	if (type != "IDENTIFIER" && name == "annotation")
	{
					
		/*

			annotation: NAME DLM_COLON expression 

		*/

	}


	if (type != "IDENTIFIER" && name == "if_stmt")
	{
					
		/*

			if_stmt: KW_if expression DLM_COLON block elif_stmt 
				| KW_if expression DLM_COLON block 
				| KW_if expression DLM_COLON block else_block 

		*/
	}


	if (type != "IDENTIFIER" && name == "elif_stmt")
	{
					
		/*

			elif_stmt: KW_elif expression DLM_COLON block elif_stmt 
				| KW_elif expression DLM_COLON block 
				| KW_elif expression DLM_COLON block else_block 

		*/
	}


	if (type != "IDENTIFIER" && name == "else_block")
	{
					
		/*

			else_block: KW_else DLM_COLON block 

		*/

	}


	if (type != "IDENTIFIER" && name == "while_stmt")
	{
					
		/*

			while_stmt: KW_while expression DLM_COLON block 
				| KW_while expression DLM_COLON block else_block

		*/

	}


	if (type != "IDENTIFIER" && name == "for_stmt")
	{
					
		/*

			for_stmt: KW_for for_expr DLM_COLON block
				| KW_for for_expr DLM_COLON block else_block

		*/

	}

	if (type != "IDENTIFIER" && name == "for_expr")
	{
					
		/*
			for_expr: l_primary KW_in expression
		*/

	}


	if (type != "IDENTIFIER" && name == "expressions")
	{
					
		/*

			expressions: expression DLM_COMMA expressions
				| expression DLM_COMMA
				| expression   

		*/

	}


	if (type != "IDENTIFIER" && name == "expression")
	{
					
		/*
			expression: disjunction             
		*/
	}


	if (type != "IDENTIFIER" && name == "disjunction")
	{
					
		/*

			disjunction: conjunction
				| disjunction OP_LOG_OR conjunction

		*/

	}


	if (type != "IDENTIFIER" && name == "conjunction")
	{
					
		/*

			conjunction: inversion
				| conjunction OP_LOG_AND inversion

		*/

	}


	if (type != "IDENTIFIER" && name == "inversion")
	{
					
		/*

			inversion: OP_LOG_NOT inversion 
				| comparison

		*/

	}


	if (type != "IDENTIFIER" && name == "comparison")
	{
					
		/*

			comparison: bitwise_or
				| comparison bitwise_operator bitwise_or

		*/
	}


	if (type != "IDENTIFIER" && name == "bitwise_operator")
	{
					
		/*

			bitwise_operator: OP_REL_EQ
				| OP_REL_NEQ
				| OP_REL_LTE
				| OP_REL_LT
				| OP_REL_GTE
				| OP_REL_GT
				| KW_in
				| KW_is

		*/

		/* do nothing */

	}


	if (type != "IDENTIFIER" && name == "bitwise_or")
	{
					
		/*

			bitwise_or: bitwise_or OP_BIT_OR bitwise_xor 
				| bitwise_xor

		*/

	}


	if (type != "IDENTIFIER" && name == "bitwise_xor")
	{
					
		/*

			bitwise_xor: bitwise_xor OP_BIT_XOR bitwise_and 
				| bitwise_and

		*/

	}


	if (type != "IDENTIFIER" && name == "bitwise_and")
	{
					
		/*

			bitwise_and: bitwise_and OP_BIT_AND shift_expr 
				| shift_expr

		*/
	}


	if (type != "IDENTIFIER" && name == "shift_expr")
	{
					
		/*

			shift_expr: shift_expr OP_BIT_LS sum 
				| shift_expr OP_BIT_RS sum 
				| sum

		*/
	}


	if (type != "IDENTIFIER" && name == "sum")
	{
					
		/*

			sum: sum OP_ATH_ADD term 
				| sum OP_ATH_SUB term 
				| term

		*/
	}


	if (type != "IDENTIFIER" && name == "term")
	{
					
		/*

			term: term OP_ATH_MUL factor 
				| term OP_ATH_DIV factor 
				| term OP_ATH_FDIV factor 
				| term OP_ATH_MOD factor 
				| factor

		*/
	}


	if (type != "IDENTIFIER" && name == "factor")
	{
					
		/*

			factor: OP_ATH_ADD factor 
				| OP_ATH_SUB factor 
				| OP_BIT_NEG factor 
				| power

		*/
	}


	if (type != "IDENTIFIER" && name == "power")
	{
					
		/*

			power: primary OP_ATH_POW factor 
				| primary

		*/
	}


	if (type != "IDENTIFIER" && name == "function_call")
	{
					
		/*
			function_call: NAME DLM_LFT_PRN args DLM_RGT_PRN
		*/
	}


	if (type != "IDENTIFIER" && name == "args")
	{
					
		/*

			args:
				| expressions

		*/

	}


	if (type != "IDENTIFIER" && name == "list_access")
	{
					
		/*

			list_access: NAME DLM_LFT_SQ expression DLM_RGT_SQ

		*/
	}


	if (type != "IDENTIFIER" && name == "l_primary")
	{
					
		/*

			l_primary:  NAME DLM_DOT NAME 
				| list_access 
				| atom

		*/
	}


	if (type != "IDENTIFIER" && name == "primary")
	{
					
		/*

			primary: l_primary
				| NAME DLM_DOT function_call
				| function_call

		*/

		if (children.size() == 3 && children[1] -> name == ".")
		{
			tableRecord* entry = table -> lookup(children[0] -> name);
			assert (entry);
			dotRecord_1 = entry;
			entry = globTable -> lookup_table(entry ->type, recordType::TYPE_CLASS);
			assert (entry);
			dotTable = entry -> symTab;
			dotRecord = entry;
		}

	}


	if (type != "IDENTIFIER" && name == "atom")
	{
					
		/*

			atom: NAME
				| KW_True 
				| KW_False 
				| KW_None 
				| strings
				| FLOAT_LITERAL
				| INT_LITERAL
				| group
				| list

		*/

	}


	if (type != "IDENTIFIER" && name == "group")
	{
					
		/*

			group: DLM_LFT_PRN expression DLM_RGT_PRN 
				| DLM_LFT_PRN DLM_RGT_PRN

		*/

	}


	if (type != "IDENTIFIER" && name == "string")
	{
					
		/*
			string: STRING_LITERAL              
		*/

		/* do nothing */

	}


	if (type != "IDENTIFIER" && name == "strings")
	{
					
		/*

			strings: strings string
				| string

		*/

	}

	if (type != "IDENTIFIER" && name == "list")
	{
					
		/*

			list: DLM_LFT_SQ expressions DLM_RGT_SQ 

		*/

		allocate = true;

	}



	/////////////////////////////////////////// DFS BEGIN ///////////////////////////////////////////////////
	/////////////////////////////////////////// DFS BEGIN ///////////////////////////////////////////////////
	/////////////////////////////////////////// DFS BEGIN ///////////////////////////////////////////////////
	/////////////////////////////////////////// DFS BEGIN ///////////////////////////////////////////////////

	// cout << name << endl;
	for (auto &i : children)
	{	
		// cout << "children : " << i -> name << endl;	
		i -> genAC();
		
	}

	// cout << "return " << name << endl;
	
	/////////////////////////////////////////// DFS END ///////////////////////////////////////////////////
	/////////////////////////////////////////// DFS END ///////////////////////////////////////////////////
	/////////////////////////////////////////// DFS END ///////////////////////////////////////////////////
	/////////////////////////////////////////// DFS END ///////////////////////////////////////////////////
	/////////////////////////////////////////// DFS END ///////////////////////////////////////////////////



	if (type != "IDENTIFIER" && name == "file")
	{
					
		/*

			file: ENDMARK 
				| NEWLINE ENDMARK 
				| statements ENDMARK 

		*/

		/* nothing to do */
	   
	}


	if (type != "IDENTIFIER" && name == "statements")
	{
					
		/*

			statements: statements statement
				| statement

		*/

		/* nothing to do */

	}


	if (type != "IDENTIFIER" && name == "statement")
	{
					
		/*

			statement: compound_stmt 
				| simple_stmts

		*/

		/* nothing to do */

	}


	if (type != "IDENTIFIER" && name == "simple_stmts")
	{
					
		/*

			simple_stmts: simple1 simple2 NEWLINE

		*/

	
		/* nothing to do */

	}


	if (type != "IDENTIFIER" && name == "simple1")
	{
					
		/*

			simple1: simple_stmt
				| simple1 DLM_SM_COL simple_stmt

		*/
	
		/* nothing to do */

	}


	if (type != "IDENTIFIER" && name == "simple2")
	{
					
		/*

			simple2: 
				| DLM_SM_COL

		*/
	
		/* nothing to do */

	}


	if (type != "IDENTIFIER" && name == "simple_stmt")
	{
					
		/*

			simple_stmt: assignment
				| expressions 
				| return_stmt
				| KW_break 
				| KW_continue 

		*/

		if (children[0] -> name == "expressions")
		{
			tempExprs.clear();
		}

		if (children[0] -> name == "continue")
		{
			code inst;
			string label = newLabel();
			inst.field_1 = "goto";
			inst.field_2 = "@FILL_LATER@";
			inst.field_3 = "";
			inst.field_4 = "";
			inst.field_5 = "";
			inst.label = label;
			threeAC.push_back(inst);
			children[0] -> tmp = label;
		}

		if (children[0] -> name == "break")
		{
			code inst;
			string label = newLabel();
			inst.field_1 = "goto";
			inst.field_2 = "@FILL_LATER@";
			inst.field_3 = "";
			inst.field_4 = "";
			inst.field_5 = "";
			inst.label = label;
			threeAC.push_back(inst);
			children[0] -> tmp = label;

		}

	}


	if (type != "IDENTIFIER" && name == "compound_stmt")
	{
					
		/*

			compound_stmt: function_def
				| if_stmt
				| class_def
				| for_stmt
				| while_stmt

		*/

		/* nothing to do */

		

	}


	if (type != "IDENTIFIER" && name == "typedecl")
	{
					
		/*

			typedecl: l_primary DLM_COLON expression

		*/

		tmp = children[0] -> tmp;
		string type = host -> dataType;

		// if (type == "int" || type == "float" || type == "bool" || type == "str" || type.compare(0, 4, "list") == 0)
		// {
		// 	// do nothing
		// }

		// else
		// {
		// 	// handle class objects
		// 	tableRecord* entry = globTable -> lookup_table(type, recordType::TYPE_CLASS);
		// 	assert (entry);
		// 	int size = entry -> symTab -> size;
		// 	allocate_mem(to_string(size));
		// 	tmp = MemRg;
		// }

	}


	if (type != "IDENTIFIER" && name == "assignment")
	{
					
		/*

			assignment: typedecl
				| typedecl OP_ASN_ASN expression 
				| l_primary augassign expression 
				| multi_targets_assgn   

		*/

		if (children[0] -> name == "typedecl" && children.size() == 3)
		{
			code inst;
			inst.field_1 = children[0] -> tmp;
			inst.field_2 = "=";
			inst.field_3 = children[2] -> tmp;
			inst.field_4 = "";
			inst.field_5 = "";
			inst.label = newLabel();
			threeAC.push_back(inst);
		}

		if (children[0] -> name == "l_primary" && children.size() == 3)
		{

			int operation_len = (children[1] -> name).length();
			string operation = (children[1] -> name).substr(0, operation_len - 1); 

			code inst;
			inst.field_1 = children[0] -> tmp;
			inst.field_2 = "=";
			inst.field_3 = children[0] -> tmp;
			inst.field_4 = operation;
			inst.field_5 = children[2] -> tmp;
			inst.label = newLabel();
			threeAC.push_back(inst);

		}


	}


	if (type != "IDENTIFIER" && name == "multi_targets_assgn")
	{
					
		/*

			multi_targets_assgn: l_primary OP_ASN_ASN expression
				| l_primary OP_ASN_ASN multi_targets_assgn

		*/

		code inst;
		inst.field_1 = children[0] -> tmp;
		inst.field_2 = "=";
		inst.field_3 = children[2] -> tmp;
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);
		tmp = children[0] -> tmp;

	}


	if (type != "IDENTIFIER" && name == "augassign")
	{
					
		/*

			augassign: OP_ASN_ADD
				| OP_ASN_SUB
				| OP_ASN_MUL
				| OP_ASN_DIV
				| OP_ASN_FDIV
				| OP_ASN_MOD
				| OP_ASN_POW
				| OP_ASN_AND
				| OP_ASN_OR 
				| OP_ASN_XOR 
				| OP_ASN_LS 
				| OP_ASN_RS

		*/

		name = children[0] -> name;

	}


	if (type != "IDENTIFIER" && name == "return_stmt")
	{
					
		/*

			return_stmt: KW_return expression
				| KW_return 

		*/

		code inst;

		if (children.size() == 2)
		{
			inst.field_1 = "push";
			inst.field_2 = children[1] -> tmp;
			inst.label = newLabel();
			threeAC.push_back(inst);
			isReturn = children[1] -> tmp;
		}

		inst.field_1 = "return";
		inst.field_2 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

	}


	if (type != "IDENTIFIER" && name == "block")
	{
					
		/*

			block: NEWLINE INDENT statements DEDENT 
				| simple_stmts

		*/

		/* do nothing */

	}


	if (type != "IDENTIFIER" && name == "class_def")
	{
					
		/*

			class_def: KW_class NAME is_arguments DLM_COLON block

		*/
		table = globTable;

	}


	if (type != "IDENTIFIER" && name == "function_def")
	{
					
		/*
			function_def: KW_def NAME DLM_LFT_PRN params DLM_RGT_PRN is_fn_expression DLM_COLON block
		*/

		table = table -> parentSymtable;
		code inst;
		inst.field_1 = "end_function";
		inst.label = newLabel();
		threeAC.push_back(inst);

	}


	if (type != "IDENTIFIER" && name == "is_arguments")
	{
					
		/*

			is_arguments:
				| DLM_LFT_PRN DLM_RGT_PRN
				| DLM_LFT_PRN arguments DLM_RGT_PRN

		*/

		/* do nothing */

	}


	if (type != "IDENTIFIER" && name == "is_fn_expression")
	{
					
		/*

			is_fn_expression:
				| DLM_TO expression

		*/

		/* do nothing */
	}


	if (type != "IDENTIFIER" && name == "params")
	{
					
		/*

			params: 
				| param param_nd 

		*/

		/* do nothing */
	}


	if (type != "IDENTIFIER" && name == "param_nd")
	{
					
		/*

			param_nd: 
				| param_no_default param_nd
				| DLM_COMMA

		*/

		/* do nothing */

	}


	if (type != "IDENTIFIER" && name == "param_no_default")
	{
					
		/*

			param_no_default: DLM_COMMA param 

		*/
	}


	if (type != "IDENTIFIER" && name == "param")
	{
					
		/*

			param: NAME 
				| annotation

		*/

		if (children[0] -> type == "IDENTIFIER")
		{
			code inst;

			inst.field_1 = "pop_param";
			inst.field_2 = mangle(children[0] -> name);
			inst.field_3 = "";
			inst.label = newLabel();
			threeAC.push_back(inst); 
		}

	}


	if (type != "IDENTIFIER" && name == "annotation")
	{
					
		/*
			annotation: NAME DLM_COLON expression 
		*/

		code inst;
		inst.field_1 = "pop_param";
		inst.field_2 = mangle(children[0] -> name);
		inst.label = newLabel();
		threeAC.push_back(inst);

	}


	if (type != "IDENTIFIER" && name == "if_stmt")
	{
					
		/*

			if_stmt: KW_if expression DLM_COLON block elif_stmt 
				| KW_if expression DLM_COLON block 
				| KW_if expression DLM_COLON block else_block 

		*/


		code inst;
		int loc = -1;
		string label = newLabel();
		for (int i = 0; i < threeAC.size(); i++)
		{
			if (threeAC[i].label == children[3]->first)
			{
				loc = i;
				break;
			}
		}

		assert (loc != -1);

		inst.field_1 = "if_false";
		inst.field_2 = children[1] -> tmp;
		inst.field_3 = "goto";
		inst.field_4 = "@FILL_LATER@";
		inst.label = label;
		threeAC.insert(threeAC.begin() + loc + 1, inst);


		if (children[3] -> first == children[3] -> last)
		{
			filler.push_back({label, label});
		}
		else{
			filler.push_back({label, children[3]->last});
		}



		if (children.size() == 5)
		{

			loc = -1;
			label = newLabel();
			for (int i = 0; i < threeAC.size(); i++)
			{
				if (threeAC[i].label == children[4]->first)
				{
					loc = i;
					break;
				}
			}

			assert (loc != -1);

			inst.field_1 = "if";
			inst.field_2 = children[1] -> tmp;
			inst.field_3 = "goto";
			inst.field_4 = "@FILL_LATER@";
			inst.label = label;
			threeAC.insert(threeAC.begin() + loc + 1, inst);

			if (children[4] -> first == children[4] -> last)
			{
				filler.push_back({label, label});
			}
			else{
				filler.push_back({label, children[4]->last});
			}

			// filler.push_back({label, children[4]->last});
		}
		
	}


	if (type != "IDENTIFIER" && name == "elif_stmt")
	{
					
		/*

			elif_stmt: KW_elif expression DLM_COLON block elif_stmt 
				| KW_elif expression DLM_COLON block 
				| KW_elif expression DLM_COLON block else_block 

		*/

		code inst;
		int loc = -1;
		string label = newLabel();
		for (int i = 0; i < threeAC.size(); i++)
		{
			if (threeAC[i].label == children[3]->first)
			{
				loc = i;
				break;
			}
		}

		assert (loc != -1);

		inst.field_1 = "if_false";
		inst.field_2 = children[1] -> tmp;
		inst.field_3 = "goto";
		inst.field_4 = "@FILL_LATER@";
		inst.label = label;
		threeAC.insert(threeAC.begin() + loc + 1, inst);

		// filler.push_back({label, children[3]->last});

		if (children[3] -> first == children[3] -> last)
		{
			filler.push_back({label, label});
		}
		else{
			filler.push_back({label, children[3]->last});
		}

		if (children.size() == 5)
		{

			loc = -1;
			label = newLabel();
			for (int i = 0; i < threeAC.size(); i++)
			{
				if (threeAC[i].label == children[4]->first)
				{
					loc = i;
					break;
				}
			}

			assert (loc != -1);

			inst.field_1 = "if";
			inst.field_2 = children[1] -> tmp;
			inst.field_3 = "goto";
			inst.field_4 = "@FILL_LATER@";
			inst.label = label;

			threeAC.insert(threeAC.begin() + loc + 1, inst);
			if (children[4] -> first == children[4] -> last)
			{
				filler.push_back({label, label});
			}
			else{
				filler.push_back({label, children[4]->last});
			}

			// filler.push_back({label, children[4]->last});
		}

	}


	if (type != "IDENTIFIER" && name == "else_block")
	{
					
		/*

			else_block: KW_else DLM_COLON block 

		*/

		/* do nothing */

	}


	if (type != "IDENTIFIER" && name == "while_stmt")
	{
					
		/*

			while_stmt: KW_while expression DLM_COLON block 
				| KW_while expression DLM_COLON block else_block 

		*/

		code inst;
		int loc = -1;
		int loc_2 = -1;
		int loc_3 = -1;
		string label = newLabel();
		string const_label = label;
		for (int i = 0; i < threeAC.size(); i++)
		{
			if (threeAC[i].label == children[3]->first)
			{
				loc = i;
			}

			if (threeAC[i].label == children[1]->first)
			{
				loc_2 = i;
			}

			if (threeAC[i].label == children[3]->last)
			{
				loc_3 = i;
			}
		}

		assert (loc != -1);
		assert (loc_2 != -1);
		assert (loc_3 != -1);

		inst.field_1 = "if_false";
		inst.field_2 = children[1] -> tmp;
		inst.field_3 = "goto";
		inst.field_4 = "@FILL_LATER@";
		inst.label = const_label;
		threeAC.insert(threeAC.begin() + loc + 1, inst);

		string t2 = threeAC[loc_2 + 1].label;
		label = newLabel();

		inst.field_1 = "goto";
		inst.field_2 = t2;
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = label;
		threeAC.insert(threeAC.begin() + loc_3 + 2, inst);

		filler.push_back({const_label, label});
	}


	if (type != "IDENTIFIER" && name == "for_stmt")
	{
					
		/*

			for_stmt: KW_for for_expr DLM_COLON block
				| KW_for for_expr DLM_COLON block else_block

		*/

		code inst;
		int loc = -1;
		int loc_3 = -1;
		string label = newLabel();
		string const_label = label;
		for (int i = 0; i < threeAC.size(); i++)
		{
			if (threeAC[i].label == children[3]->first)
			{
				loc = i;
			}

			if (threeAC[i].label == children[3]->last)
			{
				loc_3 = i;
			}
		}

		assert (loc != -1);
		assert (loc_3 != -1);

		inst.field_1 = "if_false";
		inst.field_2 = children[1] -> tmp;
		inst.field_3 = "goto";
		inst.field_4 = "@FILL_LATER@";
		inst.label = const_label;
		threeAC.insert(threeAC.begin() + loc + 1, inst);

		label = newLabel();

		inst.field_1 = "goto";
		inst.field_2 = "@FILL_LATER@";
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = label;
		threeAC.insert(threeAC.begin() + loc_3 + 2, inst); // +2 due to an instruction added 

		filler.push_back({const_label, label});
		filler.push_back({label, children[1] -> first});

		first = children[1] -> first;
	}

	if (type != "IDENTIFIER" && name == "for_expr")
	{
					
		/*

			for_expr: l_primary KW_in expression

		*/
		

		string t1 = newTmp();
		string t2 = newTmp();
		string t3 = newTmp();
		string t4 = newTmp();
		string t5 = newTmp();

		code inst;
		inst.field_1 = t3;
		inst.field_2 = "=";
		inst.field_3 = "*(" + children[2] -> tmp + ")";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = t4;
		inst.field_2 = "=";
		inst.field_3 = t3;
		inst.field_4 = "+";
		inst.field_5 = "1";
		inst.label = newLabel();
		threeAC.push_back(inst);

		string lab = newLabel();
		inst.field_1 = t1;
		inst.field_2 = "=";
		inst.field_3 = "1";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = lab;
		threeAC.push_back(inst);


		inst.field_1 = t2;
		inst.field_2 = "=";
		inst.field_3 = t1;
		inst.field_4 = "*";
		inst.field_5 = "8";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = t5;
		inst.field_2 = "=";
		inst.field_3 = children[2] -> tmp;
		inst.field_4 = "+";
		inst.field_5 = t2;
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = children[0] -> tmp;
		inst.field_2 = "=";
		inst.field_3 = "*(" + t5 + ")";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		inst.field_1 = t1;
		inst.field_2 = "=";
		inst.field_3 = t1;
		inst.field_4 = "+";
		inst.field_5 = "1";
		inst.label = newLabel();
		threeAC.push_back(inst);

		tmp = newTmp();
		inst.field_1 = tmp;
		inst.field_2 = "=";
		inst.field_3 = t1;
		inst.field_4 = "<";
		inst.field_5 = t4;
		inst.label = newLabel();
		threeAC.push_back(inst);

		first = lab;

	}


	if (type != "IDENTIFIER" && name == "expressions")
	{
					
		/*

			expressions: expression DLM_COMMA expressions
				| expression DLM_COMMA
				| expression 

		*/


		tempExprs.insert(tempExprs.begin(), children[0] -> tmp);




		/* nothing to do */

	}


	if (type != "IDENTIFIER" && name == "expression")
	{
					
		/*
			expression: disjunction             
		*/



		tmp = children[0] -> tmp;

	}


	if (type != "IDENTIFIER" && name == "disjunction")
	{
					
		/*

			disjunction: conjunction
				| disjunction OP_LOG_OR conjunction

		*/

		if (children.size() == 1)
		{
			tmp = children[0] -> tmp; 
		}

		if (children.size() == 3)
		{
			tmp = newTmp();
			code inst;
			inst.field_1 = tmp;
			inst.field_2 = "=";
			inst.field_3 = children[0] -> tmp;
			inst.field_4 = children[1] -> name;
			inst.field_5 = children[2] -> tmp;
			inst.label = newLabel();
			threeAC.push_back(inst);
		}

	}


	if (type != "IDENTIFIER" && name == "conjunction")
	{
					
		/*

			conjunction: inversion
				| conjunction OP_LOG_AND inversion

		*/


		if (children.size() == 1)
		{
			tmp = children[0] -> tmp; 
		}

		if (children.size() == 3)
		{
			tmp = newTmp();
			code inst;
			inst.field_1 = tmp;
			inst.field_2 = "=";
			inst.field_3 = children[0] -> tmp;
			inst.field_4 = children[1] -> name;
			inst.field_5 = children[2] -> tmp;
			inst.label = newLabel();
			threeAC.push_back(inst);
		}

	}


	if (type != "IDENTIFIER" && name == "inversion")
	{
					
		/*

			inversion: OP_LOG_NOT inversion 
				| comparison

		*/

		if (children.size() == 1)
		{
			tmp = children[0] -> tmp; 
		}

		if (children.size() == 2)
		{
			tmp = newTmp();
			code inst;
			inst.field_1 = tmp;
			inst.field_2 = "=";
			inst.field_3 = children[0] -> name;
			inst.field_4 = children[1] -> tmp;
			inst.label = newLabel();
			threeAC.push_back(inst);
		}

	}


	if (type != "IDENTIFIER" && name == "comparison")
	{
					
		/*

			comparison: bitwise_or
				| comparison bitwise_operator bitwise_or

		*/

		if (children.size() == 1)
		{
			tmp = children[0] -> tmp; 
		}

		else if (children.size() == 3 && children[1] -> name == "in")
		{
			tmp = newTmp();
			code inst;

			inst.field_1 = tmp;
			inst.field_2 = "=";
			inst.field_3 = "0";
			inst.field_4 = ""; 
			inst.field_5 = "";
			inst.label = newLabel();
			threeAC.push_back(inst);

			string t1 = newTmp();
			inst.field_1 = t1;
			inst.field_2 = "=";
			inst.field_3 = "*(" + children[2] -> tmp + ")";
			inst.field_4 = "";
			inst.field_5 = "";
			inst.label = newLabel();
			threeAC.push_back(inst);

			string t2 = newTmp();
			inst.field_1 = t2;
			inst.field_2 = "=";
			inst.field_3 = "1";
			inst.field_4 = "";
			inst.field_5 = "";
			inst.label = newLabel();
			threeAC.push_back(inst);

			string label = newLabel();
			string t3 = newTmp();
			inst.field_1 = t3;
			inst.field_2 = "=";
			inst.field_3 = t2;
			inst.field_4 = "*";
			inst.field_5 = "8";
			inst.label = label;
			threeAC.push_back(inst);

			string t4 = newTmp();
			inst.field_1 = t4;
			inst.field_2 = "=";
			inst.field_3 = children[2] -> tmp;
			inst.field_4 = "+";
			inst.field_5 = t3;
			inst.label = newLabel();
			threeAC.push_back(inst);

			string t5 = newTmp();
			inst.field_1 = t5;
			inst.field_2 = "=";
			inst.field_3 = "*(" + t4 + ")";
			inst.field_4 = ""; 
			inst.field_5 = "";
			inst.label = newLabel();
			threeAC.push_back(inst);

			inst.field_1 = tmp;
			inst.field_2 = "=";
			inst.field_3 = "(" + children[0] -> tmp;
			inst.field_4 = "=="; 
			inst.field_5 = t5 + ")";
			inst.label = newLabel();
			threeAC.push_back(inst);

			string label2 = newLabel();
			inst.field_1 = "if";
			inst.field_2 = tmp;
			inst.field_3 = "goto";
			inst.field_4 = "@FILL_LATER@"; 
			inst.field_5 = "";
			inst.label = label2;
			threeAC.push_back(inst);

			inst.field_1 = t2;
			inst.field_2 = "=";
			inst.field_3 = t2;
			inst.field_4 = "+";
			inst.field_5 = "1";
			inst.label = newLabel();
			threeAC.push_back(inst);

			string t7 = newTmp();
			inst.field_1 = t7;
			inst.field_2 = "=";
			inst.field_3 = t2;
			inst.field_4 = "<"; 
			inst.field_5 = t1;
			inst.label = newLabel();
			threeAC.push_back(inst);

			string label3 = newLabel();
			inst.field_1 = "if";
			inst.field_2 = t7;
			inst.field_3 = "goto";
			inst.field_4 = label;
			inst.field_5 = "";
			inst.label = label3;
			threeAC.push_back(inst);

			filler.push_back({label2, label3});
		}

		else if (children.size() == 3)
		{
			tmp = newTmp();
			code inst;
			inst.field_1 = tmp;
			inst.field_2 = "=";
			inst.field_3 = children[0] -> tmp;
			inst.field_4 = children[1] -> name;
			inst.field_5 = children[2] -> tmp;
			inst.label = newLabel();
			threeAC.push_back(inst);
		}

	}


	if (type != "IDENTIFIER" && name == "bitwise_operator")
	{
					
		/*

			bitwise_operator: OP_REL_EQ
				| OP_REL_NEQ
				| OP_REL_LTE
				| OP_REL_LT
				| OP_REL_GTE
				| OP_REL_GT
				| KW_in
				| KW_is

		*/

		

		name = children[0] -> name;

	}


	if (type != "IDENTIFIER" && name == "bitwise_or")
	{
					
		/*

			bitwise_or: bitwise_or OP_BIT_OR bitwise_xor 
				| bitwise_xor

		*/

		

		if (children.size() == 1)
		{
			tmp = children[0] -> tmp; 
		}

		if (children.size() == 3)
		{
			tmp = newTmp();
			code inst;
			inst.field_1 = tmp;
			inst.field_2 = "=";
			inst.field_3 = children[0] -> tmp;
			inst.field_4 = children[1] -> name;
			inst.field_5 = children[2] -> tmp;
			inst.label = newLabel();
			threeAC.push_back(inst);
		}

	}


	if (type != "IDENTIFIER" && name == "bitwise_xor")
	{
					
		/*

			bitwise_xor: bitwise_xor OP_BIT_XOR bitwise_and 
				| bitwise_and

		*/

		
		if (children.size() == 1)
		{
			tmp = children[0] -> tmp; 
		}

		if (children.size() == 3)
		{
			tmp = newTmp();
			code inst;
			inst.field_1 = tmp;
			inst.field_2 = "=";
			inst.field_3 = children[0] -> tmp;
			inst.field_4 = children[1] -> name;
			inst.field_5 = children[2] -> tmp;
			inst.label = newLabel();
			threeAC.push_back(inst);
		}

	}


	if (type != "IDENTIFIER" && name == "bitwise_and")
	{
					
		/*

			bitwise_and: bitwise_and OP_BIT_AND shift_expr 
				| shift_expr

		*/

		

		if (children.size() == 1)
		{
			tmp = children[0] -> tmp; 
		}

		if (children.size() == 3)
		{
			tmp = newTmp();
			code inst;
			inst.field_1 = tmp;
			inst.field_2 = "=";
			inst.field_3 = children[0] -> tmp;
			inst.field_4 = children[1] -> name;
			inst.field_5 = children[2] -> tmp;
			inst.label = newLabel();
			threeAC.push_back(inst);
		}

	}


	if (type != "IDENTIFIER" && name == "shift_expr")
	{
					
		/*

			shift_expr: shift_expr OP_BIT_LS sum 
				| shift_expr OP_BIT_RS sum 
				| sum

		*/

		

		if (children.size() == 1)
		{
			tmp = children[0] -> tmp; 
		}

		if (children.size() == 3)
		{
			tmp = newTmp();
			code inst;
			inst.field_1 = tmp;
			inst.field_2 = "=";
			inst.field_3 = children[0] -> tmp;
			inst.field_4 = children[1] -> name;
			inst.field_5 = children[2] -> tmp;
			inst.label = newLabel();
			threeAC.push_back(inst);
		}

	}


	if (type != "IDENTIFIER" && name == "sum")
	{
					
		/*

			sum: sum OP_ATH_ADD term 
				| sum OP_ATH_SUB term 
				| term

		*/

		

		if (children.size() == 1)
		{
			tmp = children[0] -> tmp; 
		}

		if (children.size() == 3)
		{
			tmp = newTmp();
			code inst;
			inst.field_1 = tmp;
			inst.field_2 = "=";
			inst.field_3 = children[0] -> tmp;
			inst.field_4 = children[1] -> name;
			inst.field_5 = children[2] -> tmp;
			inst.label = newLabel();
			threeAC.push_back(inst);
		}
	}


	if (type != "IDENTIFIER" && name == "term")
	{
					
		/*

			term: term OP_ATH_MUL factor 
				| term OP_ATH_DIV factor 
				| term OP_ATH_FDIV factor 
				| term OP_ATH_MOD factor 
				| factor

		*/

		

		if (children.size() == 1)
		{
			tmp = children[0] -> tmp; 
		}

		if (children.size() == 3)
		{
			tmp = newTmp();
			code inst;
			inst.field_1 = tmp;
			inst.field_2 = "=";
			inst.field_3 = children[0] -> tmp;
			inst.field_4 = children[1] -> name;
			inst.field_5 = children[2] -> tmp;
			inst.label = newLabel();
			threeAC.push_back(inst);
		}

	}


	if (type != "IDENTIFIER" && name == "factor")
	{
					
		/*

			factor: OP_ATH_ADD factor 
				| OP_ATH_SUB factor 
				| OP_BIT_NEG factor 
				| power

		*/

		if (children.size() == 1)
		{
			tmp = children[0] -> tmp; 
		}

		if (children.size() == 2)
		{
			tmp = newTmp();
			code inst;
			inst.field_1 = tmp;
			inst.field_2 = "=";
			inst.field_3 = children[0] -> name;
			inst.field_4 = children[1] -> tmp;
			inst.label = newLabel();
			threeAC.push_back(inst);
		}

	}


	if (type != "IDENTIFIER" && name == "power")
	{
					
		/*

			power: primary OP_ATH_POW factor 
				| primary

		*/


		if (children.size() == 1)
		{
			tmp = children[0] -> tmp; 
		}

		if (children.size() == 3)
		{
			tmp = newTmp();
			code inst;
			inst.field_1 = tmp;
			inst.field_2 = "=";
			inst.field_3 = children[0] -> tmp;
			inst.field_4 = "";
			inst.field_5 = "";
			inst.label = newLabel();
			threeAC.push_back(inst);

			string tmp1 = newTmp();
			inst.field_1 = tmp1;
			inst.field_2 = "=";
			inst.field_3 = children[2] -> tmp;
			inst.field_4 = "";
			inst.field_5 = "";
			inst.label = newLabel();
			threeAC.push_back(inst);

			string label = newLabel();
			string tmp2 = newTmp();
			inst.field_1 = tmp2;
			inst.field_2 = "=";
			inst.field_3 = tmp1;
			inst.field_4 = ">";
			inst.field_5 = "0";
			inst.label = label;
			threeAC.push_back(inst);

			string label1 = newLabel();
			inst.field_1 = "if_false";
			inst.field_2 = tmp2;
			inst.field_3 = "goto";
			inst.field_4 = "@FILL_LATER@";			// fill
			inst.field_5 = "";
			inst.label = label1;
			threeAC.push_back(inst);

			inst.field_1 = tmp;
			inst.field_2 = "=";
			inst.field_3 = tmp;
			inst.field_4 = "*";				
			inst.field_5 = children[0] -> tmp;
			inst.label = newLabel();
			threeAC.push_back(inst);

			inst.field_1 = tmp1;
			inst.field_2 = "=";
			inst.field_3 = tmp1;
			inst.field_4 = "-";				
			inst.field_5 = "1";
			inst.label = newLabel();
			threeAC.push_back(inst);


			string label3 = newLabel();
			inst.field_1 = "goto";
			inst.field_2 = label;
			inst.field_3 = "";
			inst.field_4 = "";				
			inst.field_5 = "";
			inst.label = label3;
			threeAC.push_back(inst);

			filler.push_back({label1, label3});
		}

	}


	if (type != "IDENTIFIER" && name == "function_call")
	{
					
		/*

			function_call: NAME DLM_LFT_PRN args DLM_RGT_PRN

		*/

		int nparams = tempExprs.size();
		vector<tableRecord*> params;
		code inst;
		tableRecord* funcEntry = NULL;

		tableRecord* entry = globTable -> lookup_table(children[0] -> name);

		if (entry && (entry -> recordType == recordType::TYPE_CLASS))
		{
			// this is the constructor
			int width = entry -> size;
			allocate_mem(to_string(width));
			tmp = MemRg;
		}

		for(int i = nparams -1; i >= 0; i--)
		{
			TreeNode* node = ((host -> children)[2] -> children)[i];
			tableRecord* record = new tableRecord(node -> name, node -> dataType);
			params.insert(params.begin(), record);

			inst.field_1 = "param";
			inst.field_2 = mangle(tempExprs[i]);
			inst.label = newLabel();
			threeAC.push_back(inst);
		}

		tempExprs.clear();


		if (entry && (entry -> recordType == recordType::TYPE_CLASS))
		{
			// this is the constructor
			inst.field_1 = "param";
			inst.field_2 = tmp;
			inst.label = newLabel();
			threeAC.push_back(inst);
			nparams ++;
			funcEntry = entry -> symTab -> lookup_table("__init__");
		}

		else {

			if (dotRecord)
			{
				params.insert(params.begin(), dotRecord);
				inst.field_1 = "param";
				inst.field_2 = mangle(dotRecord_1 -> name);
				inst.label = newLabel();
				threeAC.push_back(inst);
				nparams ++;
			}

			if (dotTable)
			{
				funcEntry = dotTable -> lookup(children[0] -> name, recordType::TYPE_FUNCTION, &params);
			}

			else {
				funcEntry = table -> lookup(children[0] -> name, recordType::TYPE_FUNCTION, &params);
			}

		}

		assert (funcEntry);
		int local_size = funcEntry -> symTab -> size;

		if(local_size)
		{
			inst.field_1 = "shift_sp";
			inst.field_2 = to_string(local_size * -1);
			inst.label = newLabel();
			threeAC.push_back(inst);
		}

		assert (funcEntry -> symTab -> parentSymtable);
		string funcName = tableHash(funcEntry -> symTab -> parentSymtable);

		if (dotTable)
		{
			funcName = tableHash(dotTable);
		}

		if (funcName.length()) funcName += "_Cc";
		funcName += funcEntry -> name;

		symbolTable* funcTable = funcEntry -> symTab;
		for (int i = 0; i < nparams; i++)
		{
			funcName += "_Zz" + (funcTable -> entries)[i] -> type;
		}
		funcName += "_Nn" + to_string(nparams);

		if(funcTable -> name == "main") funcName = "main";


		inst.field_1 = "call";
		inst.field_2 = funcName;
		inst.field_3 = to_string(nparams);
		inst.label = newLabel();
		threeAC.push_back(inst);

		if(local_size)
		{
			inst.field_1 = "shift_sp";
			inst.field_2 = to_string(local_size);
			inst.field_3 = "";
			inst.label = newLabel();
			threeAC.push_back(inst);
		}

		if (funcEntry -> type != "None" && funcEntry -> type != "")
		{
			tmp = newTmp();
			inst.field_1 = tmp;
			inst.field_2 = "=";
			inst.field_3 = "popparam";
			inst.label = newLabel();
			threeAC.push_back(inst);
		}

		dotTable = NULL;
		dotRecord = NULL;
		dotRecord_1 = NULL;

	}


	if (type != "IDENTIFIER" && name == "args")
	{
					
		/*

			args:
				| expressions

		*/

		if (children.size() == 1)
		{
			tmp = children[0] -> tmp;
		}

		/* do nothing */
	}


	if (type != "IDENTIFIER" && name == "list_access")
	{
					
		/*

			list_access: NAME DLM_LFT_SQ expression DLM_RGT_SQ

		*/

		if (children[0] -> name != "list")
		{
			string type = (host -> dataType);
			tableRecord* entry = globTable -> lookup_table(type, recordType::TYPE_CLASS);
			assert(entry);
			int size = entry -> size;
			
			string tmpry = newTmp();
			code inst;
			inst.field_1 = tmpry;
			inst.field_2 = "=";
			inst.field_3 = to_string(size);
			inst.field_4 = "*";
			inst.field_5 = children[2] -> tmp;
			inst.label = newLabel();
			threeAC.push_back(inst);

			inst.field_1 = tmpry;
			inst.field_2 = "=";
			inst.field_3 = tmpry;
			inst.field_4 = "+";
			inst.field_5 = "8";
			inst.label = newLabel();
			threeAC.push_back(inst);

			string t1 = newTmp();
			inst.field_1 = t1;
			inst.field_2 = "=";
			inst.field_3 = mangle(children[0] -> name);
			inst.field_4 = "+";
			inst.field_5 = tmpry;
			inst.label = newLabel();
			threeAC.push_back(inst);

			tmp = newTmp();
			inst.field_1 = tmp;
			inst.field_2 = "=";
			inst.field_3 = "*(" + t1 + ")";
			inst.field_4 = "";
			inst.field_5 = "";
			inst.label = newLabel();
			threeAC.push_back(inst);
		}


	}

	if (type != "IDENTIFIER" && name == "l_primary")
	{
					
		/*

			l_primary:  NAME DLM_DOT NAME 
				| list_access 
				| atom

		*/

		

		if (children.size() == 3)
		{
			tableRecord* entry = table -> lookup(children[0] -> name);
			assert (entry);
			entry = globTable -> lookup (entry -> type , recordType::TYPE_CLASS);
			assert (entry);
			symTable* tmpTable = entry -> symTab;
			entry = tmpTable -> lookup_table(children[2] -> name);
			assert (entry);

			code inst;
			string t1 = newTmp();
			inst.field_1 = t1;
			inst.field_2 = "=";
			inst.field_3 = mangle(children[0] -> name);
			inst.field_4 = "";
			inst.field_5 = "";
			inst.label = newLabel();
			threeAC.push_back(inst);

			children[0] -> tmp = t1;

			string t2 = newTmp();
			inst.field_1 = t2;
			inst.field_2 = "=";
			inst.field_3 = children[0] -> tmp;
			inst.field_4 = "+";
			inst.field_5 = to_string(entry -> offset);
			inst.label = newLabel();
			threeAC.push_back(inst);

			tmp = "*(" + t2 + ")";
		}


		else
		{
			tmp = children[0] -> tmp;
		}

	}

	if (type != "IDENTIFIER" && name == "primary")
	{
					
		/*

			primary: l_primary
				| NAME DLM_DOT function_call
				| function_call

		*/


		if (children.size() == 1)
		{
			tmp = children[0]->tmp;
		}

		if (children.size() == 3)
		{
			tmp = children[2] -> tmp;
			dotTable = NULL;
			dotRecord = NULL;
			dotRecord_1 = NULL;
		}

	}


	if (type != "IDENTIFIER" && name == "atom")
	{
					
		/*

			atom: NAME
				| KW_True 
				| KW_False 
				| KW_None 
				| FLOAT_LITERAL
				| INT_LITERAL
				| strings
				| group
				| list

		*/

		if (children[0]->name == "strings")
		{
			allocate_mem(to_string((children[0] -> tmp).length() - 1 + 8));
			tmp = MemRg;
			code inst;

			inst.field_1 = "*(" + tmp + ")";
			inst.field_2 = "=";
			inst.field_3 = to_string((children[0] -> tmp).length() - 1);
			inst.label = newLabel();
			threeAC.push_back(inst);

			inst.field_1 = tmp;
			inst.field_2 = "=";
			inst.field_3 = tmp;
			inst.field_4 = "+";
			inst.field_5 = "8";
			inst.label = newLabel();
			threeAC.push_back(inst);

			string a1 = newTmp();
			inst.field_1 = a1;
			inst.field_2 = "=";
			inst.field_3 = "findAddress";
			inst.field_4 = stringMap[children[0] -> tmp];
			inst.field_5 = "";
			inst.label = newLabel();
			threeAC.push_back(inst);

			inst.field_1 = "param";
			inst.field_2 = tmp;
			inst.field_3 = "";
			inst.field_4 = "";
			inst.field_5 = "";
			inst.label = newLabel();
			threeAC.push_back(inst);

			inst.field_1 = "param";
			inst.field_2 = a1;
			inst.field_3 = "";
			inst.field_4 = "";
			inst.field_5 = "";
			inst.label = newLabel();
			threeAC.push_back(inst);

			inst.field_1 = "call";
			inst.field_2 = "strcpy";
			inst.field_3 = "";
			inst.field_4 = "";
			inst.field_5 = "";
			inst.label = newLabel();
			threeAC.push_back(inst);

		}

		else if (children[0]->type == "KEYWORD")
		{
			if (children[0]->name == "True")
			{
				tmp = "1";
			}

			if (children[0]->name == "False")
			{
				tmp = "0";
			}
		}

		else if (children[0]->type == "IDENTIFIER")
		{
			tableRecord* entry = table -> lookup(children[0] -> name);
			assert (entry);
			if (entry -> name  == entry -> type)
			{
				tmp = children[0] -> name;
			}
			else tmp = mangle(children[0] -> name);
		}

		else if (children[0]->type != "NON_TERMINAL")
		{
			tmp = children[0] -> name;
		}

		else
		{
			tmp = children[0] -> tmp;
		}

	}


	if (type != "IDENTIFIER" && name == "group")
	{
					
		/*

			group: DLM_LFT_PRN expression DLM_RGT_PRN 

		*/

		tmp = children[1] -> tmp;

	}


	if (type != "IDENTIFIER" && name == "string")
	{
					
		/*
			string: STRING_LITERAL              
		*/


		tmp = children[0] -> name;



	}


	if (type != "IDENTIFIER" && name == "strings")
	{
					
		/*

			strings: strings string
				| string

		*/


		if (children.size() == 2)
		{
			formatString(children[0]->tmp);
			formatString(children[1]->tmp);
			tmp = "\"" + children[0]->tmp + children[1]->tmp + "\\0\"";
		}
		else
		{
			formatString(children[0]->tmp);
			tmp = "\"" + children[0]->tmp + "\"";
		}

	}

	if (type != "IDENTIFIER" && name == "list")
	{
					
		/*

			list: DLM_LFT_SQ expressions DLM_RGT_SQ 

		*/

		string type = (host -> dataType).substr(5, (host -> dataType).length() - 6);
		tableRecord* entry = globTable -> lookup_table(type, recordType::TYPE_CLASS);
		assert(entry);
		int size = tempExprs.size() * (entry -> size) + 8;
		allocate = false;
		allocate_mem(to_string(size));
		tmp = MemRg;

		code inst;
		string location = newTmp();

		inst.field_1 = "*(" + tmp + ")";
		inst.field_2 = "=";
		inst.field_3 = to_string(tempExprs.size());
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();
		threeAC.push_back(inst);

		for (int i=0; i<tempExprs.size(); i++)
		{
			inst.field_1 = location;
			inst.field_2 = "=";
			inst.field_3 = tmp;
			inst.field_4 = "+";
			inst.field_5 = to_string((i+1) * (entry -> size));
			inst.label = newLabel();
			threeAC.push_back(inst);

			inst.field_1 = "*(" + location + ")";
			inst.field_2 = "=";
			inst.field_3 = tempExprs[i];
			inst.field_4 = "";
			inst.field_5 = "";
			inst.label = newLabel();
			threeAC.push_back(inst);
		}

		tempExprs.clear();

	}

	if (type == "NON_TERMINAL")
		if (threeAC.size())
			last = threeAC[threeAC.size() - 1].label;
}

/* 

NON TERMINALS THAT GENERATE CODE IN DFS IN THE HOOKS MUST SET THE CURRENT OF THE FIRST NON TERMINAL CHILD AS A NEW VALUE.
OTHERWISE REASSIGNING OLD VALUE IS OKAY.

NEW VALUE IS NOW BEING USED IN ALL CASES, HOPING TO REWRITE ALL VALUES AFTER THE DFS

*/


void fillCode()
{
	for (int index = 0; index < filler.size(); index++)
	{

		int entry1 = -1, entry2 = -1;
		for (int i = 0; i < threeAC.size(); i++)
		{
			if(entry1 < 0 && threeAC[i].label == filler[index].first)
			{
				entry1 = i;
			}
			if(entry2 < 0 && threeAC[i].label == filler[index].second)
			{
				entry2 = i + 1;
			}
		}

		assert(entry1 >= 0);
		assert(entry2 >= 0);

		if	(entry2 == threeAC.size())
		{
			if (threeAC[entry1].field_4 == "@FILL_LATER@")
			{
				threeAC[entry1].field_4 = "END";
			}
			else if (threeAC[entry1].field_2 == "@FILL_LATER@")
			{
				threeAC[entry1].field_2 = "END";
			}
		}
		else
		{
			if (threeAC[entry1].field_4 == "@FILL_LATER@")
			{
				threeAC[entry1].field_4 = threeAC[entry2].label;
			}
			else if (threeAC[entry1].field_2 == "@FILL_LATER@")
			{
				threeAC[entry1].field_2 = threeAC[entry2].label;
			}
		}
	}

	return;
}

void Parasite::genCode()
{
	map<string, string> labelMap;

	fill_stringMap();
	genAC();
	formFirstLast();
	fillCode();

	for (int i=0; i<threeAC.size(); i++)
	{
		labelMap[threeAC[i].label] = to_string(i + 1);
	}

	for (int i=0; i<threeAC.size(); i++)
	{

		threeAC[i].label = labelMap[threeAC[i].label] + ":";

		if (labelMap.find(threeAC[i].field_1) != labelMap.end())
		{
			threeAC[i].field_1 = labelMap[threeAC[i].field_1];
		}

		if (labelMap.find(threeAC[i].field_2) != labelMap.end())
		{
			threeAC[i].field_2 = labelMap[threeAC[i].field_2];
		}

		if (labelMap.find(threeAC[i].field_3) != labelMap.end())
		{
			threeAC[i].field_3 = labelMap[threeAC[i].field_3];
		}

		if (labelMap.find(threeAC[i].field_4) != labelMap.end())
		{
			threeAC[i].field_4 = labelMap[threeAC[i].field_4];
		}

		if (labelMap.find(threeAC[i].field_5) != labelMap.end())
		{
			threeAC[i].field_5 = labelMap[threeAC[i].field_5];
		}
	}

	code inst;
	inst.label = "END";
	threeAC.push_back(inst);
}
