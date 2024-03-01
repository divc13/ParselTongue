#include "node.hpp"

// root of the parse tree
TreeNode* root;

TreeNode::node(string __name, string __type)
{
    name = __name;
    type = __type;
}

TreeNode::node(string __name)
{
    name = __name;
}