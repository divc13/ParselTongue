#include "node.hpp"

// root of the parse tree
TreeNode* root;

TreeNode::node(string __name, string __type)
{
	name = __name;
	type = __type;
}

ofstream DOT("dot.dot");

void TreeNode::make_dot()
{
	DOT << "digraph G {\n";
	
	queue<pair<int, TreeNode*>> NodeQueue;
	map<string, vector<int>> NodeMap;
	NodeQueue.push({0, root});
	int index = 1;

	while(!NodeQueue.empty()) {
		int current_index = NodeQueue.front().first;
		TreeNode *node = NodeQueue.front().second;
		NodeQueue.pop();
		NodeMap[node->type].push_back(current_index);
		for(int i = 0; i < (node->children).size(); i++, index++) {
			DOT << current_index << " { label = \"" << (node->type == "NON_TERMINAL" ? node->name : node->type) << "(" << node->name <<")" << "\";}" << " -> " << index << " { label = \"" << (((node->children)[i])->type == "NON_TERMINAL" ? ((node->children)[i])->name : ((node->children)[i])->type) << "(" << ((node->children)[i])->name <<")" << "\";}" << ";\n";
			NodeQueue.push({index, (node->children)[i]});
		}
	}

	// for (auto it : NodeMap) {
    //     const string& nodeType = it.first;
    //     const vector<int>& nodeIndices = it.second;
        
	// 	if (nodeType != "NON_TERMINAL")
	// 		DOT << "node [ shape = rectangle, color = ";
	// 	else
	// 		DOT << "node [ color = ";

	// 	if(nodeType == "DEDENT") 
	// 		DOT << "<<color>>";
	// 	else if(nodeType == "EOF") 
	// 		DOT << "<<color>>";
	// 	else if(nodeType == "NEWLINE") 
	// 		DOT << "<<color>>";
	// 	else if(nodeType == "INDENT") 
	// 		DOT << "<<color>>";
	// 	else if(nodeType == "KEYWORD") 
	// 		DOT << "<<color>>";
	// 	else if(nodeType == "OPERATOR") 
	// 		DOT << "<<color>>";
	// 	else if(nodeType == "IDENTIFIER") 
	// 		DOT << "<<color>>";
	// 	else if(nodeType == "INT_LITERAL") 
	// 		DOT << "<<color>>";
	// 	else if(nodeType == "FLOAT_LITERAL") 
	// 		DOT << "<<color>>";
	// 	else if(nodeType == "STRING_LITERAL") 
	// 		DOT << "<<color>>";
	// 	else if(nodeType == "DELIMITER") 
	// 		DOT << "<<color>>";
	// 	else if(nodeType == "NON_TERMINAL") 
	// 		DOT << "<<color>>";

	// 	DOT << " ]; ";
		
	// 	for(auto i : nodeIndices)
	// 		DOT << i << " ";

	// 	DOT << ";\n";
    // }
	DOT << "}";
}
