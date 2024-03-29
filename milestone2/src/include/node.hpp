#include<bits/stdc++.h>
using namespace std;


struct node;

typedef struct parasite{
	struct node* host;
	vector <struct parasite*> children;
	parasite(struct node* __host);
} ParasiticNode;



typedef struct node{
	string name;
	string type;
	int lineno;
	int column;
	string dataType = "";
	vector <struct node*> children;
	symbolTable* scope;
	node(string __name, int __lineno = 0, int __column = 0, string __type = "NON_TERMINAL");
	void make_dot(string out);
	void make_dot_debug(string out);
	ParasiticNode* make_tree();
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

bool isOperator(TreeNode* root);

void init_token_map();
