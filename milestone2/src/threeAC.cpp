#include "include/threeAC.hpp"

extern string inputFile;
extern symbolTable* globTable;

int label = 0;
int tmp = 0;

string state = "#$" + inputFile + "#$";
symbolTable* table = globTable;
stack<pair<string, string>> labelStack;
vector<code> threeAC;

string newLabel()
{
	return "L" + to_string(++label);
}

string newTmp()
{
	return "@#__" + to_string(++tmp) + "__#@";
}

void Parasite::genAC()
{


	if (name == "file")
	{
					
		/*

			file: ENDMARK 
				| NEWLINE ENDMARK 
				| statements ENDMARK 

		*/

		/* nothing to do */

		next = newLabel();
		current = newLabel();

		if (children[0] -> name == "statements")
		{
			children[0] -> next = next;
			children[0] -> current = current;
		}
		
	   
	}


	if (name == "statements")
	{
					
		/*

			statements: statements statement
				| statement

		*/

		if (children.size() == 2)
		{
			children[0] -> next = newLabel();
			children[0] -> current = current;

			children[1] -> next = next;
			children[1] -> current = children[0] -> next;
		}

		else 
		{
			children[0] -> next = next;
			children[0] -> current = current;
		}

	}


	if (name == "statement")
	{
					
		/*

			statement: compound_stmt 
				| simple_stmts

		*/

		children[0] -> next = next;
		children[0] -> current = current;

		/* nothing to do */

	}


	if (name == "simple_stmts")
	{
					
		/*

			simple_stmts: simple1 simple2 NEWLINE

		*/

		children[0] -> next = newLabel();
		children[0] -> current = current;
		children[1] -> next = next;
		children[1] -> current = children[0] -> next;
	
	}


	if (name == "simple1")
	{
					
		/*

			simple1: simple_stmt
				| simple1 DLM_SM_COL simple_stmt

		*/

		if (children.size() == 1)
		{
			children[0] -> next = next;
			children[0] -> current = current;
		}
		else
		{
			children[0] -> next = newLabel();
			children[0] -> current = current;
			children[2] -> next = next;
			children[2] -> current = children[0] -> next;
		}

	}


	if (name == "simple2")
	{
					
		/*

			simple2: 
				| DLM_SM_COL

		*/
	
		/* nothing to do */

	}


	if (name == "simple_stmt")
	{
					
		/*

			simple_stmt: assignment
				| expressions 
				| return_stmt
				| KW_break 
				| KW_continue 

		*/

		children[0] -> next = next;
		children[0] -> current = current;

		if (children[0] -> name == "continue")
		{
			children[0] -> next = newLabel();
			children[0] -> current = current;
		}

		if (children[0] -> name == "break")
		{
			children[0] -> next = newLabel();
			children[0] -> current = current;
		}

	}


	if (name == "compound_stmt")
	{
					
		/*

			compound_stmt: function_def
				| if_stmt
				| class_def
				| for_stmt
				| while_stmt

		*/

		children[0] -> next = next;
		children[0] -> current = current;

	}


	if (name == "typedecl")
	{
					
		/*

			typedecl: l_primary DLM_COLON expression

		*/

		children[0] -> next = newLabel();
		children[0] -> current = current;
		children[2] -> next = next;
		children[2] -> current = children[0] -> next;
		
	}


	if (name == "assignment")
	{
					
		/*

			assignment: typedecl
				| typedecl OP_ASN_ASN expression 
				| l_primary augassign expression 
				| multi_targets_assgn   

		*/

		if (children.size() == 1)
		{
			children[2] -> next = newLabel();
			children[2] -> current = current;
		}

		else
		{
			children[0] -> current = current;
			children[0] -> next = newLabel();
			children[2] -> next = next;
			children[2] -> current = children[0] -> next;
		}

	}


	if (name == "multi_targets_assgn")
	{
					
		/*

			multi_targets_assgn: l_primary OP_ASN_ASN expression
				| l_primary OP_ASN_ASN multi_targets_assgn

		*/

		children[0] -> next = newLabel();
		children[0] -> current = current;
		children[2] -> next = next;
		children[2] -> current = children[0] -> next;

	}


	if (name == "augassign")
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


	if (name == "return_stmt")
	{
					
		/*

			return_stmt: KW_return expression
				| KW_return 

		*/

		if (children.size() == 1)
		{
			children[0] -> next = next;
			children[0] -> current = current;
		}

		else 
		{
			children[0] -> next = newLabel();
			children[0] -> current = current;
			children[1] -> next = next;
			children[1] -> current = children[0] -> next;
		}

	}


	if (name == "block")
	{
					
		/*

			block: NEWLINE INDENT statements DEDENT 
				| simple_stmts

		*/

		if (children.size() == 1)
		{
			children[0] -> next = next;
			children[0] -> current = current;
		}

		else 
		{
			children[1] -> next = next;
			children[1] -> current = current;
		}

	}


	if (name == "class_def")
	{
					
		/*

			class_def: KW_class NAME is_arguments DLM_COLON block

		*/

		children[2] -> next = newLabel();
		children[2] -> current = current;
		children[4] -> next = next;
		children[4] -> current = children[2] -> next;

		state = state + children[1] -> name + "#$";
		tableRecord* entry = table -> lookup(children[2] -> name, recordType::TYPE_CLASS);
		assert (entry);
		assert (entry -> symTab);
		table = entry -> symTab;

	}


	if (name == "function_def")
	{
					
		/*

			function_def: KW_def NAME DLM_LFT_PRN params DLM_RGT_PRN is_fn_expression DLM_COLON block

		*/

		children[3] -> next = newLabel();
		children[3] -> current = current;
		children[5] -> next = newLabel();
		children[5] -> current = children[3] -> next;
		children[7] -> next = next;
		children[7] -> current = children[5] -> next;

		/* class constructors are not an issue here */
		string funcName = state + children[1] -> name + "%@";
		tableRecord* entry = table -> lookup(children[1] -> name);
		
		assert (entry);
		assert (entry -> recordType == recordType::TYPE_FUNCTION);
		assert (entry -> symTab);
		table = entry -> symTab;

		for (int i = 0; i < table -> numParams; i++)
		{
			funcName += (table -> entries)[i] -> type + "%@";
		}

		code inst;
		inst.field_1 = "begin_function";
		inst.field_2 = "funcName";

	}


	if (name == "is_arguments")
	{
					
		/*

			is_arguments:
				| DLM_LFT_PRN DLM_RGT_PRN
				| DLM_LFT_PRN arguments DLM_RGT_PRN

		*/

		if (children.size() == 3)
		{
			children[1] -> next = next;
			children[1] -> current = current;
		}

	}


	if (name == "is_fn_expression")
	{
					
		/*

			is_fn_expression:
				| DLM_TO expression

		*/

		if (children.size() == 2)
		{
			children[1] -> next = next;
			children[1] -> current = current;
		}

	}


	if (name == "params")
	{
					
		/*

			params: 
				| param param_nd 

		*/

		if (children.size() == 2)
		{
			children[0] -> next = newLabel();
			children[0] -> current = current;
			children[1] -> next = next;
			children[1] -> current = children[0] -> next;
		}

	}


	if (name == "param_nd")
	{
					
		/*

			param_nd: 
				| param_no_default param_nd
				| DLM_COMMA

		*/

		if (children.size() == 2)
		{
			children[0] -> next = newLabel();
			children[0] -> current = current;
			children[1] -> next = next;
			children[1] -> current = children[0] -> next;
		}

	}


	if (name == "param_no_default")
	{
					
		/*

			param_no_default: DLM_COMMA param 

		*/

		children[1] -> next = next;
		children[1] -> current = current;
	}


	if (name == "param")
	{
					
		/*

			param: NAME 
				| annotation

		*/

		if (children[0] -> name == "annotation")
		{
			children[1] -> next = next;
			children[1] -> current = current;
		}

	}


	if (name == "annotation")
	{
					
		/*

			annotation: NAME DLM_COLON expression 

		*/

		children[2] -> next = next;
		children[2] -> current = current;

	}


	if (name == "if_stmt")
	{
					
		/*

			if_stmt: KW_if expression DLM_COLON block elif_stmt 
				| KW_if expression DLM_COLON block 
				| KW_if expression DLM_COLON block else_block 

		*/

		if (children.size() == 4)
		{
			children[1] -> trueL = newLabel();
			children[1] -> falseL = next;
			children[1] -> current = current;
			children[3] -> next = next;
			children[3] -> current = children[1] -> trueL;
		}

		else
		{
			children[1] -> trueL = newLabel(); 
			children[1] -> falseL = newLabel();
			children[1] -> current = current;
			children[3] -> next = next; 
			children[3] -> current = children[1] -> trueL; 
			children[4] -> next = next; 
			children[4] -> current = children[1] -> falseL; 
		}


	}


	if (name == "elif_stmt")
	{
					
		/*

			elif_stmt: KW_elif expression DLM_COLON block elif_stmt 
				| KW_elif expression DLM_COLON block 
				| KW_elif expression DLM_COLON block else_block 

		*/

		if (children.size() == 4)
		{
			children[1] -> trueL = newLabel();
			children[1] -> falseL = next;
			children[1] -> current = current;
			children[3] -> next = next;
			children[3] -> current = children[1] -> trueL;
		}

		else
		{
			children[1] -> trueL = newLabel(); 
			children[1] -> falseL = newLabel();
			children[1] -> current = current;
			children[3] -> next = next; 
			children[3] -> current = children[1] -> trueL; 
			children[4] -> next = next; 
			children[4] -> current = children[1] -> falseL; 
		}
	}


	if (name == "else_block")
	{
					
		/*

			else_block: KW_else DLM_COLON block 

		*/

		children[2] -> next = next;
		children[2] -> current = current;

	}


	if (name == "while_stmt")
	{
					
		/*

			while_stmt: KW_while expression DLM_COLON block 
				| KW_while expression DLM_COLON block else_block

		*/

		if (children.size() == 4)
		{
			children[1] -> trueL = newLabel();
			children[1] -> falseL = next;
			children[1] -> current = current;
			children[3] -> next = next;
			children[3] -> current = children[1] -> trueL;
		}

		else
		{
			children[1] -> trueL = newLabel(); 
			children[1] -> falseL = newLabel();
			children[1] -> current = current;
			children[3] -> next = next; 
			children[3] -> current = children[1] -> trueL; 
			children[4] -> next = next; 
			children[4] -> current = children[1] -> falseL; 
		}

	}


	if (name == "for_stmt")
	{
					
		/*

			for_stmt: KW_for for_expr DLM_COLON block
				| KW_for for_expr DLM_COLON block else_block

		*/

		if (children.size() == 4)
		{
			children[1] -> trueL = newLabel();
			children[1] -> falseL = next;
			children[1] -> current = current;
			children[3] -> next = next;
			children[3] -> current = children[1] -> trueL;
		}

		else
		{
			children[1] -> trueL = newLabel(); 
			children[1] -> falseL = newLabel();
			children[1] -> current = current;
			children[3] -> next = next; 
			children[3] -> current = children[1] -> trueL; 
			children[4] -> next = next; 
			children[4] -> current = children[1] -> falseL; 
		}
		
	}

	if (name == "for_expr")
	{
					
		/*
			for_expr: l_primary KW_in expression
		*/

		children[0] -> next = newLabel();
		children[0] -> current = current;
		children[2] -> next = next;
		children[2] -> current = children[0] -> next;

	}


	if (name == "expressions")
	{
					
		/*

			expressions: expression DLM_COMMA expressions
				| expression DLM_COMMA
				| expression   

		*/

		if (children.size() == 1)
		{
			children[0] -> next = next;
			children[0] -> current = current;
		}

		if (children.size() == 2)
		{
			children[0] -> next = next;
			children[0] -> current = current;
		}

		if (children.size() == 3)
		{
			children[0] -> next = newLabel();
			children[0] -> current = current;
			children[2] -> next = next;		
			children[2] -> current = children[0] -> next;
		}


	}


	if (name == "expression")
	{
					
		/*
			expression: disjunction             
		*/

		children[0] -> next = next;
		children[0] -> current = current;
	}


	if (name == "disjunction")
	{
					
		/*

			disjunction: conjunction
				| disjunction OP_LOG_OR conjunction

		*/

		if (children.size() == 1)
		{
			children[0] -> next = next;
			children[0] -> current = current;
		}

		else 
		{
			children[0] -> next = newLabel();
			children[0] -> current = current;
			children[2] -> next = next;		
			children[2] -> current = children[0] -> next;
		}


	}


	if (name == "conjunction")
	{
					
		/*

			conjunction: inversion
				| conjunction OP_LOG_AND inversion

		*/

		if (children.size() == 1)
		{
			children[0] -> next = next;
			children[0] -> current = current;
		}

		else 
		{
			children[0] -> next = newLabel();
			children[0] -> current = current;
			children[2] -> next = next;		
			children[2] -> current = children[0] -> next;
		}

	}


	if (name == "inversion")
	{
					
		/*

			inversion: OP_LOG_NOT inversion 
				| comparison

		*/

		if (children.size() == 1)
		{
			children[0] -> next = next;
			children[0] -> current = current;
		}

		else 
		{
			children[0] -> next = newLabel();
			children[0] -> current = current;
			children[1] -> next = next;		
			children[1] -> current = children[0] -> next;
		}

	}


	if (name == "comparison")
	{
					
		/*

			comparison: bitwise_or
				| comparison bitwise_operator bitwise_or

		*/

		if (children.size() == 1)
		{
			children[0] -> next = next;
			children[0] -> current = current;
		}

		else 
		{
			children[0] -> next = newLabel();
			children[0] -> current = current;
			children[2] -> next = next;		
			children[2] -> current = children[0] -> next;
		}

	}


	if (name == "bitwise_operator")
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


	if (name == "bitwise_or")
	{
					
		/*

			bitwise_or: bitwise_or OP_BIT_OR bitwise_xor 
				| bitwise_xor

		*/

		if (children.size() == 1)
		{
			children[0] -> next = next;
			children[0] -> current = current;
		}

		else 
		{
			children[0] -> next = newLabel();
			children[0] -> current = current;
			children[2] -> next = next;		
			children[2] -> current = children[0] -> next;
		}


	}


	if (name == "bitwise_xor")
	{
					
		/*

			bitwise_xor: bitwise_xor OP_BIT_XOR bitwise_and 
				| bitwise_and

		*/

		if (children.size() == 1)
		{
			children[0] -> next = next;
			children[0] -> current = current;
		}

		else 
		{
			children[0] -> next = newLabel();
			children[0] -> current = current;
			children[2] -> next = next;		
			children[2] -> current = children[0] -> next;
		}

	}


	if (name == "bitwise_and")
	{
					
		/*

			bitwise_and: bitwise_and OP_BIT_AND shift_expr 
				| shift_expr

		*/

		if (children.size() == 1)
		{
			children[0] -> next = next;
			children[0] -> current = current;
		}

		else 
		{
			children[0] -> next = newLabel();
			children[0] -> current = current;
			children[2] -> next = next;		
			children[2] -> current = children[0] -> next;
		}
	}


	if (name == "shift_expr")
	{
					
		/*

			shift_expr: shift_expr OP_BIT_LS sum 
				| shift_expr OP_BIT_RS sum 
				| sum

		*/

		if (children.size() == 1)
		{
			children[0] -> next = next;
			children[0] -> current = current;
		}

		else 
		{
			children[0] -> next = newLabel();
			children[0] -> current = current;
			children[2] -> next = next;		
			children[2] -> current = children[0] -> next;
		}
	}


	if (name == "sum")
	{
					
		/*

			sum: sum OP_ATH_ADD term 
				| sum OP_ATH_SUB term 
				| term

		*/

		if (children.size() == 1)
		{
			children[0] -> next = next;
			children[0] -> current = current;
		}

		else 
		{
			children[0] -> next = newLabel();
			children[0] -> current = current;
			children[2] -> next = next;		
			children[2] -> current = children[0] -> next;
		}
	}


	if (name == "term")
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
			children[0] -> next = next;
			children[0] -> current = current;
		}

		else 
		{
			children[0] -> next = newLabel();
			children[0] -> current = current;
			children[2] -> next = next;		
			children[2] -> current = children[0] -> next;
		}
	}


	if (name == "factor")
	{
					
		/*

			factor: OP_ATH_ADD factor 
				| OP_ATH_SUB factor 
				| OP_BIT_NEG factor 
				| power

		*/

		if (children.size() == 1)
		{
			children[0] -> next = next;
			children[0] -> current = current;
		}

		else 
		{
			children[0] -> next = newLabel();
			children[0] -> current = current;
			children[1] -> next = next;		
			children[1] -> current = children[0] -> next;
		}

	}


	if (name == "power")
	{
					
		/*

			power: primary OP_ATH_POW factor 
				| primary

		*/

		if (children.size() == 1)
		{
			children[0] -> next = next;
			children[0] -> current = current;
		}

		else 
		{
			children[0] -> next = newLabel();
			children[0] -> current = current;
			children[2] -> next = next;		
			children[2] -> current = children[0] -> next;
		}

	}


	if (name == "function_call")
	{
					
		/*

			function_call: NAME DLM_LFT_PRN args DLM_RGT_PRN

		*/

		children[2] -> next = next;
		children[2] -> current = current;

	}


	if (name == "args")
	{
					
		/*

			args:
				| expressions

		*/

		if (children.size() == 1)
		{
			children[0] -> next = next;
			children[0] -> current = current;
		}

	}


	if (name == "list_access")
	{
					
		/*

			list_access: NAME DLM_LFT_SQ expression DLM_RGT_SQ

		*/

		children[2] -> next = next;
		children[2] -> current = current;
	}


	if (name == "l_primary")
	{
					
		/*

			l_primary:  NAME DLM_DOT NAME 
				| list_access DLM_DOT NAME
				| NAME DLM_DOT list_access
				| list_access 
				| atom

		*/

		if (children[0] -> name == "list_access")
		{
			children[0] -> next = next;
			children[0] -> current = current;
		}

		if (children.size() == 3 && children[2] -> name == "list_access")
		{
			children[0] -> next = next;
			children[0] -> current = current;
		}

		if (children[0] -> name == "atom")
		{
			children[0] -> next = next;
			children[0] -> current = current;
		}

	}


	if (name == "primary")
	{
					
		/*

			primary: l_primary
				| list_access DLM_DOT function_call
				| NAME DLM_DOT function_call
				| function_call

		*/

		if (children.size() == 1)
		{
			children[0] -> next = next;
			children[0] -> current = current;
		}

		else
		{
			if (children[0] -> name == "list_access")
			{
				children[0] -> next = newLabel();
				children[0] -> current = current;
				children[2] -> next = next;
				children[2] -> current = children[0] -> next;

			}

			else
			{
				children[2] -> next = next;
				children[2] -> current = current;

			}

		} 

	}


	if (name == "atom")
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

		if (children[0] -> name == "group")
		{
			children[0] -> next = next;
			children[0] -> current = current;
		}

		if (children[0] -> name == "list")
		{
			children[0] -> next = next;
			children[0] -> current = current;
		}

		if (children[0] -> name == "strings")
		{
			children[0] -> next = next;
			children[0] -> current = current;
		}

	}


	if (name == "group")
	{
					
		/*

			group: DLM_LFT_PRN expression DLM_RGT_PRN 
				| DLM_LFT_PRN DLM_RGT_PRN

		*/

		if (children.size() == 3)
		{
			children[1] -> next = next;
			children[1] -> current = current;
		}

	}


	if (name == "string")
	{
					
		/*
			string: STRING_LITERAL              
		*/

		/* do nothing */

	}


	if (name == "strings")
	{
					
		/*

			strings: strings string
				| string

		*/

		if (children.size() == 1)
		{
			children[0] -> next = next;
			children[0] -> current = current;
		}

		if (children.size() == 2)
		{
			children[0] -> next = newLabel();
			children[0] -> current = current;
			children[1] -> next = next;
			children[1] -> current = children[0] -> next;
		}

	}

	if (name == "list_expr")
	{
					
		/*
			list_expr: expressions 
		*/

		children[0] -> next = next;
		children[0] -> current = current;

	}

	if (name == "list")
	{
					
		/*

			list: DLM_LFT_SQ list_expr DLM_RGT_SQ 

		*/

		children[2] -> next = next;
		children[2] -> current = current;

	}





	/////////////////////////////////////////// DFS BEGIN ///////////////////////////////////////////////////
	/////////////////////////////////////////// DFS BEGIN ///////////////////////////////////////////////////
	/////////////////////////////////////////// DFS BEGIN ///////////////////////////////////////////////////
	/////////////////////////////////////////// DFS BEGIN ///////////////////////////////////////////////////

	for (auto &i : children)
	{		
		i -> genAC();
	}
	
	/////////////////////////////////////////// DFS END ///////////////////////////////////////////////////
	/////////////////////////////////////////// DFS END ///////////////////////////////////////////////////
	/////////////////////////////////////////// DFS END ///////////////////////////////////////////////////
	/////////////////////////////////////////// DFS END ///////////////////////////////////////////////////
	/////////////////////////////////////////// DFS END ///////////////////////////////////////////////////



	if (name == "file")
	{
					
		/*

			file: ENDMARK 
				| NEWLINE ENDMARK 
				| statements ENDMARK 

		*/

		/* nothing to do */
	   
	}


	if (name == "statements")
	{
					
		/*

			statements: statements statement
				| statement

		*/

		/* nothing to do */

	}


	if (name == "statement")
	{
					
		/*

			statement: compound_stmt 
				| simple_stmts

		*/

		/* nothing to do */

	}


	if (name == "simple_stmts")
	{
					
		/*

			simple_stmts: simple1 simple2 NEWLINE

		*/

	
		/* nothing to do */

	}


	if (name == "simple1")
	{
					
		/*

			simple1: simple_stmt
				| simple1 DLM_SM_COL simple_stmt

		*/
	
		/* nothing to do */

	}


	if (name == "simple2")
	{
					
		/*

			simple2: 
				| DLM_SM_COL

		*/
	
		/* nothing to do */

	}


	if (name == "simple_stmt")
	{
					
		/*

			simple_stmt: assignment
				| expressions 
				| return_stmt
				| KW_break 
				| KW_continue 

		*/

		if (children[0] -> name == "continue")
		{
			assert (!labelStack.empty());
			code inst;
			inst.field_1 = "goto";
			inst.field_2 = (labelStack.top()).first;
			inst.label = children[0] -> next;
			threeAC.push_back(inst);
		}

		if (children[0] -> name == "break")
		{
			assert (!labelStack.empty());
			code inst;
			inst.field_1 = "goto";
			inst.field_2 = (labelStack.top()).second;
			labelStack.pop();
			inst.label = children[0] -> next;
			threeAC.push_back(inst);
		}

	}


	if (name == "compound_stmt")
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


	if (name == "typedecl")
	{
					
		/*

			typedecl: l_primary DLM_COLON expression

		*/

		tmp = children[0] -> tmp;

		/* nothing to do */

	}


	if (name == "assignment")
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
			threeAC.push_back(inst);
		}

		if (children[0] -> name == "primary" && children.size() == 3)
		{

			int operation_len = (children[1] -> name).length();
			string operation = (children[1] -> name).substr(1, operation_len - 1); 

			code inst;
			inst.field_1 = children[0] -> tmp;
			inst.field_2 = "=";
			inst.field_3 = children[0] -> tmp;
			inst.field_4 = operation;
			inst.field_5 = children[2] -> tmp;
			threeAC.push_back(inst);

		}


	}


	if (name == "multi_targets_assgn")
	{
					
		/*

			multi_targets_assgn: l_primary OP_ASN_ASN expression
				| l_primary OP_ASN_ASN multi_targets_assgn

		*/

		code inst;
		inst.field_1 = children[0] -> tmp;
		inst.field_2 = "=";
		inst.field_3 = children[2] -> tmp;
		threeAC.push_back(inst);


	}


	if (name == "augassign")
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


	if (name == "return_stmt")
	{
					
		/*

			return_stmt: KW_return expression
				| KW_return 

		*/

		code inst;
		inst.field_1 = "ret";

		if (children.size() == 2)
		{
			inst.field_2 = children[1] -> tmp;
		}

		threeAC.push_back(inst);

	}


	if (name == "block")
	{
					
		/*

			block: NEWLINE INDENT statements DEDENT 
				| simple_stmts

		*/

	}


	if (name == "class_def")
	{
					
		/*

			class_def: KW_class NAME is_arguments DLM_COLON block

		*/

	}


	if (name == "function_def")
	{
					
		/*
			function_def: KW_def NAME DLM_LFT_PRN params DLM_RGT_PRN is_fn_expression DLM_COLON block
		*/



	}


	if (name == "is_arguments")
	{
					
		/*

			is_arguments:
				| DLM_LFT_PRN DLM_RGT_PRN
				| DLM_LFT_PRN arguments DLM_RGT_PRN

		*/
	}


	if (name == "is_fn_expression")
	{
					
		/*

			is_fn_expression:
				| DLM_TO expression

		*/
	}


	if (name == "params")
	{
					
		/*

			params: 
				| param param_nd 

		*/
	}


	if (name == "param_nd")
	{
					
		/*

			param_nd: 
				| param_no_default param_nd
				| DLM_COMMA

		*/
	}


	if (name == "param_no_default")
	{
					
		/*

			param_no_default: DLM_COMMA param 

		*/
	}


	if (name == "param")
	{
					
		/*

			param: NAME 
				| annotation

		*/



	}


	if (name == "annotation")
	{
					
		/*

			annotation: NAME DLM_COLON expression 

		*/
	}


	if (name == "if_stmt")
	{
					
		/*

			if_stmt: KW_if expression DLM_COLON block elif_stmt 
				| KW_if expression DLM_COLON block 
				| KW_if expression DLM_COLON block else_block 

		*/

		
		code inst;
		if (children.size() == 4)
		{

			// inst.field_1 = "if";
			// inst.field_2 = children[1] -> tmp;
			// inst.field_3 = "goto";
			// inst.field_4 = "L" + to_string(children[3] -> next);
			// inst.
			
			// threeAC.push_back(inst);

			// inst.field_1 = "if_false";
			// inst.field_2 = children[1] -> tmp;
			// inst.field_3 = "goto";
			// inst.field_4 = "L" + to_string(children[3] -> next);
			// inst.

			// threeAC.push_back(inst);

		}

		else if (children[4] -> name == "elif_stmt")
		{

		}

		else
		{

		}
		

	}


	if (name == "elif_stmt")
	{
					
		/*

			elif_stmt: KW_elif expression DLM_COLON block elif_stmt 
				| KW_elif expression DLM_COLON block 
				| KW_elif expression DLM_COLON block else_block 

		*/
	}


	if (name == "else_block")
	{
					
		/*

			else_block: KW_else DLM_COLON block 

		*/
	}


	if (name == "while_stmt")
	{
					
		/*

			while_stmt: KW_while expression DLM_COLON block 
				| KW_while expression DLM_COLON block else_block 

		*/
	}


	if (name == "for_stmt")
	{
					
		/*

			for_stmt: KW_for for_expr DLM_COLON block
				| KW_for for_expr DLM_COLON block else_block

		*/

		

	}

	if (name == "for_expr")
	{
					
		/*

			for_expr: l_primary KW_in expression

		*/

	}


	if (name == "expressions")
	{
					
		/*

			expressions: expression DLM_COMMA expressions
				| expression DLM_COMMA
				| expression 

		*/

		/* nothing to do */

	}


	if (name == "expression")
	{
					
		/*
			expression: disjunction             
		*/

		

		tmp = children[0] -> tmp;

	}


	if (name == "disjunction")
	{
					
		/*

			disjunction: conjunction
				| disjunction OP_LOG_OR conjunction

		*/


		tmp = children[0] -> tmp;
		if (children.size() == 3)
		{
			code inst;
			inst.field_1 = tmp;
			inst.field_2 = children[0] -> tmp;
			inst.field_3 = children[1] -> name;
			inst.field_4 = children[2] -> tmp;
			threeAC.push_back(inst);
		}

	}


	if (name == "conjunction")
	{
					
		/*

			conjunction: inversion
				| conjunction OP_LOG_AND inversion

		*/


		tmp = children[0] -> tmp;
		if (children.size() == 3)
		{
			code inst;
			inst.field_1 = tmp;
			inst.field_2 = children[0] -> tmp;
			inst.field_3 = children[1] -> name;
			inst.field_4 = children[2] -> tmp;
			threeAC.push_back(inst);
		}

	}


	if (name == "inversion")
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
			code inst;
			inst.field_1 = newTmp();
			inst.field_2 = children[0] -> name;
			inst.field_3 = children[1] -> tmp;
			threeAC.push_back(inst);
		}

	}


	if (name == "comparison")
	{
					
		/*

			comparison: bitwise_or
				| comparison bitwise_operator bitwise_or

		*/

		tmp = children[0] -> tmp;
		if (children.size() == 3)
		{
			code inst;
			inst.field_1 = tmp;
			inst.field_2 = children[0] -> tmp;
			inst.field_3 = children[1] -> name;
			inst.field_4 = children[2] -> tmp;
			threeAC.push_back(inst);
		}

	}


	if (name == "bitwise_operator")
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


	if (name == "bitwise_or")
	{
					
		/*

			bitwise_or: bitwise_or OP_BIT_OR bitwise_xor 
				| bitwise_xor

		*/

		

		tmp = children[0] -> tmp;
		if (children.size() == 3)
		{
			code inst;
			inst.field_1 = tmp;
			inst.field_2 = children[0] -> tmp;
			inst.field_3 = children[1] -> name;
			inst.field_4 = children[2] -> tmp;
			threeAC.push_back(inst);
		}

	}


	if (name == "bitwise_xor")
	{
					
		/*

			bitwise_xor: bitwise_xor OP_BIT_XOR bitwise_and 
				| bitwise_and

		*/

		
		tmp = children[0] -> tmp;
		if (children.size() == 3)
		{
			code inst;
			inst.field_1 = tmp;
			inst.field_2 = children[0] -> tmp;
			inst.field_3 = children[1] -> name;
			inst.field_4 = children[2] -> tmp;
			threeAC.push_back(inst);
		}

	}


	if (name == "bitwise_and")
	{
					
		/*

			bitwise_and: bitwise_and OP_BIT_AND shift_expr 
				| shift_expr

		*/

		

		tmp = children[0] -> tmp;

		if (children.size() == 3)
		{
			code inst;
			inst.field_1 = tmp;
			inst.field_2 = children[0] -> tmp;
			inst.field_3 = children[1] -> name;
			inst.field_4 = children[2] -> tmp;
			threeAC.push_back(inst);
		}

	}


	if (name == "shift_expr")
	{
					
		/*

			shift_expr: shift_expr OP_BIT_LS sum 
				| shift_expr OP_BIT_RS sum 
				| sum

		*/

		

		tmp = children[0] -> tmp;

		if (children.size() == 3)
		{
			code inst;
			inst.field_1 = tmp;
			inst.field_2 = children[0] -> tmp;
			inst.field_3 = children[1] -> name;
			inst.field_4 = children[2] -> tmp;

			threeAC.push_back(inst);
		}

	}


	if (name == "sum")
	{
					
		/*

			sum: sum OP_ATH_ADD term 
				| sum OP_ATH_SUB term 
				| term

		*/

		

		tmp = children[0] -> tmp;

		if (children.size() == 3)
		{
			code inst;
			inst.field_1 = tmp;
			inst.field_2 = children[0] -> tmp;
			inst.field_3 = children[1] -> name;
			inst.field_4 = children[2] -> tmp;

			threeAC.push_back(inst);
		}

	}


	if (name == "term")
	{
					
		/*

			term: term OP_ATH_MUL factor 
				| term OP_ATH_DIV factor 
				| term OP_ATH_FDIV factor 
				| term OP_ATH_MOD factor 
				| factor

		*/

		

		tmp = children[0] -> tmp;

		if (children.size() == 3)
		{
			code inst;
			inst.field_1 = tmp;
			inst.field_2 = children[0] -> tmp;
			inst.field_3 = children[1] -> name;
			inst.field_4 = children[2] -> tmp;

			threeAC.push_back(inst);
		}

	}


	if (name == "factor")
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
			code inst;
			inst.field_1 = newTmp();
			inst.field_2 = children[0] -> name;
			inst.field_3 = children[1] -> tmp;

			threeAC.push_back(inst);
		}

	}


	if (name == "power")
	{
					
		/*

			power: primary OP_ATH_POW factor 
				| primary

		*/

		

		tmp = children[0] -> tmp;

		if (children.size() == 3)
		{
			code inst;
			inst.field_1 = tmp;
			inst.field_2 = children[0] -> tmp;
			inst.field_3 = children[1] -> name;
			inst.field_4 = children[2] -> tmp;

			threeAC.push_back(inst);
		}

	}


	if (name == "function_call")
	{
					
		/*

			function_call: NAME DLM_LFT_PRN args DLM_RGT_PRN

		*/
	}


	if (name == "args")
	{
					
		/*

			args:
				| expressions

		*/
	}


	if (name == "list_access")
	{
					
		/*

			list_access: NAME DLM_LFT_SQ expression DLM_RGT_SQ

		*/



	}

	if (name == "l_primary")
	{
					
		/*

			l_primary:  NAME DLM_DOT NAME 
				| list_access DLM_DOT NAME
				| NAME DLM_DOT list_access
				| list_access 
				| atom

		*/
	}

	if (name == "primary")
	{
					
		/*

			primary: l_primary
				| list_access DLM_DOT function_call
				| NAME DLM_DOT function_call
				| function_call

		*/
	
		tmp = children[0]->tmp;

	}


	if (name == "atom")
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

		if (children[0]->type != "NON_TERMINAL")
		{
			tmp = newTmp();
			code inst;
			inst.field_1 = tmp;
			inst.field_2 = children[0] -> name;
			threeAC.push_back(inst);
		}

		else
		{
			tmp = children[0] -> tmp;
		}

	}


	if (name == "group")
	{
					
		/*

			group: DLM_LFT_PRN expression DLM_RGT_PRN 

		*/

		tmp = children[1] -> tmp;

	}


	if (name == "string")
	{
					
		/*

			string: STRING_LITERAL              
			*/

		tmp = newTmp();
		code inst;
		inst.field_1 = tmp;
		inst.field_2 = children[0] -> name;
		threeAC.push_back(inst);

	}


	if (name == "strings")
	{
					
		/*

			strings: strings string
				| string

		*/

		if (children.size() == 1)
		{
			tmp = children[0] -> tmp;
		}

		else 
		{
			tmp = children[0] -> tmp + children[1] -> tmp;
		}

	}

	if (name == "list_expr")
	{
					
		/*

			list_expr: expressions 

		*/
	}


	if (name == "list")
	{
					
		/*

			list: DLM_LFT_SQ list_expr DLM_RGT_SQ 

		*/

		

	}

}