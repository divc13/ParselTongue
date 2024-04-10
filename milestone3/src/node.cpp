#include "include/symtable.hpp"

bool isOperator(TreeNode* root)
{
	if ((root->type).compare(0, 3, "OP_"))
		return false;
	return true;
}

// root of the parse tree / AST
TreeNode *root;
Parasite *parasiticRoot;
extern map<string, string> opType;

set<string> SkipToken2({
	"arguments",
	"statement",
	"block",
	"slices",
	"bitwise_operators",
	"kwargs",
	"expression",
	"list_expr",
	"l_primary",
	"class_access",
});

// Non-Terminals to remove from parse tree to form AST
set<string> SkipToken1({

	// typedecl
	// list_access 
	// function_call
	// args
	// "->",
	// ":",
	// "in",
	// "file",
	// "statement",
	// "raise_stmt",
	// "assert_stmt",
	// "nonlocal_stmt",
	// "block",
	// "class_def",
	// "function_def",
	// "params",
	// "bitwise_operators",
	// "slices",
	// "list",
	// "kwargs",
	// "expression",

	// "for_stmt",
	// "if_stmt",
	// "elif_stmt",
	// "else_block",
	// "while_stmt",
	// "group",
	// "global_stmt",
	// "return_stmt",

	// "typedecl",
	// "for_expr",

	"list_expr",
	"arguments",
	",",
	";",
	"NEWLINE",
	"EOF",
	"INDENT",
	"DEDENT",
	"return",
	"global",
	"nonlocal",
	"assert",
	"class",
	"def",
	"for",
	"while",
	"if",
	"statements",
	"simple_stmts",
	"simple_stmt",
	"compound_stmt",
	"simple1",
	"simple2",
	"assignment",
	"multi_targets_assgn",
	"augassign",
	"names",
	"is_params",
	"is_arguments",
	"is_fn_expression",
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
	"expressions",
	"expressions_comma",
	"disjunction",
	"conjunction",
	"inversion",
	"comparison",
	"bitwise_operator",
	"bitwise_xor",
	"bitwise_or",
	"bitwise_and",
	"shift_expr",
	"sum",
	"term",
	"factor",
	"power",
	"primary",
	"slices_comma",
	"slice",
	"atom",
	"string",
	"strings",
	"kwarg",
	"else",
	"elif",
	"operand",
	"param_default",
	"l_primary",
	"class_access",
});

TreeNode::node(string __name, int __lineno, int __column, string __type)
{
	name = __name;
	type = __type;
	lineno = __lineno;
	column = __column;
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

// just for bitwise_operators pair (is not, not in)
void ConstrainedExchange_bitwise(TreeNode *root, int &nchild, int args)
{
	vector<TreeNode *> &children = root->children;
	TreeNode *child = children[nchild];
	if (nchild + 1 >= (root->children).size())
		return;
	(child->children).push_back(children[nchild + 1]);
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

TreeNode* copyTree(TreeNode* root)
{
	TreeNode* newNode = new TreeNode(root->name, root->lineno, root->column, root->type);
	for (auto child : root->children)
	{
		TreeNode* childNode = copyTree(child);
		(newNode->children).push_back(childNode);
	}
	return newNode;
}

void handle_augAssign(TreeNode* child)
{
	int len = (child->name).length();
	assert((child->children).size() == 2);
				
	TreeNode* node1 = (child->children)[0];
	TreeNode* node2 = (child->children)[1];

	string newOpName = (child->name).substr(0, len - 1);
	TreeNode* newOperator = new TreeNode(newOpName, child->lineno, child->column, opType[newOpName]);
	TreeNode* newOperand = copyTree(node1);
	
	(newOperator -> children).push_back(newOperand);
	(newOperator -> children).push_back((child->children)[1]);

	child -> name = "=";
	(child->children)[1] = newOperator;
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
		if (((child->name).compare(":") == 0) && flag == 1)
		{
			// if (((root->name).compare("typedecl") == 0 || (root->name).compare("annotation") == 0) && (child->type).compare("IDENTIFIER") != 0)
			// 	ExchangeWithChild(root, nchild);

			if (((root->name).compare("annotation") == 0) && (child->type).compare("IDENTIFIER") != 0)
				ExchangeWithChild(root, nchild);

			else if (((root->name).compare("typedecl") == 0 ) && (child->type).compare("IDENTIFIER") != 0)
				ConstrainedExchange(root, nchild, 2);
			else
				SkipNode(root, nchild);
			continue;
		}

		// bring operators, dot and to symbol one level up
		if ((isOperator(child) || (child->name).compare(".") == 0 || (child->name).compare("in") == 0 || (child->name).compare("->") == 0) && flag == 2 && SkipToken1.find(root->name) != SkipToken1.end() && (root->type).compare("IDENTIFIER") != 0)
		{
			ExchangeWithChild(root, nchild);
			continue;
		}

		// perform constrained exchang, if the root nonterminal cannot be replaced
		if ((isOperator(child) || (child->name).compare(".") == 0) && flag == 2)
		// if ((isOperator(child) || (child->name).compare(".") == 0 || (child->name).compare("in") == 0) && flag == 2)
		{
			if ((child->name).compare("not") == 0 || (child->name).compare("~") == 0)
				ConstrainedExchange(root, nchild, 1);
			else if (nchild > 1 || ((root -> name).compare("for_expr") == 0 && (child -> name).compare("in") == 0 ))
			{
				ConstrainedExchange(root, nchild, 2);
			}
			continue;
		}

		if (((child->name).compare("in") == 0) && flag == 3 && (root->type).compare("IDENTIFIER") != 0)
		{
			// if (SkipToken1.find(root -> name) == SkipToken1.end())
			if (root -> name == "for_expr" && root -> type == "NON_TERMINAL" && (root -> children).size() == 3)
			{
				ConstrainedExchange(root, nchild, 2);
				continue;
			}
			// else ExchangeWithChild(root, nchild);
		}

		// third iteration

		// skip all unnecessary nonterminal symbols
		if ((child->type).compare("IDENTIFIER") != 0 && SkipToken1.find(child->name) != SkipToken1.end() && flag == 4)
		{
			SkipNode(root, nchild);
			continue;
		}

		// fourth iteration

		// skip statements block and args with only one child
		if ((child->type).compare("IDENTIFIER") != 0 && SkipToken2.find(child->name) != SkipToken2.end() && (child->children).size() == 1 && flag == 5)
		{
			SkipNode(root, nchild);
			continue;
		}

		if (isOperator(child) && flag == 5)
		{
			int len = (child->name).length();
			if (opType[child -> name].compare("OP_ASSIGNMENT") == 0 && len > 1)
			{
				handle_augAssign(child);
				continue;
			}
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
	generateAST(root, 4);
	generateAST(root, 5);
	return;
}




Parasite::parasite(struct node* __host)
{
	host = __host;
	name = __host -> name;
	type = __host -> type;
	string trueL = "";
	string falseL = "";
	string next = "";
	string current = "";
	string tmp = "";
}

Parasite* TreeNode::make_tree()
{
	Parasite* root = new Parasite(this);
	for (auto child : children)
	{
		Parasite* parasitic_child = child -> make_tree();
		(root -> children).push_back(parasitic_child);
	}
	return root;
}

vector<string> StringToRemove;

void should_clear(Parasite* root)
{
	if (root->type != "NON_TERMINAL")
	{
		StringToRemove.push_back(root->name);
	}

	for (auto child : root->children)
		should_clear(child);
}

void clear_tree(Parasite* root)
{
	if (root->name == "file")
	{
		if ((root->children)[0]->name == "statements" && (((root->children)[0])->children).size() == 2)
		{
			should_clear(((root->children)[0]->children)[1]);
			if (StringToRemove[0] == "if" && StringToRemove[1] == "__name__" && StringToRemove[2] == "==" && StringToRemove[3] == "\"__main__\"")
			{
				((root->children)[0]->children).pop_back();
				(root->children).insert((root->children).begin(), ((root->children)[0]->children)[0]);
				(root->children).erase((root->children).begin() + 1);
			}
		}
	}
}