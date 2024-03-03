#include<bits/stdc++.h>
using namespace std;

typedef struct node{
	string name;
	string type;
	vector <struct node*> children;
	node(string __name, string __type = "NON_TERMINAL");
	static void make_dot(string out);
} TreeNode;


// replace parent to one of the child, making the sibling so f the corrsponding child as its children
// nchild is 0 indexed
// child must be a terminal symbol
void ExchangeWithChild(TreeNode* root, int nchild);

// skip the node and bring the children of the node to one level up, while maintaing ordering
// nchild is zero indexed
void SkipNode(TreeNode* root, int nchild);


// make AST from the parse tree
void AST_Maker(TreeNode* root);