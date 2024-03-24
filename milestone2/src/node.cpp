#include "include/node.hpp"
#include "include/symtable.hpp"
// #include "include/error.hpp"

#define SIZE_INT 4
#define SIZE_FLOAT 8
#define SIZE_BOOL 1
#define SIZE_STRING(x) (1 + (x))
#define SIZE_LIST(x) (8 * ((x)-2))

// root of the parse tree / AST
TreeNode *root;
symbolTable *currTable = new symbolTable("__GLOBAL__", NULL);
symbolTable *globTable = currTable;

set<string> SkipToken2({
	"arguments", // what to do in function
	"statement",
	"block",
	"slices",
	"bitwise_operators",
	"kwargs",
	"expression",
});

// Non-Terminals to remove from parse tree to form AST
set<string> SkipToken1({

	",",
	";",
	// "->",
	// ":",
	"NEWLINE",
	"EOF",
	"INDENT",
	"DEDENT",
	"return",
	"global",
	"nonlocal",
	"assert",
	"class",
	// "in",
	"def",
	"for",
	"while",
	"if",
	// "file",
	"statements",
	// "statement",
	"simple_stmts",
	"simple_stmt",
	"compound_stmt",
	"simple1",
	"simple2",
	"assignment",
	// "return_stmt",
	// "raise_stmt",
	// "assert_stmt",
	// "global_stmt",
	// "nonlocal_stmt",
	"multi_targets_assgn",
	"augassign",
	"names",
	// "block",
	// "class_def",
	// "function_def",
	"is_params",
	"is_arguments",
	"is_fn_expression",
	// "params",
	"param_nd",
	"param_wd",
	"slashed_param_nd",
	"slashed_param_wd",
	"star_etc",
	"sparam_maybe_default",
	"param_no_default",
	"param_with_default",
	"param_maybe_default",
	"param_mb_star",
	"param",
	"annotation",
	"default",
	// "if_stmt",
	// "elif_stmt",
	// "else_block",
	// "while_stmt",
	// "for_stmt",
	"expressions",
	"expressions_comma",
	// "expression",
	"disjunction",
	"conjunction",
	"inversion",
	"comparison",
	"bitwise_operator",
	// "bitwise_operators",
	"bitwise_xor",
	"bitwise_or",
	"bitwise_and",
	"shift_expr",
	"sum",
	"term",
	"factor",
	"power",
	"primary",
	// "slices",
	"slices_comma",
	"slice",
	"atom",
	// "group",
	"string",
	"strings",
	// "list",
	// "arguments",
	// "kwargs",
	"kwarg",
	"typedecl",
	"else",
	"elif",
	// "if_expression",
	// "operand",
	"param_default",
});

TreeNode::node(string __name, int __lineno, int __column, string __type)
{
	name = __name;
	type = __type;
	lineno = __lineno;
	column = __column;
}

void TreeNode::make_dot(string out)
{
	ofstream DOT(out);

	if (!DOT.is_open())
	{
		cerr << "Error: Unable to open file " << out << endl;
		return;
	}

	DOT << "digraph Tree {\nnode [style = radial]\nedge [arrowhead=onormal]\nnodesep=2;\nranksep=5;\n";
	// DOT << "digraph Tree {\n";

	queue<pair<int, TreeNode *>> NodeQueue;
	NodeQueue.push({0, root});
	int index = 1;

	while (!NodeQueue.empty())
	{
		int current_index = NodeQueue.front().first;
		TreeNode *node = NodeQueue.front().second;
		NodeQueue.pop();
		for (int i = 0; i < (node->children).size(); i++, index++)
		{
			DOT << current_index << " -> " << index << ";\n";
			NodeQueue.push({index, (node->children)[i]});
		}
	}

	NodeQueue.push({0, root});
	index = 1;
	while (!NodeQueue.empty())
	{
		int current_index = NodeQueue.front().first;
		TreeNode *node = NodeQueue.front().second;
		NodeQueue.pop();
		string tmp;

		if (node->type == "NON_TERMINAL")
			tmp = node->name;
		else if (node->type == "DEDENT" || node->type == "EOF" || node->type == "NEWLINE" || node->type == "INDENT")
			tmp = node->type;
		else
			tmp = node->type + "\n(" + node->name + ")";

		DOT << current_index << " [ label = \"";

		if (node->type == "NON_TERMINAL")
			DOT << tmp << "\", fillcolor=\"white:bisque";
		else if (node->type == "STRING_LITERAL")
		{
			DOT << node->type + "\n(";
			for (char c : node->name)
			{
				if (c == '"')
				{
					DOT << '\\' << '\"';
				}
				else if (c == '\\')
				{
					DOT << '\\' << '\\';
				}
				else
				{
					DOT << c;
				}
			}
			DOT << ")\", shape = rectangle, fillcolor=\"white:lightgreen";
		}
		else
		{
			DOT << tmp << "\", shape = rectangle, fillcolor=\"white:";

			if (node->type == "DEDENT")
				DOT << "pink";
			else if (node->type == "EOF")
				DOT << "pink";
			else if (node->type == "NEWLINE")
				DOT << "pink";
			else if (node->type == "INDENT")
				DOT << "pink";
			else if (node->type == "KEYWORD")
				DOT << "orchid1";
			else if (node->type == "OPERATOR")
				DOT << "lightcoral";
			else if (node->type == "IDENTIFIER")
				DOT << "slategray1";
			else if (node->type == "INT_LITERAL")
				DOT << "lightgreen";
			else if (node->type == "FLOAT_LITERAL")
				DOT << "lightgreen";
			else if (node->type == "DELIMITER")
				DOT << "pink";
		}

		DOT << "\" ];\n";

		for (int i = 0; i < (node->children).size(); i++, index++)
		{
			NodeQueue.push({index, (node->children)[i]});
		}
	}

	DOT << "}";
	DOT.close();
}

void ExchangeWithChild(TreeNode *root, int nchild)
{
	vector<TreeNode *> children = root->children;
	TreeNode *child = children[nchild];
	*root = *child;
	children.erase(children.begin() + nchild);
	root->children = children;
	delete child;
	return;
}

void ConstrainedExchange(TreeNode *root, int &nchild, int args)
{
	vector<TreeNode *> &children = root->children;
	TreeNode *child = children[nchild];
	if (nchild + 1 >= (root->children).size())
		return;
	(child->children).insert((child->children).begin(), children[nchild + 1]);
	children.erase(children.begin() + nchild + 1);
	if (args == 2)
	{
		if (nchild < 1)
			return;
		(child->children).insert((child->children).begin(), children[nchild - 1]);
		children.erase(children.begin() + nchild - 1);
		nchild--;
	}
	nchild++;
	return;
}

void SkipNode(TreeNode *root, int nchild)
{
	vector<TreeNode *> children = root->children;
	TreeNode *child = children[nchild];
	vector<TreeNode *> newChildren = child->children;
	children.insert(children.begin() + nchild, newChildren.begin(), newChildren.end());
	children.erase(children.begin() + nchild + newChildren.size());
	root->children = children;
	delete child;
	return;
}

void generateAST(TreeNode *root, int flag)
{
	vector<TreeNode *> &children = root->children;
	for (int nchild = 0; nchild < (root->children).size();)
	{
		TreeNode *child = children[nchild];

		// first iteration

		// remove all non terminals with one child
		if ((child->children).size() == 1 && SkipToken1.find(child->name) != SkipToken1.end() && (child->type).compare("IDENTIFIER") != 0 && flag == 0)
		{
			SkipNode(root, nchild);
			continue;
		}

		// second iteration

		// handle colon in different cases, if used for expression, bring up, otherwise skip
		if ((child->name).compare(":") == 0 && flag == 1)
		{
			if ((root->name).compare("typedecl") == 0 || (root->name).compare("annotation") == 0 && (child->type).compare("IDENTIFIER") != 0)
				ExchangeWithChild(root, nchild);
			else
				SkipNode(root, nchild);
			continue;
		}

		// bring operators, dot and to symbol one level up
		if (((child->type).compare("OPERATOR") == 0 || (child->name).compare(".") == 0 || (child->name).compare("in") == 0 || (child->name).compare("->") == 0) && flag == 1 && SkipToken1.find(root->name) != SkipToken1.end() && (root->type).compare("IDENTIFIER") != 0)
		{
			ExchangeWithChild(root, nchild);
			continue;
		}

		// perform constrained exchang, if the root nonterminal cannot be replaced
		if (((child->type).compare("OPERATOR") == 0 || (child->name).compare(".") == 0 || (child->name).compare("in") == 0) && flag == 1 && (root->name).compare("bitwise_operators") != 0)
		{
			if ((child->name).compare("not") == 0 || (child->name).compare("~") == 0)
				ConstrainedExchange(root, nchild, 1);
			else if (nchild > 1)
				ConstrainedExchange(root, nchild, 2);
			continue;
		}

		// handle function call and array access
		if ((root->name).compare("operand") == 0 && (nchild == 0) && (child->type).compare("IDENTIFIER") == 0 && flag == 1)
		{
			ExchangeWithChild(root, nchild);
			continue;
		}

		// third iteration

		// skip all unnecessary nonterminal symbols
		if ((child->type).compare("IDENTIFIER") != 0 && SkipToken1.find(child->name) != SkipToken1.end() && flag == 2)
		{
			SkipNode(root, nchild);
			continue;
		}

		// fourth iteration

		// skip statements block and args with only one child
		if ((child->type).compare("IDENTIFIER") != 0 && SkipToken2.find(child->name) != SkipToken2.end() && (child->children).size() == 1 && flag == 3)
		{
			SkipNode(root, nchild);
			continue;
		}

		generateAST(child, flag);

		nchild++;
	}
}

void AST_Maker(TreeNode *root)
{
	generateAST(root, 0);
	generateAST(root, 1);
	generateAST(root, 2);
	generateAST(root, 3);
	return;
}

int generate_symtable(TreeNode *root, tableRecord* &record)
{

	// handle functions
	if ((root->type).compare("NON_TERMINAL") == 0 && (root->name).compare("function_def") == 0)
	{
		// a function definition has 6 children
		assert((root->children).size() == 6);

		symbolTable *Table = new symbolTable(((root->children)[0])->name, currTable);
		currTable = Table;
		currTable->offset = (((root->children)[2])->children).size();
	}

	// // handle classes
	// if ((root->type).compare("NON_TERMINAL") == 0 && (root->name).compare("function_def") == 0)
	// {

	// 	symbolTable *Table = new symbolTable(((root->children)[0])->name, currTable);
	// 	currTable = Table;

	// }

	// apply dfs here
	vector<TreeNode *> &children = root->children;
	for (int nchild = 0; nchild < (root->children).size(); nchild++)
	{
		int ret = generate_symtable(children[nchild], record);

		if (ret < 0)
			return ret;
	}

	// constant integers
	if ((root->type).compare("INT_LITERAL") == 0)
	{
		tableRecord* tempRecord = record;
		record = new tableRecord(root->name, "int", SIZE_INT, root->lineno, root->column);
		if(!globTable->lookup(record->name, record->lineno, record->column, false))
			globTable->insert(record, NULL);
		free (record);
		record = tempRecord;
		return 0;
	}

	// constant floating point literals
	if ((root->type).compare("FLOAT_LITERAL") == 0)
	{
		tableRecord* tempRecord = record;
		record = new tableRecord(root->name, "float", SIZE_FLOAT, root->lineno, root->column);
		if(!globTable->lookup(record->name, record->lineno, record->column, false))
			globTable->insert(record, NULL);
		free (record);
		record = NULL;
		record = tempRecord;
		return 0;
	}

	// constant string literals
	if ((root->type).compare("STRING_LITERAL") == 0)
	{
		tableRecord* tempRecord = record;
		root->name = (root->name).substr(1, (root->name).length() - 2);
		record = new tableRecord(root->name, "str", SIZE_STRING((root->name).length()), root->lineno, root->column);
		if(!globTable->lookup(record->name, record->lineno, record->column, false))
			globTable->insert(record, NULL);
		free (record);
		record = NULL;
		record = tempRecord;
		return 0;
	}

	// handle type declarations
	if ((root->type).compare("DELIMITER") == 0 && (root->name).compare(":") == 0)
	{
		// colon will have exactly 2 chidren
		assert((root->children).size() == 2);

		TreeNode* node = ((root->children)[0]);
		record = new tableRecord(node->name, "", 0, node->lineno, node->column);

		node = (root->children)[1];
		// type of list constructed here
		if ((node->name).compare("list") == 0)
		{
			// some explicit type inside list 
			assert((node->children).size() > 2);

			string type = node->name + "[" + (node->children)[1]->name + "]";
			record->type = type;
		}

		else
		{
			record->type = ((root->children)[1])->name;

			if ((record->type).compare("int") == 0)
			{
				record->size = SIZE_INT;
			}

			else if ((record->type).compare("float") == 0)
				record->size = SIZE_FLOAT;

			else if ((record->type).compare("bool") == 0)
				record->size = SIZE_BOOL;

			// dealing with classes
			else if ((record->type).compare("str") && (record->type).compare(0, 4, "list"))
			{
				tableRecord *entry = currTable->lookup(record->type, record->lineno, record->column, true);
				if (!entry)
				{
					return -1;
				}
				record->size = entry->size;
			}

		}

		int err = currTable->insert(record, NULL);
		// cout <<"name = " << record->name << " type = " << record->type << " size = " << record->size << "lineno = " << record->lineno << endl;
		
		if (err < 0)
			return err;

		if (record->size)
		{
			free(record);
			record = NULL;
		}

		return 0;
	}

	// Update the sizes of the records, in case of lists and strings
	if ((root->type).compare("OPERATOR") == 0 && (root->name).compare("=") == 0)
	{
		
		if (record)
		{
			if (!(record->size))
			{
				
				// = will have only 2 child
				assert((root->children).size() == 2);

				TreeNode* node = (root->children)[1];
				if ((record->type).compare(0, 4, "list") == 0)
				{
					int num = (node->children).size() - 2;
					string type = record->type;

					// remove the characters l i s t [ ] = 6
					string category = type.substr(5, type.length() - 6);

					if (category.compare("int") == 0)
						record->size = num * SIZE_INT;

					else if (category.compare("float") == 0)
						record->size = num * SIZE_FLOAT;

					else if (category.compare("bool") == 0)
						record->size = num * SIZE_BOOL;

					else if (category.compare("str") == 0)
					{
						for(auto &c: node->children)
						{
							record->size += SIZE_STRING((c->name).length());
						}
					}
						
					// class objects
					else
					{
						tableRecord *entry = currTable->lookup(record->type, record->lineno, record->column, true);
						if (!entry)
						{
							return -1;
						}
						record->size = num * entry->size;
					}
					
				}

				else if ((record->type).compare("str") == 0)
					record->size = SIZE_STRING((node->name).length());

				int ret = currTable->UpdateRecord(record);
				if (ret < 0)
					return ret;

			}

			free(record);
			record = NULL;
		}

		return 0;
	}

	// dealing with functions again, after completeing the function symbol table
	if ((root->type).compare("NON_TERMINAL") == 0 && (root->name).compare("function_def") == 0)
	{
		TreeNode* node = ((root->children)[0]);
		string type = (((root->children)[4])->children)[0]->name;
		record = new tableRecord(node->name, type, currTable->size, node->lineno, node->column);

		int err = currTable->parentSymtable->insert(record, currTable);
		if (err < 0)
			return err;

		currTable = currTable->parentSymtable;

		free(record);
		record = NULL;
	}

	return 0;
}

void symTable_Maker(TreeNode *root)
{
	tableRecord* record = NULL;
	generate_symtable(root, record);
}
