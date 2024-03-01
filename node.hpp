#include<bits/stdc++.h>
using namespace std;

typedef struct node{
	string name;
	string type;
	vector <struct node*> children;
	node(string __name, string __type);
	node(string __name);
} TreeNode;