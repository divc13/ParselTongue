#include "include/node.hpp"

bool isOperator(TreeNode* root)
{
	if ((root->type).compare(0, 3, "OP_"))
		return false;
	return true;
}

// root of the parse tree / AST
TreeNode *root;
extern map<string, string> opType;

set<string> SkipToken2({
	// "arguments",
	"statement",
	"block",
	"slices",
	"bitwise_operators",
	"kwargs",
	"expression",
});

// Non-Terminals to remove from parse tree to form AST
set<string> SkipToken1({

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
	// "arguments",
	// "kwargs",


	// "if_stmt",
	// "elif_stmt",
	// "else_block",
	// "while_stmt",
	// "for_stmt",
	// "expression",
	// "group",
	// "if_expression",
	// "global_stmt",
	// "return_stmt", 

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
	"typedecl",
	"else",
	"elif",
	"operand",
	"param_default",
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

void handle_augAssign(TreeNode* child)
{
	int len = (child->name).length();
	assert((child->children).size() == 2);
				
	TreeNode* node1 = (child->children)[0];
	TreeNode* node2 = (child->children)[1];

	string newOpName = (child->name).substr(0, len - 1);
	TreeNode* newOperator = new TreeNode(newOpName, child->lineno, child->column, opType[newOpName]);
	TreeNode* newOperand = new TreeNode((child->children)[0]->name, (child->children)[0]->lineno, (child->children)[0]->column, (child->children)[0]->type);
	
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
		if (((child->name).compare(":") == 0 || (child->name).compare("bitwise_operators") == 0) && flag == 1)
		{
			if (((root->name).compare("typedecl") == 0 || (root->name).compare("annotation") == 0) && (child->type).compare("IDENTIFIER") != 0)
				ExchangeWithChild(root, nchild);
			else if((root->name).compare("comparison") == 0 && (child->name).compare("bitwise_operators") == 0)
				ConstrainedExchange_bitwise(root, nchild, 2);
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
		if ((isOperator(child) || (child->name).compare(".") == 0 || (child->name).compare("in") == 0) && flag == 2 && (root->name).compare("bitwise_operators") != 0)
		{
			if ((child->name).compare("not") == 0 || (child->name).compare("~") == 0)
				ConstrainedExchange(root, nchild, 1);
			else if (nchild > 1)
				ConstrainedExchange(root, nchild, 2);
			continue;
		}

		// handle function call and array access
		if ((root->name).compare("operand") == 0 && (nchild == 0) && (child->type).compare("IDENTIFIER") == 0 && flag == 2)
		{
			ExchangeWithChild(root, nchild);
			continue;
		}

		// third iteration

		// skip all unnecessary nonterminal symbols
		if ((child->type).compare("IDENTIFIER") != 0 && SkipToken1.find(child->name) != SkipToken1.end() && flag == 3)
		{
			SkipNode(root, nchild);
			continue;
		}

		// fourth iteration

		// skip statements block and args with only one child
		if ((child->type).compare("IDENTIFIER") != 0 && SkipToken2.find(child->name) != SkipToken2.end() && (child->children).size() == 1 && flag == 4)
		{
			SkipNode(root, nchild);
			continue;
		}

		if (isOperator(child) && flag == 4)
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
	return;
}
