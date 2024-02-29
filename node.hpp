#include<bits/stdc++.h>
using namespace std;

struct node{
    int token;
    string name;
    string type;
	vector <struct node> children;
};

// root of the parse tree
struct node* root;