#include<bits/stdc++.h>
using namespace std;

typedef struct node{
	string name;
	string type;
	int lineno;
	int column;
	int dataType = 0;
	vector <struct node*> children;
	node(string __name, int __lineno = 0, int __column = 0, string __type = "NON_TERMINAL");
	static void make_dot(string out);
} TreeNode;


// replace parent to one of the child, making the siblings of the corrsponding child as its children
// nchild is 0 indexed
// child must be a terminal symbol
void ExchangeWithChild(TreeNode* root, int nchild);

// skip the node and bring the children of the node to one level up, while maintaing ordering
// nchild is zero indexed
void SkipNode(TreeNode* root, int nchild);

// make sibling nodes of nchild its children without bring up the nchild
void ConstrainedExchange(TreeNode* root, int &nchild, int args);

// make AST from the parse tree
void AST_Maker(TreeNode* root);
