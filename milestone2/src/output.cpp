#include "include/symtable.hpp"
#include "include/error.hpp"

extern map<int, string> recordTypeMap;

void print_name(ofstream &MD, string temp)
{
	size_t pos = 0;
    while((pos = temp.find("\\\r\n", pos)) != string::npos)
	{
        temp.replace(pos, 4, "\\\\<br>");
		pos++;
    }

	pos = 0;
    while((pos = temp.find("\\\r", pos)) != string::npos)
	{
        temp.replace(pos, 3, "\\\\<br>");
		pos++;
    }

	pos = 0;
    while((pos = temp.find("\\\n", pos)) != string::npos)
	{
        temp.replace(pos, 3, "\\\\<br>");
		pos++;
    }

	pos = 0;
    while((pos = temp.find("\n", pos)) != string::npos)
	{
        temp.replace(pos, 2, "<br>");
		pos++;
    }

	pos = 0;
    while((pos = temp.find("_", pos)) != string::npos)
	{
        temp.replace(pos, 1, "\\_");
		pos+=2;
    }

	MD << temp;
}

void tableRecord::dumpMD(ofstream &MD)
{
	MD << "| ";
	MD << index;
	MD << "  |  ";
	print_name(MD, name);
	MD << "  |  " << type;
	MD << "  |  " << recordTypeMap[recordType];
	MD << "  |  " << offset;
	MD << "  |  " << size;
	MD << "  |  " << lineno;
	MD << "  |" << endl;

	return;
}


void symbolTable::dumpMD(ofstream &MD)
{
    MD << "\n<br /><br />\n<span style=\"font-size: 25px;\">__Table Name: ";
    if (tableType == tableType::CLASS)
        MD << "Class ";
    if (tableType == tableType::FUNCTION)
        MD << "Function ";
    print_name(MD, name);
	MD << "__</span>\n";
	if (parentSymtable)
	{
		MD << " \n<span style=\"font-size: 20px;\">__Parent Table: ";
		print_name(MD, parentSymtable -> name);
        MD << "__</span>";
    }
	MD << endl;

	int index = 0;

	if (tableType == tableType::FUNCTION)
	{
		MD << "\n<span style=\"font-size: 20px;\">__Incoming Parameters:__</span> \n";
		MD << "|  index";
		MD << "  |  name";
		MD << "  |  type";
		MD << "  |  recordType";
		MD << "  |  offset";
		MD << "  |  size";
		MD << "  |  line no. |\n";
		MD << "|-|-|-|-|-|-|-|\n";
		for (; index < numParams; index++)
			(entries[index]) -> dumpMD(MD);
	}

	MD << "\n<span style=\"font-size: 20px;\">__Local Variables:__</span> \n";
	MD << "|  index";
	MD << "  |  name";
	MD << "  |  type";
	MD << "  |  recordType";
	MD << "  |  offset";
	MD << "  |  size";
	MD << "  |  line no. |\n";
	MD << "|-|-|-|-|-|-|-|\n";
	for (; index < currentIndex; index++)
		(entries[index]) -> dumpMD(MD);

	for(auto child : childIndices) 
	{
		MD << "\n\n\n";
		assert(entries[child] -> symTab);
		((entries[child]) -> symTab) -> dumpMD(MD);
	}

	return;
}

void tableRecord::dumpCSV(ofstream &CSV)
{
	CSV << index << ", " << name << ", " << type << ", " << recordTypeMap[recordType] << ", " << offset << ", " << size << ", " << lineno << endl;
	return;
}


void symbolTable::dumpCSV(ofstream &CSV)
{
	CSV << "# File Name: " << inputFile << endl;
	CSV << "# Table Name: " << name;
	if (parentSymtable)
		CSV << ", Parent Table: " << parentSymtable -> name;
	
	CSV << endl;

	int index = 0;

	if (tableType == tableType::FUNCTION)
	{
		CSV << "\n# Incoming Parameters: \n";
		CSV << "index, name, type, recordType, offset, size, line no. \n";
		for (; index < numParams; index++)
			(entries[index]) -> dumpCSV(CSV);
	}

	CSV << "\n# Local Variables: \n";
	CSV << "index, name, type, recordType, offset, size, line no. \n";
	for (; index < currentIndex; index++)
		(entries[index]) -> dumpCSV(CSV);

	for(auto child : childIndices) 
	{
		CSV << ",\n,\n,\n";
		assert(entries[child] -> symTab);
		((entries[child]) -> symTab) -> dumpCSV(CSV);
	}

	return;
}



void TreeNode::make_dot(string out)
{
	ofstream DOT(out);

	if (!DOT.is_open())
	{
		cerr << "Error: Unable to open file " << out << endl;
		return;
	}

	DOT << "digraph Tree {\nnode [style = radial]\nedge [arrowhead=onormal]\nnodesep=2;\nranksep=5;\n";
	// DOT << "digraph Tree {\n";

	queue<pair<int, TreeNode *>> NodeQueue;
	NodeQueue.push({0, this});
	int index = 1;

	while (!NodeQueue.empty())
	{
		int current_index = NodeQueue.front().first;
		TreeNode *node = NodeQueue.front().second;
		NodeQueue.pop();
		for (int i = 0; i < (node->children).size(); i++, index++)
		{
			DOT << current_index << " -> " << index << ";\n";
			NodeQueue.push({index, (node->children)[i]});
		}
	}

	NodeQueue.push({0, this});
	index = 1;
	while (!NodeQueue.empty())
	{
		int current_index = NodeQueue.front().first;
		TreeNode *node = NodeQueue.front().second;
		NodeQueue.pop();
		string tmp;

		if (node->type == "NON_TERMINAL")
			tmp = node->name;
		else if (node->type == "DEDENT" || node->type == "EOF" || node->type == "NEWLINE" || node->type == "INDENT")
			tmp = node->type;
		else
			tmp = node->type + "\n" + "(" + node->name + ")";

		DOT << current_index << " [ label = \"";

		if (node->type == "NON_TERMINAL")
			DOT << tmp << "\", fillcolor=\"white:bisque";
		else if (node->type == "STRING_LITERAL")
		{
			DOT << node->type + "\n(";
			for (char c : node->name)
			{
				if (c == '"')
				{
					DOT << '\\' << '\"';
				}
				else if (c == '\\')
				{
					DOT << '\\' << '\\';
				}
				else
				{
					DOT << c;
				}
			}
			DOT << ")\", shape = rectangle, fillcolor=\"white:lightgreen";
		}
		else
		{
			DOT << tmp << "\", shape = rectangle, fillcolor=\"white:";

			if (node->type == "DEDENT")
				DOT << "pink";
			else if (node->type == "EOF")
				DOT << "pink";
			else if (node->type == "NEWLINE")
				DOT << "pink";
			else if (node->type == "INDENT")
				DOT << "pink";
			else if (node->type == "KEYWORD")
				DOT << "orchid1";
			else if (isOperator(node))
				DOT << "lightcoral";
			else if (node->type == "IDENTIFIER")
				DOT << "slategray1";
			else if (node->type == "INT_LITERAL")
				DOT << "lightgreen";
			else if (node->type == "FLOAT_LITERAL")
				DOT << "lightgreen";
			else if (node->type == "DELIMITER")
				DOT << "pink";
		}

		DOT << "\" ];\n";

		for (int i = 0; i < (node->children).size(); i++, index++)
		{
			NodeQueue.push({index, (node->children)[i]});
		}
	}

	DOT << "}";
	DOT.close();
}
