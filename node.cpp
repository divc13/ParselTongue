#include "node.hpp"
// #include "token_map.hpp"

extern map<string, string> TokenNameToString;

// root of the parse tree
TreeNode* root;

// Terminals to remove from parse tree to form AST
set<string> UselessTerminals({

						

});

// Non-Terminals to remove from parse tree to form AST
set<string> SkipToken({

	",", 	
	";", 	
	"->",		
	":",		
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
	// "file",
	"statements",
	"statement",
	"simple_stmts",
	"simple_stmt",
	"compound_stmt",
	"simple1",
	"simple2",
	// "assignment",
	// "return_stmt",
	// "raise_stmt",
	// "assert_stmt",
	// "global_stmt",
	// "nonlocal_stmt",
	// "multi_targets_assgn",
	"augassign",
	"names",
	"block",
	// "class_def",
	// "function_def",
	"is_params",
	"is_arguments",
	"is_fn_expression",
	"params",
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
	"many_compare_op_bitwise_or_pairs",
	"compare_op_bitwise_or_pair",
	"eq_bitwise_or",
	"noteq_bitwise_or",
	"lte_bitwise_or",
	"lt_bitwise_or",
	"gte_bitwise_or",
	"gt_bitwise_or",
	"notin_bitwise_or",
	"in_bitwise_or",
	"isnot_bitwise_or",
	"is_bitwise_or",
	"bitwise_xor",
	"bitwise_or",
	"bitwise_and",
	"shift_expr",
	"sum",
	"term",
	"factor",
	"power",
	"primary",
	"slices",
	"slices_comma",
	"slice",
	"atom",
	"group",
	"string",
	"strings",
	"list",
	"arguments",
	"kwargs",
	"kwarg"
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

	DOT << "digraph Tree {\nnode [style = filled]\nnodesep=2;\nranksep=5;\n";
	DOT << "digraph Tree {\n";
	
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
			tmp = node->type + "(" + node->name + ")";

		DOT << current_index << " [ label = \"";

		if (node->type == "NON_TERMINAL")
			DOT << tmp << "\"";
		else if (node->type == "STRING_LITERAL") {
			DOT << node->type + "(";
			for (char c : node->name) {
				if (c == '"') {
					DOT << '\\' << '\"';
				} else {
					DOT << c;
				}
			}
			DOT << ")\", shape = rectangle";
		}
		else {
			DOT << tmp << "\"";
		}
		
		DOT << " ];\n";

		for(int i = 0; i < (node->children).size(); i++, index++) {
			NodeQueue.push({index, (node->children)[i]});
		}
	}

	
	DOT << "}";
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

void SkipNode(TreeNode* root, int nchild)
{
	vector<TreeNode*> &children = root->children;
	TreeNode* child = children[nchild];

	int itr = 0;
	for(auto i: (child->children))
	{
		itr++;
		cout << "-------------------\n";
		cout << i->name << "\n";
		cout << "-------------------\n";
		children.insert(children.begin() + nchild + itr, i);
	}
	children.erase(children.begin() + nchild);
	delete child;

	return;
}

void generateAST(map<TreeNode*, bool> &visited, TreeNode* root)
{
	vector<TreeNode*> children = root -> children;
	for(int nchild = 0; nchild < children.size();)
	{
		TreeNode* child = children[nchild];
		if(visited[child] == true)
		{
			nchild++;
			continue;
		}

		visited[child] = true;
		generateAST(visited, child);


		if(SkipToken.find(root->name) != SkipToken.end())
		{
			cout << "skip me " << endl;
			// SkipNode(root, nchild);

			continue;
		}
		
		if((child->type).compare("OPERATOR") == 0 && SkipToken.find(root->name) != SkipToken.end())
		{
			cout << "exchange me " << endl;
			ExchangeWithChild(root, nchild);
			continue;
		}

		nchild++;
	}
}

void AST_Maker(TreeNode* root)
{
	map<TreeNode*, bool> visited;
	visited[root] = true;
	generateAST(visited, root);
	return;
}
