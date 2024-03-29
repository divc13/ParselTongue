#include<bits/stdc++.h>
using namespace std;


struct node;

typedef struct parasite{

	string trueL = "";
	string falseL = "";
	string next = "";
	string name = "";
	string type = "";
	string tmp = "";
	struct node* host;
	vector <struct parasite*> children;
	void genAC();
	parasite(struct node* __host);
	void make_ptree_debug(string out);

} Parasite;

typedef struct address_code {

	string label = "";
	string field_1 = "";
	string field_2 = "";
	string field_3 = "";
	string field_4 = "";
	string field_5 = "";

} code;

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
	Parasite* make_tree();
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
