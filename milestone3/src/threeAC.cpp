#include "include/code.hpp"

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
vector<code> class_copy;
map <string, string> Temporaries;
string MemRg = "";
bool allocate = false;
vector<string> tempExprs;
string isReturn = "";
vector<pair<string, string>> filler;
map<string, string> tempType;
map<string, tableRecord*> Temp_to_record;
extern map<symbolTable*, int> visited;
int stringMapSize = 0;
extern map<string, string> copyMap;

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
	if (curr -> parentSymtable && curr -> parentSymtable -> tableType == tableType::CLASS && curr -> tableType != tableType::CLASS)
		name = curr -> parentSymtable -> name + "_Cc" + name;
	// while(curr -> parentSymtable && curr -> parentSymtable -> name != "__GLOBAL__")
	// {
	// 	curr = curr -> parentSymtable;
	// 	name = curr -> name + "_Cc" + name;
	// }
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
		
		init_tac();
	   
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

		vector<tableRecord*> params;

		for (int i = ((children[3]->host) -> children).size() - 1; i >= 0; i--)
		{
			TreeNode* node = ((children[3]->host) -> children)[i];
			tableRecord* record = new tableRecord(node -> name, node -> dataType);
			params.insert(params.begin(), record);
		}

		tableRecord* entry = table -> lookup(children[1] -> name, recordType::TYPE_FUNCTION, &params);
		assert (entry);
		assert (entry -> recordType == recordType::TYPE_FUNCTION);
		assert (entry -> symTab);
		table = entry -> symTab;

		for (int i = 0; i < table -> numParams; i++)
		{
			funcName += "_Zz" + (table -> entries)[i] -> type;
		}

		funcName += "_Nn" + to_string(table -> numParams);

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

			if (tempType[children[0] -> tmp] == "bool" && tempType[children[2] -> tmp] != "bool")
			{
				code inst;
				inst.field_1 = children[0] -> tmp;
				inst.field_2 = "=";
				inst.field_3 = children[2] -> tmp;
				inst.field_4 = "!=";
				inst.field_5 = "0";
				inst.label = newLabel();
				threeAC.push_back(inst);
			}
			else
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
		}

		if (children[0] -> name == "l_primary" && children.size() == 3)
		{
			int operation_len = (children[1] -> name).length();
			string operation = (children[1] -> name).substr(0, operation_len - 1); 

			if (tempType[children[0] -> tmp] == "bool")
			{
				if (operation == "**")
				{
					string tmp = newTmp();
					tempType[tmp] = "bool";
					code inst;
					inst.field_1 = tmp;
					inst.field_2 = "=";
					inst.field_3 = children[2] -> tmp;
					inst.field_4 = "==";
					inst.field_5 = "0";
					inst.label = newLabel();
					threeAC.push_back(inst);

					string tmp1 = newTmp();
					tempType[tmp1] = "bool";
					inst.field_1 = tmp1;
					inst.field_2 = "=";
					inst.field_3 = children[0] -> tmp;
					inst.field_4 = "!=";
					inst.field_5 = "0";
					inst.label = newLabel();
					threeAC.push_back(inst);

					string tmp2 = newTmp();
					tempType[tmp2] = "bool";
					inst.field_1 = children[0]->tmp;
					inst.field_2 = "=";
					inst.field_3 = tmp1;
					inst.field_4 = "|";
					inst.field_5 = tmp;
					inst.label = newLabel();
					threeAC.push_back(inst);
				}
				else
				{

					string tmp = newTmp();
					tempType[tmp] = "bool";

					code inst;
					inst.field_1 = tmp;
					inst.field_2 = "=";
					inst.field_3 = children[0] -> tmp;
					inst.field_4 = operation;
					inst.field_5 = children[2] -> tmp;
					inst.label = newLabel();
					threeAC.push_back(inst);


					inst.field_1 = children[0] -> tmp;
					inst.field_2 = "=";
					inst.field_3 = tmp;
					inst.field_4 = "!=";
					inst.field_5 = "0";
					inst.label = newLabel();
					threeAC.push_back(inst);
				}
			}
			else
			{
				if (operation == "**")
				{
					string tmp = newTmp();
					tempType[tmp] = "int";
					code inst;
					inst.field_1 = tmp;
					inst.field_2 = "=";
					inst.field_3 = children[0] -> tmp;
					inst.field_4 = "";
					inst.field_5 = "";
					inst.label = newLabel();
					threeAC.push_back(inst);

					string tmp1 = newTmp();
					tempType[tmp1] = "int";
					inst.field_1 = tmp1;
					inst.field_2 = "=";
					inst.field_3 = children[2] -> tmp;
					inst.field_4 = "";
					inst.field_5 = "";
					inst.label = newLabel();
					threeAC.push_back(inst);

					string label = newLabel();
					string tmp2 = newTmp();
					tempType[tmp2] = "bool";
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

					
					string label4 = newLabel();
					inst.field_1 = children[0] -> tmp;
					inst.field_2 = "=";
					inst.field_3 = tmp;
					inst.field_4 = "";				
					inst.field_5 = "";
					inst.label = label4;
					threeAC.push_back(inst);

					filler.push_back({label1, label3});
				}
				else
				{
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

		}

	}


	if (type != "IDENTIFIER" && name == "multi_targets_assgn")
	{
					
		/*

			multi_targets_assgn: l_primary OP_ASN_ASN expression
				| l_primary OP_ASN_ASN multi_targets_assgn

		*/


		if (tempType[children[0] -> tmp] == "bool" && tempType[children[2] -> tmp] != "bool")
		{
			code inst;
			inst.field_1 = children[0] -> tmp;
			inst.field_2 = "=";
			inst.field_3 = children[2] -> tmp;
			inst.field_4 = "!=";
			inst.field_5 = "0";
			inst.label = newLabel();
			threeAC.push_back(inst);
		}
		else
		{
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
		tempType[t1] = "int";
		string t2 = newTmp();
		tempType[t2] = tempType[t1];
		string t3 = newTmp();
		tempType[t3] = tempType[t1];
		string t4 = newTmp();
		tempType[t4] = tempType[t1];
		string t5 = newTmp();
		tempType[t5] = tempType[children[2] -> tmp];
		string t6 = newTmp();
		tempType[t6] = "int";

		code inst;
		inst.field_1 = t3;
		inst.field_2 = "=";
		inst.field_3 = "*(" + children[2] -> tmp + ")";
		inst.label = newLabel();
		threeAC.push_back(inst);

		// inst.field_1 = t4;
		// inst.field_2 = "=";
		// inst.field_3 = t3;
		// inst.field_4 = "";
		// inst.field_5 = "";
		// inst.label = newLabel();
		// threeAC.push_back(inst);

		string lab = newLabel();
		inst.field_1 = t1;
		inst.field_2 = "=";
		inst.field_3 = "1";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = lab;
		threeAC.push_back(inst);

		inst.field_1 = t6;
		inst.field_2 = "=";
		inst.field_3 = t1;
		inst.field_4 = "-";
		inst.field_5 = "1";
		inst.label = newLabel();
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
		tempType[tmp] = "bool";
		inst.field_1 = tmp;
		inst.field_2 = "=";
		inst.field_3 = t6;
		inst.field_4 = "<";
		inst.field_5 = t3;
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
			tempType[tmp] = "bool";
			string tmp1 = children[0] -> tmp;
			string tmp2 = children[2] -> tmp;
			if (tempType[children[0]->tmp] == "int")
			{
				tmp1 = newTmp();
				tempType[tmp1] = "bool";
				code inst;
				inst.field_1 = tmp1;
				inst.field_2 = "=";
				inst.field_3 = children[0] -> tmp;
				inst.field_4 = "!=";
				inst.field_5 = "0";
				inst.label = newLabel();
				threeAC.push_back(inst);
			}
			if (tempType[children[2]->tmp] == "int")
			{
				tmp2 = newTmp();
				tempType[tmp2] = "bool";
				code inst;
				inst.field_1 = tmp2;
				inst.field_2 = "=";
				inst.field_3 = children[2] -> tmp;
				inst.field_4 = "!=";
				inst.field_5 = "0";
				inst.label = newLabel();
				threeAC.push_back(inst);
			}
			if (tempType[children[0]->tmp] == "str")
			{
				tmp1 = newTmp();
				tempType[tmp1] = "bool";
				code inst;
				inst.field_1 = tmp1;
				inst.field_2 = "=";
				inst.field_3 = "*(" + children[0] -> tmp + ")";
				inst.field_4 = "!=";
				inst.field_5 = "0";
				inst.label = newLabel();
				threeAC.push_back(inst);
			}
			if (tempType[children[2]->tmp] == "str")
			{
				tmp2 = newTmp();
				tempType[tmp2] = "bool";
				code inst;
				inst.field_1 = tmp2;
				inst.field_2 = "=";
				inst.field_3 = "*(" + children[2] -> tmp + ")";
				inst.field_4 = "!=";
				inst.field_5 = "0";
				inst.label = newLabel();
				threeAC.push_back(inst);
			}
			code inst;
			inst.field_1 = tmp;
			inst.field_2 = "=";
			inst.field_3 = tmp1;
			inst.field_4 = children[1] -> name;
			inst.field_5 = tmp2;
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
			tempType[tmp] = "bool";
			string tmp1 = children[0] -> tmp;
			string tmp2 = children[2] -> tmp;
			if (tempType[children[0]->tmp] == "int")
			{
				tmp1 = newTmp();
				tempType[tmp1] = "bool";
				code inst;
				inst.field_1 = tmp1;
				inst.field_2 = "=";
				inst.field_3 = children[0] -> tmp;
				inst.field_4 = "!=";
				inst.field_5 = "0";
				inst.label = newLabel();
				threeAC.push_back(inst);
			}
			if (tempType[children[2]->tmp] == "int")
			{
				tmp2 = newTmp();
				tempType[tmp2] = "bool";
				code inst;
				inst.field_1 = tmp2;
				inst.field_2 = "=";
				inst.field_3 = children[2] -> tmp;
				inst.field_4 = "!=";
				inst.field_5 = "0";
				inst.label = newLabel();
				threeAC.push_back(inst);
			}
			if (tempType[children[0]->tmp] == "str")
			{
				tmp1 = newTmp();
				tempType[tmp1] = "bool";
				code inst;
				inst.field_1 = tmp1;
				inst.field_2 = "=";
				inst.field_3 = "*(" + children[0] -> tmp + ")";
				inst.field_4 = "!=";
				inst.field_5 = "0";
				inst.label = newLabel();
				threeAC.push_back(inst);
			}
			if (tempType[children[2]->tmp] == "str")
			{
				tmp2 = newTmp();
				tempType[tmp2] = "bool";
				code inst;
				inst.field_1 = tmp2;
				inst.field_2 = "=";
				inst.field_3 = "*(" + children[2] -> tmp + ")";
				inst.field_4 = "!=";
				inst.field_5 = "0";
				inst.label = newLabel();
				threeAC.push_back(inst);
			}
			code inst;
			inst.field_1 = tmp;
			inst.field_2 = "=";
			inst.field_3 = tmp1;
			inst.field_4 = children[1] -> name;
			inst.field_5 = tmp2;
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
			tempType[tmp] = "bool";
			string tmp1 = children[1] -> tmp;
			if (tempType[children[1]->tmp] == "int")
			{
				tmp1 = newTmp();
				tempType[tmp1] = "bool";
				code inst;
				inst.field_1 = tmp1;
				inst.field_2 = "=";
				inst.field_3 = children[1] -> tmp;
				inst.field_4 = "!=";
				inst.field_5 = "0";
				inst.label = newLabel();
				threeAC.push_back(inst);
			}
			if (tempType[children[0]->tmp] == "str")
			{
				tmp1 = newTmp();
				tempType[tmp1] = "bool";
				code inst;
				inst.field_1 = tmp1;
				inst.field_2 = "=";
				inst.field_3 = "*(" + children[1] -> tmp + ")";
				inst.field_4 = "!=";
				inst.field_5 = "0";
				inst.label = newLabel();
				threeAC.push_back(inst);
			}
			code inst;
			inst.field_1 = tmp;
			inst.field_2 = "=";
			inst.field_3 = children[0] -> name;
			inst.field_4 = tmp1;
			inst.field_5 = "";
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
			tempType[tmp] = "bool";
			code inst;

			inst.field_1 = tmp;
			inst.field_2 = "=";
			inst.field_3 = "0";
			inst.field_4 = ""; 
			inst.field_5 = "";
			inst.label = newLabel();
			threeAC.push_back(inst);

			string t1 = newTmp();
			tempType[t1] = tempType["*(" + children[2] -> tmp + ")"];
			inst.field_1 = t1;
			inst.field_2 = "=";
			inst.field_3 = "*(" + children[2] -> tmp + ")";
			inst.field_4 = "";
			inst.field_5 = "";
			inst.label = newLabel();
			threeAC.push_back(inst);

			string t2 = newTmp();
			tempType[t2] = "int";
			inst.field_1 = t2;
			inst.field_2 = "=";
			inst.field_3 = "1";
			inst.field_4 = "";
			inst.field_5 = "";
			inst.label = newLabel();
			threeAC.push_back(inst);

			string label = newLabel();
			string t3 = newTmp();
			tempType[t3] = tempType[t2];
			inst.field_1 = t3;
			inst.field_2 = "=";
			inst.field_3 = t2;
			inst.field_4 = "*";
			inst.field_5 = "8";
			inst.label = label;
			threeAC.push_back(inst);

			string t4 = newTmp();
			tempType[t4] = tempType[children[2] -> tmp];
			tempType["*(" + t4 + ")"] = tempType["*(" + children[2] -> tmp + ")"];
			inst.field_1 = t4;
			inst.field_2 = "=";
			inst.field_3 = children[2] -> tmp;
			inst.field_4 = "+";
			inst.field_5 = t3;
			inst.label = newLabel();
			threeAC.push_back(inst);

			string t5 = newTmp();
			tempType[t5] = tempType["*(" + t4 + ")"];
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
			tempType[t7] = "bool";
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
			tempType[tmp] = "bool";
			code inst;


			if (tempType[children[0] -> tmp] == "str" && children[1] -> type == "OP_RELATIONAL")
			{
				assert (tempType[children[2] -> tmp] == "str");

				inst.field_1 = "param";
				inst.field_2 = children[2] -> tmp;
				inst.field_3 = "";
				inst.field_4 = "";
				inst.field_5 = "";
				inst.label = newLabel();
				threeAC.push_back(inst);

				inst.field_1 = "param";
				inst.field_2 = children[0] -> tmp;
				inst.field_3 = "";
				inst.field_4 = "";
				inst.field_5 = "";
				inst.label = newLabel();
				threeAC.push_back(inst);

				inst.field_1 = "call";
				inst.field_2 = "strcmp";
				inst.field_3 = "";
				inst.field_4 = "";
				inst.field_5 = "";
				inst.label = newLabel();
				threeAC.push_back(inst);

				string trm = newTmp();
				tempType[trm] = "int";
				inst.field_1 = trm;
				inst.field_2 = "=";
				inst.field_3 = "popparam";
				inst.field_4 = "";
				inst.field_5 = "";
				inst.label = newLabel();
				threeAC.push_back(inst);

				inst.field_1 = tmp;
				inst.field_2 = "=";
				if (children[1] -> name == "==")
					inst.field_3 = "teste";
				if (children[1] -> name == "!=")
					inst.field_3 = "testne";
				if (children[1] -> name == "<")
					inst.field_3 = "testl";
				if (children[1] -> name == ">")
					inst.field_3 = "testg";
				if (children[1] -> name == "<=")
					inst.field_3 = "testle";
				if (children[1] -> name == ">=")
					inst.field_3 = "testge";
				inst.field_4 = trm;
				inst.field_5 = "";
				inst.label = newLabel();
				threeAC.push_back(inst);
			}

			else 
			{
				inst.field_1 = tmp;
				inst.field_2 = "=";
				inst.field_3 = children[0] -> tmp;
				inst.field_4 = children[1] -> name;
				inst.field_5 = children[2] -> tmp;
				inst.label = newLabel();
				threeAC.push_back(inst);
			}
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
		type = children[0] -> type;

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
			tempType[tmp] = isCompatible(tempType[children[0] -> tmp], tempType[children[2] -> tmp]);
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
			tempType[tmp] = isCompatible(tempType[children[0] -> tmp], tempType[children[2] -> tmp]);
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
			tempType[tmp] = isCompatible(tempType[children[0] -> tmp], tempType[children[2] -> tmp]);
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
			tempType[tmp] = isCompatible(tempType[children[0] -> tmp], tempType[children[2] -> tmp]);
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
			tempType[tmp] = isCompatible(tempType[children[0] -> tmp], tempType[children[2] -> tmp]);
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
			tempType[tmp] = isCompatible(tempType[children[0] -> tmp], tempType[children[2] -> tmp]);
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
			tempType[tmp] = tempType[children[1] -> tmp];
			code inst;
			inst.field_1 = tmp;
			inst.field_2 = "=";
			inst.field_3 = children[0] -> name;
			inst.field_4 = children[1] -> tmp;

			if (children[0] -> name == "-")
			{
				inst.field_3 = "-1";
				inst.field_4 = "*";
				inst.field_5 = children[1] -> tmp;
			}

			if (children[0] -> name == "+")
			{
				inst.field_3 = children[1] -> tmp;
			}

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
			tempType[tmp] = "int";
			code inst;
			inst.field_1 = tmp;
			inst.field_2 = "=";
			inst.field_3 = children[0] -> tmp;
			inst.field_4 = "";
			inst.field_5 = "";
			inst.label = newLabel();
			threeAC.push_back(inst);

			string tmp1 = newTmp();
			tempType[tmp1] = "int";
			inst.field_1 = tmp1;
			inst.field_2 = "=";
			inst.field_3 = children[2] -> tmp;
			inst.field_4 = "";
			inst.field_5 = "";
			inst.label = newLabel();
			threeAC.push_back(inst);

			string label = newLabel();
			string tmp2 = newTmp();
			tempType[tmp2] = "bool";
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
			tempType[tmp] = entry -> type;
		}

		for(int i = nparams -1; i >= 0; i--)
		{
			TreeNode* node = ((host -> children)[2] -> children)[i];
			tableRecord* record = new tableRecord(node -> name, node -> dataType);
			params.insert(params.begin(), record);
		}

		if (entry && (entry -> recordType == recordType::TYPE_CLASS))
		{
			funcEntry = entry -> symTab -> lookup_table("__init__");
		}

		else {

			if (dotTable)
			{
				params.insert(params.begin(), dotRecord);
				funcEntry = dotTable -> lookup(children[0] -> name, recordType::TYPE_FUNCTION, &params);
			}

			else {
				funcEntry = table -> lookup(children[0] -> name, recordType::TYPE_FUNCTION, &params);
			}

		}

		assert (funcEntry);

		for(int i = nparams -1; i >= 0; i--)
		{
			TreeNode* node = ((host -> children)[2] -> children)[i];
			tableRecord* entry = ((funcEntry->symTab)->entries)[i];

			if (entry -> type == "bool" && node -> dataType != "bool")
			{
				code inst;
				string tmp = newTmp();
				tempType[tmp] = "bool";
				inst.field_1 = tmp;
				inst.field_2 = "=";
				inst.field_3 = tempExprs[i];
				inst.field_4 = "!=";
				inst.field_5 = "0";
				inst.label = newLabel();
				threeAC.push_back(inst);
				tempExprs[i] = tmp;
			}
		}

		for(int i = nparams -1; i >= 0; i--)
		{
			inst.field_1 = "param";
			inst.field_2 = tempExprs[i];
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
		}

		else {

			if (dotRecord)
			{
				inst.field_1 = "param";
				inst.field_2 = mangle(dotRecord_1 -> name);
				inst.label = newLabel();
				threeAC.push_back(inst);
				nparams ++;
			}

		}

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
			tempType[tmp] = funcEntry -> type;
			if ((funcEntry -> type).compare(0, 4, "list") == 0)
				tempType["*(" + tmp + ")"] = (funcEntry -> type).substr(5, (funcEntry -> type).length() - 6);
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
			tempType[tmpry] = "int";
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
			tempType[t1] = "list_" + type + "_";
			tempType["*(" + t1 + ")"] = type;
			inst.field_1 = t1;
			inst.field_2 = "=";
			inst.field_3 = mangle(children[0] -> name);
			inst.field_4 = "+";
			inst.field_5 = tmpry;
			inst.label = newLabel();
			threeAC.push_back(inst);

			tmp = "*(" + t1 + ")";
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
			tempType[t1] = entry -> type;
			inst.field_1 = t1;
			inst.field_2 = "=";
			inst.field_3 = mangle(children[0] -> name);
			inst.field_4 = "";
			inst.field_5 = "";
			inst.label = newLabel();
			threeAC.push_back(inst);

			children[0] -> tmp = t1;

			string t2 = newTmp();
			tempType[t2] = "list_ " + children[2]->host->dataType + "_";
			tempType["*(" + t2 + ")"] = children[2]->host->dataType;
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
			if (stringMap.find(children[0] -> tmp) == stringMap.end())
				stringMap[children[0] -> tmp] = ".LC" + to_string(stringMapSize++);
			allocate_mem(to_string((children[0] -> tmp).length() - 1 + 8));
			tmp = MemRg;
			tempType[tmp] = "str";
			tempType["*(" + tmp + ")"] = "int";
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
			tempType[a1] = "str";
			inst.field_1 = a1;
			inst.field_2 = "=";
			inst.field_3 = "findAddress";
			inst.field_4 = stringMap[children[0] -> tmp];
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
			inst.field_2 = tmp;
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
				tempType[tmp] = entry -> type;
			}
			else 
			{
				tmp = mangle(children[0] -> name);
				tempType[tmp] = entry -> type;
			}
		}

		else if (children[0]->type != "NON_TERMINAL")
		{
			tmp = children[0] -> name;
			if (children[0] -> type == "FLOAT_LITERAL") tempType[tmp] = "float";
			if (children[0] -> type == "INT_LITERAL") tempType[tmp] = "int";
			if (children[0] -> type == "KEYWORD") tempType[tmp] = "None";
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
		tempType[tmp] = "str";



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
			tmp = "\"" + children[0]->tmp + children[1]->tmp + "\"";
			tempType[tmp] = "str";
		}
		else
		{
			formatString(children[0]->tmp);
			tmp = "\"" + children[0]->tmp + "\"";
			tempType[tmp] = "str";
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
		tempType[tmp] = host -> dataType;
		tempType["*(" + tmp + ")"] = type;

		code inst;
		string location = newTmp();
		tempType[location] = host -> dataType;
		tempType["*(" + location + ")"] = type;

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

void main_changer()
{
	string label = "";
	for(int i = 0; i < threeAC.size() - 1; i++)
	{
		if (threeAC[i].field_1 == "end_function" && threeAC[i+1].field_1 == "param" && threeAC[i+1].field_2 == "17")
		{
			threeAC[i+1].field_1 = "begin_function";
			threeAC[i+1].field_2 = "#main";
			threeAC[i+1].field_3 = "";
			threeAC[i+1].field_4 = "";
			threeAC[i+1].field_5 = "";

			assert(i + 15 < threeAC.size());
			label = threeAC[i+15].field_4;

			// remove the 14 instructions
			threeAC.erase(threeAC.begin() + i + 2, threeAC.begin() + i + 16);
		}
	}

	if (label.size())
	{
		bool flag = false;
		code inst;
		inst.field_1 = "end_function";
		inst.field_2 = "";
		inst.field_3 = "";
		inst.field_4 = "";
		inst.field_5 = "";
		inst.label = newLabel();

		for(int i=0; i< threeAC.size(); i++)
		{
			if (threeAC[i].label == label)
			{
				flag = true;
				threeAC.insert(threeAC.begin() + i, inst);
			}
		}

		if (!flag)
		{
			threeAC.push_back(inst);
		}
	}
}

void symTableModifier()
{
	symTable* TABLE = NULL;
	map<string, int> Inserted;
	for (auto &inst: threeAC)
	{
		if (inst.field_1 == "begin_function")
		{
			string funcName = inst.field_2;
			// cout << funcName << endl;
			int last = funcName.size();
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
			TABLE = table;

		}

		else if (inst.field_1 == "end_function")
		{
			TABLE = NULL;
		}


		else if (TABLE)
		{
			string name = inst.field_1;
			if (name[0] == '$')
			{
				if (Inserted.find(name) == Inserted.end())
				{
					Inserted[name] = 1;
					tableRecord* record = new tableRecord();
					record -> name = name;
					record -> size = 8;
					record -> type = tempType[name];
					record -> recordType = recordType::TEMPORARY;
					TABLE -> insert(record);
					free(record);
					record = TABLE -> lookup_table(name);
					assert(record);
					Temp_to_record[name] = record;
				}
			}

			name = inst.field_2;
			if (name[0] == '$')
			{
				if (Inserted.find(name) == Inserted.end())
				{
					Inserted[name] = 1;
					tableRecord* record = new tableRecord();
					record -> name = name;
					record -> size = 8;
					record -> type = tempType[name];
					record -> recordType = recordType::TEMPORARY;
					TABLE -> insert(record);
					free(record);
					record = TABLE -> lookup_table(name);
					assert(record);
					Temp_to_record[name] = record;
				}
			}

			name = inst.field_3;
			if (name[0] == '$')
			{
				if (Inserted.find(name) == Inserted.end())
				{
					Inserted[name] = 1;
					tableRecord* record = new tableRecord();
					record -> name = name;
					record -> size = 8;
					record -> type = tempType[name];
					record -> recordType = recordType::TEMPORARY;
					TABLE -> insert(record);
					free(record);
					record = TABLE -> lookup_table(name);
					assert(record);
					Temp_to_record[name] = record;
				}
			}

			name = inst.field_4;
			if (name[0] == '$')
			{
				if (Inserted.find(name) == Inserted.end())
				{
					Inserted[name] = 1;
					tableRecord* record = new tableRecord();
					record -> name = name;
					record -> size = 8;
					record -> type = tempType[name];
					record -> recordType = recordType::TEMPORARY;
					TABLE -> insert(record);
					free(record);
					record = TABLE -> lookup_table(name);
					assert(record);
					Temp_to_record[name] = record;
				}
			}

			name = inst.field_5;
			if (name[0] == '$')
			{
				if (Inserted.find(name) == Inserted.end())
				{
					Inserted[name] = 1;
					tableRecord* record = new tableRecord();
					record -> name = name;
					record -> type = tempType[name];
					record -> size = 8;
					record -> recordType = recordType::TEMPORARY;
					TABLE -> insert(record);
					free(record);
					record = TABLE -> lookup_table(name);
					assert(record);
					Temp_to_record[name] = record;
				}
			}
		}
	}
}

void offset_modifier(symbolTable *Table)
{
	visited[Table] = 1;
	for(auto child : Table->childIndices) 
	{
		assert((Table->entries)[child] -> symTab);
		if (visited.find((Table->entries)[child] -> symTab) == visited.end())
			offset_modifier(((Table->entries)[child]) -> symTab);
	}

	if (Table->tableType == tableType::FUNCTION)
	{
		for (int index = 0; index < Table->numParams; index++)
		{
			if (index < 6)
				(Table->entries)[index]->offset = (Table->size + index * 8);
			else
				(Table->entries)[index]->offset = - ((index - 6) * 8 + 24);
		}
	}
	
}

string class_convert(string funcName)
{
	string new_var = "";
	string class_name = "";
	vector<int> param_indices;
	int last = funcName.size();
	int percent_loc = 0;
	for(int i = 0; i < funcName.length() - 2; i++)
	{
		if (funcName[i] == '%')
		{
			percent_loc = i;
			new_var += funcName.substr(0, i);
		}
		if(funcName.substr(i, 3) == "_Cc")
		{
			class_name = funcName.substr(percent_loc, i - percent_loc);
			new_var += copyMap[class_name] + "_Cc";
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

	if (class_name.length() != 0)
	{
		new_var += "_Zz" + copyMap[class_name];
		if (param_indices.size() == 1)
		{
			new_var += funcName.substr(last, funcName.length() - last);
		}
		else
		{
			new_var += funcName.substr(param_indices[1], funcName.length() - param_indices[1]);
		}
		return new_var;
	}
	return funcName;
}

void fillInheritance()
{
	for (int i = 0; i < threeAC.size(); i++)
	{
		code tac = threeAC[i];
		if (tac.field_1 == "begin_function" && tac.field_2.find("_Cc") != string::npos && tac.field_2.find("__init__") == string::npos)
		{
			string funcName = tac.field_2;
			string class_name = "";
			int percent_loc = 0;

			for (int i=0; i<funcName.length() - 2; i++)
			{
				if (funcName[i] == '%')
				{
					percent_loc = i;
				}
				if(funcName.substr(i, 3) == "_Cc")
				{
					class_name = funcName.substr(percent_loc, i - percent_loc);
				}
			}

			if (copyMap.find(class_name) != copyMap.end())
			{
				int label_offset = atoi(tac.label.substr(0, tac.label.size() - 1).c_str()) - label;
				while (tac.field_1 != "end_function")
				{
					code inst;
					cout << tac.field_1 << endl;
					if (tac.field_1.length())
						inst.field_1 = class_convert(tac.field_1);

					cout << tac.field_2 << endl;
					if (tac.field_2.length())
						inst.field_2 = class_convert(tac.field_2);

					cout << tac.field_3 << endl;
					if (tac.field_3.length())
						inst.field_3 = class_convert(tac.field_3);

					cout << tac.field_4 << endl;
					if (tac.field_4.length())
						inst.field_4 = class_convert(tac.field_4);

					cout << tac.field_5 << endl;
					if (tac.field_5.length())
						inst.field_5 = class_convert(tac.field_5);

					if (tac.field_1 == "goto")
					{
						inst.field_2 = to_string(atoi(tac.field_2.c_str()) + label_offset);
					}
					if (tac.field_3 == "goto")
					{
						inst.field_4 = to_string(atoi(tac.field_4.c_str()) + label_offset);
					}
					inst.label = newLabel();
					class_copy.push_back(inst);
					tac = threeAC[++i];
				}
				i--;
			}
		}
	}

	threeAC.insert(threeAC.end(), class_copy.begin(), class_copy.end());
}

void Parasite::genCode()
{
	map<string, string> labelMap;

	genAC();
	formFirstLast();
	fillCode();
	main_changer();
	fillInheritance();

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

	symTableModifier();
	visited.clear();
	offset_modifier(globTable);

	code inst;
	inst.label = "END";
	threeAC.push_back(inst);
}
