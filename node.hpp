#include<bits/stdc++.h>
using namespace std;

typedef struct node{
	string name;
	string type;
	vector <struct node*> children;
	node(string __name, string __type = "NON_TERMINAL");
	static void make_dot();
} TreeNode;

// nchild is 0 indexed
void SwitchChildToParent(TreeNode* parent, int nchild);

// nchild is zero indexed
void SkipChild(TreeNode* parent, int nchild);
void AST_Maker(TreeNode* root);