#include "include/symtable.hpp"
#include "include/error.hpp"

#define SIZE_INT 4
#define SIZE_PTR 8
#define SIZE_FLOAT 8
#define SIZE_BOOL 1
#define SIZE_STRING(x) (1 + (x))
#define SIZE_LIST(x) (8 * ((x)-2))

extern TreeNode *root;
symbolTable *currTable = new symbolTable("__GLOBAL__", NULL);
symbolTable *globTable = currTable;
map<int, string> recordTypeMap;
map<string, int> typeMap;
int type_offset = 10;

inline void initTypes()
{
	recordTypeMap[recordType::TYPE_FUNCTION] = "TYPE_FUNCTION";
	recordTypeMap[recordType::TYPE_CLASS] = "TYPE_CLASS";
	recordTypeMap[recordType::CONST_STRING] = "CONST_STRING";
	recordTypeMap[recordType::CLASS_ATTRIBUTE] = "CLASS_ATTRIBUTE";
	recordTypeMap[recordType::CLASS_OBJECT] = "CLASS_OBJECT";
	recordTypeMap[recordType::OBJECT_ATTRIBUTE] = "OBJECT_ATTRIBUTE";
	recordTypeMap[recordType::VARIABLE] = "VARIABLE";


	typeMap["None"]			= 0;
	typeMap["bool"]			= 1;
	typeMap["int"]			= 2;
	typeMap["float"]		= 3;
	typeMap["str"]			= 4;
	typeMap["list"]			= 5;
	typeMap["list[bool]"]	= 6;
	typeMap["list[int]"]	= 7;
	typeMap["list[float]"]	= 8;
	typeMap["list[str]"]	= 9;
	
}

tableRecord::symRecord(string __name, string __type, int __size, int __lineno, int __column, int __recordType)
{
	name = __name;
	type = __type;
	size = __size;
	lineno = __lineno;
	column = __column;
	recordType = __recordType;
}


symbolTable::symTable(string __name, symbolTable* __parentSymtable)
{
	name = __name;
	parentSymtable = __parentSymtable;
}

// look only inside the table, donot go one level up (for variables)
tableRecord* symbolTable::lookup_table(string name, int recordType, vector<tableRecord*> *params)
{
	vector<int>indices = name_to_indices[name];
	tableRecord* record = NULL;

	if (recordType == recordType::TYPE_FUNCTION)
	{
		for (auto &i: indices)
		{
			// return any matching entry with type other than function or class
			if (entries[i] -> recordType != recordType::TYPE_CLASS && entries[i] -> recordType != recordType::TYPE_FUNCTION)
			{
				if (entries[i] -> recordType == recordType::CONST_STRING)
					continue;

				return entries[i];
			}
		}

		for (auto &i: indices)
		{
			// the type must now be class or function
			if (entries[i] -> recordType == recordType::TYPE_FUNCTION && params)
			{
				tableRecord* entry = entries[i];
				symbolTable* table = entry -> symTab;
				if (params -> size() != table -> numParams) continue;
		
				int num;
				for (num = 0; num < params -> size(); num++)
				{
					if ((*params)[num] -> type != ((table -> entries)[num]) -> type)
						break;
				}
				if (num == params -> size()) return entry;
			}
		}

		return NULL;
	}

	if (recordType == recordType::TYPE_CLASS)
	{
		for (auto &i: indices)
		{
			// return any matching entry with type other than function or class
			if (entries[i] -> recordType != recordType::TYPE_FUNCTION)
			{
				if (entries[i] -> recordType == recordType::CONST_STRING)
					continue;

				return entries[i];
			}
		}
		return NULL;
	}

	if(indices.size())
	{
		assert(indices.size() == 1);
		record = entries[indices[0]];
		return record;
	}

	return NULL;
}

// for variables
tableRecord* symbolTable::lookup(string name, int recordType, vector<tableRecord*> *params)
{
	tableRecord* record = lookup_table(name, recordType);
	if (record)
		return record;

	if(!parentSymtable) 
		return NULL;

	record = parentSymtable -> lookup(name, recordType, params);
	return record;
}

int symbolTable::insert(tableRecord* inputRecord, symbolTable* funcTable)
{
	string name = inputRecord -> name;
	string type = inputRecord -> type;
	int lineno = inputRecord -> lineno;
	int column = inputRecord -> column;
	int __size = inputRecord -> size;
	int recordType = inputRecord -> recordType;
	TreeNode *tempNode = new TreeNode(name, lineno, column);

	if (recordType != recordType::TYPE_FUNCTION)
	{
		tableRecord* entry = lookup_table(name, recordType);
		if (entry)
		{
			raise_error (ERR::REDIFINITION, tempNode);
			print_note (NOTE::PREV_DECL, entry);
			free(tempNode);
			return -1;
		}
	}

	else
	{
		assert(funcTable);
		vector<tableRecord*> params;
		for (int i = 0; i < funcTable -> numParams; i++)
			params.push_back((funcTable -> entries)[i]);

		tableRecord* entry = lookup_table(name, recordType, &params);
		if(entry)
		{
			if (entry -> recordType == recordType::TYPE_FUNCTION)
			{
				raise_error (ERR::REDIFINITION, tempNode);
				print_note (NOTE::PREV_DECL, entry);
				free(tempNode);
				return -1;
			}
			raise_error (ERR::TYPE_REDECLARATION, tempNode);
			print_note (NOTE::PREV_DECL, entry);
			free(tempNode);
			return -1;
		}

	}

	tableRecord* record = new tableRecord(name, type, __size, lineno, column, recordType);
	record -> symTab = this;

	if ((recordType == recordType::TYPE_CLASS || recordType == recordType::TYPE_FUNCTION))
	{
		record -> symTab = funcTable;
		childIndices.push_back(currentIndex);
	}

	inputRecord -> symTab = record -> symTab;

	name_to_indices[name].push_back(currentIndex);
	record -> index = currentIndex;
	entries[currentIndex] = record;
	record -> offset = size;

	// size of the table not updated when function entry
	if (recordType != recordType::TYPE_FUNCTION)
		size += __size;
	
	if (recordType == recordType::CLASS_ATTRIBUTE)
	{
		staticIndices.insert(currentIndex);
		size += SIZE_PTR - __size; 			// only one pointer needs to be stored
	}
	
	// if (!isValidType(record -> type))
	// 	typeMap[record -> type] = type_offset++;

	currentIndex++;
	return 0;
}

void formatString(string &name, string &type)
{

	if (name[0] == 'r' || name[0] == 'R')
	{
		name = name.substr(1, name.length() - 1);

		if(name[0] == 'b' || name[0] == 'B')
		{
			name = name.substr(1, name.length() - 1);
			while (name[0] == '\'' || name[0] == '\"')
			{
				name = name.substr(1, name.length() - 2);
			}
			type = "bstr";
			return;
		}

		while (name[0] == '\'' || name[0] == '\"')
		{
			name = name.substr(1, name.length() - 2);
		}
		return;

	}


	if(name[0] == 'b' || name[0] == 'B')
	{
		name = name.substr(1, name.length() - 1);
		if (name[0] == 'r' || name[0] == 'R')
		{
			name = name.substr(1, name.length() - 1);
			while (name[0] == '\'' || name[0] == '\"')
			{
				name = name.substr(1, name.length() - 2);
			}
			type = "bstr";
			return;
		}
		type = "bstr";
	}


	while (name[0] == '\'' || name[0] == '\"')
	{
		name = name.substr(1, name.length() - 2);
	}


	string tmp = "";
	for (int i = 0; i < name.length(); i++)
	{
		if (name[i] == '\\')
		{
			switch (name[i + 1])
			{


				case '\\':
					tmp += '\\';
					i++;
					break;
					

				case '\'':
					tmp += '\'';
					i++;
					break;


				case '\"':
					tmp += '\"';
					i++;
					break;


				case 'a':
					tmp += '\a';
					i++;
					break;


				case 'b':
					tmp += '\b';
					i++;
					break;


				case 'f':
					tmp += '\f';
					i++;
					break;


				case 'n':
					tmp += '\n';
					i++;
					break;


				case 'r':
					tmp += '\r';
					i++;
					break;


				case 't':
					tmp += '\t';
					i++;
					break;


				case 'v':
					tmp += '\v';
					i++;
					break;


				case '\n':
					tmp += '\\';
					tmp += '\n';
					i += 2;
					break;


				case '\r':
					tmp += '\\';
					tmp += '\r';
					if (i + 2 < name.length() && name[i + 2] == '\n')
					{
						tmp += '\n';
						i++;
					}
					i += 2;
					break;


				default:
					tmp += name[i + 1];
					i++;
					break;


			}
		}
		else
			tmp += name[i];
	}
	name = tmp;
	return;
}

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
	MD << "## Table Name: ";
	print_name(MD, name);
	if (parentSymtable)
	{
		MD << " &emsp;&emsp;&emsp;&emsp; Parent Table: ";
		print_name(MD, parentSymtable -> name);
	}
	
	MD << " <br>";
	MD << endl;

	int index = 0;

	if (tableType == tableType::FUNCTION)
	{
		MD << "\n### Incoming Parameters: \n";
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

	MD << "\n### Local Variables: \n";
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

// root is function def here
int handle_function_declaration(TreeNode* root)
{
	// a function definition has 6 children
	if (((root -> children)[0] -> name).compare("main") && ((root -> children)[0] -> name).compare("__init__"))
		assert((root -> children).size() == 6);
	else 
		assert((root -> children).size() == 5 || (root -> children).size() == 6);


	symbolTable *Table = new symbolTable(((root -> children)[0]) -> name, currTable);
	Table -> tableType = tableType::FUNCTION;

	currTable = Table;
	for(auto &itr: ((root -> children)[2]) -> children)
	{
		if ((itr -> name).compare("self"))
			currTable -> numParams++;
	}

	TreeNode* node = ((root -> children)[0]);
	string type = "None";
	if (((root -> children)[0] -> name).compare("main") && ((root -> children)[0] -> name).compare("__init__"))
		type = (((root -> children)[4]) -> children)[0] -> name;

	tableRecord* record = new tableRecord(node -> name, type, currTable -> size, node -> lineno, node -> column, recordType::TYPE_FUNCTION);
	
	assert(currTable -> parentSymtable);
	int err = currTable -> parentSymtable -> insert(record, currTable);
	node -> dataType = type;

	free(record);
	record = NULL;
	
	if (err < 0)
		return err;

	return 0;
}

// root is class def here
int handle_class_declaration(TreeNode* root)
{
	TreeNode* node = ((root -> children)[0]);
	if (currTable != globTable)
	{
		raise_error(ERR::CLASS_NOT_GLOBAL, node);
		return -1;
	}
	
	symbolTable* parent = globTable;
	if((root -> children.size() > 2))
	{
		node = (root -> children)[2];
		if((node -> type).compare("IDENTIFIER") == 0)
		{
			tableRecord* entry = currTable -> lookup(node -> name, recordType::TYPE_CLASS);
			TreeNode *tempNode = new TreeNode(entry -> name, entry->lineno, entry->column);
			if(entry)
			{
				if (entry -> recordType != recordType::TYPE_CLASS)
				{
					raise_error(ERR::ILL_PARENT, tempNode);
					free (tempNode);
					return -1;
				}	
			}

			assert(entry -> symTab);
			parent = entry -> symTab;
			free (tempNode);
			tempNode = NULL;
		}
	}

	symbolTable *Table = new symbolTable(((root -> children)[0]) -> name, parent);
	Table -> tableType = tableType::CLASS;
	currTable = Table;

	node = ((root -> children)[0]);
	tableRecord* record = new tableRecord(node -> name, node -> name, currTable -> size, node -> lineno, node -> column, recordType::TYPE_CLASS);
	
	int err = globTable -> insert(record, currTable);

	node -> dataType = node -> name;

	free(record);
	record = NULL;

	if (err < 0)
		return err;

	return 0;
}

// root is const string here
int handle_const_strings(TreeNode* root)
{
	tableRecord* record;
	string str_type = "str";
	formatString(root -> name, str_type);
	record = new tableRecord(root -> name, str_type, SIZE_STRING((root -> name).length()), root -> lineno, root -> column, recordType::CONST_STRING);
	if(!globTable -> lookup(record -> name, recordType::CONST_STRING))
		globTable -> insert(record, NULL);

	root -> dataType = str_type;

	free (record);
	return 0;
}

// root is colon here
void set_record_size(TreeNode* root, tableRecord* record)
{
	assert((root->children).size() == 2);
	TreeNode* node = (root->children)[1];

	// type of list constructed here
	if ((node -> name).compare("list") == 0)
	{
		// some explicit type inside list 
		assert((node -> children).size() > 2);

		string type = node -> name + "[" + (node -> children)[1] -> name + "]";
		record -> type = type;
	}

	else
	{
		record -> type = node -> name;

		if ((record -> type).compare("int") == 0)
			record -> size = SIZE_INT;

		else if ((record -> type).compare("float") == 0)
			record -> size = SIZE_FLOAT;

		else if ((record -> type).compare("bool") == 0)
			record -> size = SIZE_BOOL;

		else if ((record -> type).compare("str") == 0)
			record -> size = SIZE_PTR;

		else if ((record -> type).compare(0, 4, "list") == 0)
			record -> size = SIZE_PTR;

		// dealing with classes
		else {
			record -> size = SIZE_PTR;
			record -> recordType = recordType::CLASS_OBJECT;
		}
	}
}

// root is colon here
int handle_type_declarations(TreeNode* root)
{
	// colon will have exactly 2 chidren
	assert((root -> children).size() == 2);
	
	TreeNode* node = ((root -> children)[0]);
	symbolTable* tempTable = currTable;
	int recordType = recordType::VARIABLE;
	int size = 0;

	if((node -> name).compare(".") == 0)
	{
		assert((node -> children).size() == 2);
		TreeNode* tempNode = (node -> children)[0];
		if(((node -> children)[0] -> name).compare("self"))
		{
			raise_error(ERR::CLASS_ATTRIBUTE_DECL_SELF, node);
			return -1;
		}

		if((currTable -> name).compare("__init__"))
		{
			raise_error(ERR::CLASS_ATTRIBUTE_DECL_CTOR, node);
			return -1;
		}

		if(!(currTable -> parentSymtable) || (currTable -> parentSymtable) -> tableType != tableType::CLASS)
		{
			raise_error(ERR::CLASS_CTOR, node);
			return -1;
		}

		// declaration inside ctor of a class
		tempTable = currTable -> parentSymtable;
		node = (node -> children)[1];
		recordType = recordType::OBJECT_ATTRIBUTE;
	}

	else if(tempTable -> tableType == tableType::CLASS)
		recordType = recordType::CLASS_ATTRIBUTE;
	
	tableRecord* record = new tableRecord(node -> name, "", size, node -> lineno, node -> column, recordType);
	set_record_size(root, record);

	node -> dataType = record -> type;
	
	int err = tempTable -> insert(record);
	assert(record->symTab);
	
	free(record);
	record = NULL;

	if (err < 0)
		return err;

	return 0;
}

// assignment in ctor
int handle_ctor_assignments(TreeNode* root)
{ 
	assert((root -> children).size() == 2);
	if ((currTable -> name).compare("__init__") == 0)
	{
		TreeNode* node = (root -> children)[0];
		assert(currTable -> parentSymtable);
		assert(currTable -> parentSymtable -> tableType == tableType::CLASS);

		if ((node -> name).compare(".") == 0)
		{
			assert((node -> children).size() == 2);
			if (((node -> children)[0] -> name).compare("self") == 0)
			{
				
				tableRecord* entry = currTable -> parentSymtable -> lookup_table((node -> children)[1] -> name);
				if (!entry)
				{
					// nothing found in the immidiate table, may be in parent class table
					node = (node -> children)[1];
					assert(node);
					entry = currTable -> parentSymtable -> lookup(node -> name);
					if (!entry)
					{
						raise_error(ERR::CLASS_NO_MATCH_ATTR, node);
						return -1;
					}

					// since the above lookup returned true
					assert (currTable -> parentSymtable -> parentSymtable);
					if (entry -> symTab -> tableType != tableType::CLASS)
					{
						raise_error(ERR::CLASS_NO_MATCH_ATTR, node);
						return -1;
					}

					int err = currTable -> parentSymtable -> insert(entry);

					node -> dataType = entry -> type;

					if (err < 0)
						return -1;
				}

				// else do nothing, this is of the same class
			}
		}
	}

	return 0;
}

int generate_symtable(TreeNode *root)
{

	if ((root -> type).compare("NON_TERMINAL") == 0 && (root -> name).compare("function_def") == 0)
	{
		int ret = handle_function_declaration(root);
		if (ret < 0)
			return ret;
	}

	if ((root -> type).compare("NON_TERMINAL") == 0 && (root -> name).compare("class_def") == 0)
	{
		int ret = handle_class_declaration(root);
		if (ret < 0)
			return ret;
	}

	vector<TreeNode *> &children = root -> children;
	for (int nchild = 0; nchild < (root -> children).size(); nchild++)
	{
		int ret = generate_symtable(children[nchild]);
		if (ret < 0)
			return ret;
	}

	if ((root -> type).compare("STRING_LITERAL") == 0)
	{
		int ret = handle_const_strings(root);
		if (ret < 0)
			return ret;
		return 0;
	}

	if ((root -> type).compare("DELIMITER") == 0 && (root -> name).compare(":") == 0)
	{
		int ret = handle_type_declarations(root);
		if (ret < 0)
			return ret;
		return 0;
	}

	if (isOperator(root) && (root -> name).compare("=") == 0)
	{
		int ret = handle_ctor_assignments(root);
		if (ret < 0)
			return ret;
	}

	if (isOperator(root))
	{
		if ((root -> type).compare("OP_ARITHMATIC") == 0)
		{
			if (root->name != "-")
				assert((root -> children).size() == 2);
			else assert ((root -> children).size() == 2 || (root -> children).size() == 1);

			if ((root -> children).size() == 2)
			{
				string type1 = (root->children)[0]->dataType;
				string type2 = (root->children)[1]->dataType;
				string myType = isCompatible(type1, type2);
				if (! myType.length())
				{
					raise_error(ERR::TYPE_MISMATCH, (root->children)[1]);
					return -1;
				}

				if (! isNumberType(type1) && isNumberType(type2));

			}
		}
	}

	// dealing with functions again, after completing the function symbol table
	if ((root -> type).compare("NON_TERMINAL") == 0 && (root -> name).compare("function_def") == 0)
	{
		assert ((currTable -> name).compare((root -> children)[0] -> name) == 0);
		vector<tableRecord*> params;
		for (int i = 0; i < currTable -> numParams; i++)
			params.push_back((currTable -> entries)[i]);
		
		assert(currTable -> parentSymtable);
		tableRecord* entry = currTable -> parentSymtable -> lookup_table((root->children)[0]->name, recordType::TYPE_FUNCTION, &params);
		assert(entry);
		entry -> size = currTable -> size;
		currTable = currTable -> parentSymtable;
		return 0;

	}

	// dealing with classes again on coming back
	if ((root -> type).compare("NON_TERMINAL") == 0 && (root -> name).compare("class_def") == 0)
	{
		assert ((currTable -> name).compare((root -> children)[0] -> name) == 0);
		assert(currTable -> parentSymtable);
		tableRecord* entry = globTable -> lookup_table((root->children)[0]->name, recordType::TYPE_CLASS);
		assert(entry);
		entry -> size = currTable -> size;
		currTable = globTable;
		return 0;
	}

	return 0;
}

int symTable_Maker(TreeNode *root)
{
	globTable -> tableType = tableType::GLOBAL;
	tableRecord* record = NULL;
	initTypes();
	return generate_symtable(root);
}

inline int isValidType(string type1)
{

	if (typeMap.find(type1) == typeMap.end())
		return 0;

	return 1;
}

string isCompatible(string type1, string type2)
{

	if (!(type1.compare("float") || type2.compare("int")))
		return type1;

	if (!(type1.compare("int") || type2.compare("float")))
		return type2;

	if (!(type1.compare("int") || type2.compare("bool")))
		return type1;

	if (!(type1.compare("bool") || type2.compare("int")))
		return type2;

	return "";
}

bool isNumberType(string type)
{

	if (type == "float")
		return true;

	if (type == "float")
		return true;

	return false;
}

// int checkListAccess(TreeNode* root)
// {
// 	if ((root->children).size() == 3)
// 	{
// 		if (! (((root->children)[0]->name).compare("[")) 
// 							|| (((root->children)[2]->name).compare("]")))
// 		{
// 			if (root -> children)
// 		}
// 	}
// }