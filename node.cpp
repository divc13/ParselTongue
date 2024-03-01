#include "node.hpp"

// root of the parse tree
TreeNode* root;

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

void SwitchChildToParent(TreeNode* parent, int nchild)
// child must be a terminal symbol, nchild is 0-indexed
{
	vector<TreeNode*> &children = parent->children;
	TreeNode* child = children[nchild];
	int itr = 0;
	for(auto &i : children)
	{
		if(i != child)
		{
			
		}
	}
}

void SkipChild(TreeNode* parent, int nchild)
// n-child is zero-indexed
{
	vector<TreeNode*> &children = parent->children;
	TreeNode* child = children[nchild];
	int itr = 0;
	for(auto i:(child->children))
	{
		itr++;
		children.insert(children.begin() + nchild + itr, i);
	}
	children.erase(children.begin() + nchild);
	return;
}

void AST_Maker(TreeNode* root)
{
	
}

