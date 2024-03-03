#include "include/node.hpp"

// root of the parse tree / AST
TreeNode* root;

set<string> SkipToken2({
	"arguments",	// what to do in function
	"statement",
	"block",
	"slices",
	"bitwise_operators",
	// "expressions",
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
	"in",		
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
	"expression",
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
});

TreeNode::node(string __name, string __type)
{
	name = __name;
	type = __type;
}

void TreeNode::make_dot(string out)
{
	ofstream DOT(out);

	if (!DOT.is_open()) {
		cerr << "Error: Unable to open file " << out << endl;
		return;
	}

	DOT << "digraph Tree {\nnode [style = radial]\nedge [arrowhead=onormal]\nnodesep=2;\nranksep=5;\n";
	// DOT << "digraph Tree {\n";
	
	queue<pair<int, TreeNode*>> NodeQueue;
	NodeQueue.push({0, root});
	int index = 1;

	while(!NodeQueue.empty()) {
		int current_index = NodeQueue.front().first;
		TreeNode *node = NodeQueue.front().second;
		NodeQueue.pop();
		for(int i = 0; i < (node->children).size(); i++, index++) {
			DOT << current_index << " -> " << index << ";\n";
			NodeQueue.push({index, (node->children)[i]});
		}
	}

	NodeQueue.push({0, root});
	index = 1;
	while(!NodeQueue.empty()) {
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
		else if (node->type == "STRING_LITERAL") {
			DOT << node->type + "\n(";
			for (char c : node->name) {
				if (c == '"') {
					DOT << '\\' << '\"';
				} else {
					DOT << c;
				}
			}
			DOT << ")\", shape = rectangle, fillcolor=\"white:lightgreen";
		}
		else {
			DOT << tmp << "\", shape = rectangle, fillcolor=\"white:";


			if (node->type == "DEDENT") 					DOT << "pink";
			else if (node->type == "EOF") 					DOT << "pink";
			else if (node->type == "NEWLINE") 				DOT << "pink";
			else if (node->type == "INDENT") 				DOT << "pink";
			else if (node->type == "KEYWORD") 				DOT << "orchid1";
			else if (node->type == "OPERATOR") 				DOT << "lightcoral";
			else if (node->type == "IDENTIFIER") 			DOT << "slategray1";
			else if (node->type == "INT_LITERAL") 			DOT << "lightgreen";
			else if (node->type == "FLOAT_LITERAL") 		DOT << "lightgreen";
			else if (node->type == "DELIMITER") 			DOT << "pink";

		}
		
		DOT << "\" ];\n";

		for(int i = 0; i < (node->children).size(); i++, index++) {
			NodeQueue.push({index, (node->children)[i]});
		}
	}
	
	DOT << "}";
	DOT.close();
}

void ExchangeWithChild(TreeNode* root, int nchild)
{
	vector<TreeNode*> children = root->children;
	TreeNode* child = children[nchild];
	*root = *child;
	children.erase(children.begin() + nchild);
	root -> children = children;
	delete child;
	return;
}

void ConstrainedExchange(TreeNode* root, int &nchild, int args)
{
	vector<TreeNode*> &children = root->children;
	TreeNode* child = children[nchild];
	if (nchild + 1 >= (root->children).size()) return;
	(child->children).insert((child->children).begin(), children[nchild + 1]);
	children.erase(children.begin() + nchild + 1);
	if(args == 2)
	{
		cout << children[nchild]->name << endl;
		if (nchild < 1) return;
		(child->children).insert((child->children).begin(), children[nchild - 1]);
		children.erase(children.begin() + nchild - 1);
		nchild--;
		cout << children[nchild]->name << endl;
	}
	nchild++;
	return;
}

void SkipNode(TreeNode* root, int nchild)
{
	vector<TreeNode*> children = root->children;
	TreeNode* child = children[nchild];
	vector<TreeNode*> newChildren = child->children;
	children.insert(children.begin() + nchild, newChildren.begin(), newChildren.end());
	children.erase(children.begin() + nchild + newChildren.size());
	root -> children = children;
	delete child;
	return;
}

void generateAST(map<TreeNode*, bool> &visited, TreeNode* root, int flag)
{
	vector<TreeNode*> &children = root -> children;
	for(int nchild = 0; nchild < (root -> children).size();)
	{
		TreeNode* child = children[nchild];
		if(visited[child] == true)
		{
			nchild++;
			continue;
		}

		// first iteration

		// remove all non terminals with one child
		if((child->children).size() == 1 && SkipToken1.find(child->name) != SkipToken1.end() && (child->type).compare("IDENTIFIER") != 0 && flag == 0)
		{
			SkipNode(root, nchild);
			continue;
		}

		// second iteration
		
		// handle colon in different cases, if used for expression, bring up, otherwise skip
		if((child->name).compare(":") == 0 && flag == 1)
		{
			if ((root->name).compare("typedecl") == 0 || (root->name).compare("annotation") == 0 && (child->type).compare("IDENTIFIER") != 0) ExchangeWithChild(root, nchild);
			else SkipNode(root, nchild);
			continue;
		}


		// bring operators, dot and to symbol one level up
		if(((child->type).compare("OPERATOR") == 0 || (child->name).compare(".") == 0 || (child->name).compare("->") == 0) && flag == 1 && SkipToken1.find(root->name) != SkipToken1.end() && (root->type).compare("IDENTIFIER") != 0)
		{
			ExchangeWithChild(root, nchild);
			continue;
		}

		// perform constrained exchang, if the root nonterminal cannot be replaced
		if(((child->type).compare("OPERATOR") == 0 || (child->name).compare(".") == 0) && flag == 1 && (root->name).compare("bitwise_operators") != 0)
		{
			cout << child->name<< " , "<< root->name << " , " << (root->children).size() << endl;
			if ((child->name).compare("not") == 0 || (child->name).compare("~") == 0) ConstrainedExchange(root, nchild, 1);
			else if(nchild > 1)ConstrainedExchange(root, nchild, 2);
			continue;
		}

		// handle function call and array access
		if((root->name).compare("operand") == 0 && (nchild == 0) && (child->type).compare("IDENTIFIER") == 0 && flag == 1)
		{
			ExchangeWithChild(root, nchild);
			continue;
		}

		// third iteration

		// skip all unnecessary nonterminal symbols
		if((child->type).compare("IDENTIFIER") != 0 && SkipToken1.find(child->name) != SkipToken1.end() && flag == 2)
		{
			SkipNode(root, nchild);
			continue;
		}

		// fourth iteration

		// skip statements block and args with only one child 
		if((child->type).compare("IDENTIFIER") != 0 && SkipToken2.find(child->name) != SkipToken2.end() && (child->children).size() == 1 && flag == 3)
		{
			SkipNode(root, nchild);
			continue;
		}

		visited[child] = true;
		generateAST(visited, child, flag);
		
		nchild++;
	}
}

void AST_Maker(TreeNode* root)
{
	map<TreeNode*, bool> visited;
	visited[root] = true;
	generateAST(visited, root, 0);
	visited.clear();
	visited[root] = true;
	generateAST(visited, root, 1);
	visited.clear();
	visited[root] = true;
	generateAST(visited, root, 2);
	visited.clear();
	visited[root] = true;
	generateAST(visited, root, 3);
	return;
}
