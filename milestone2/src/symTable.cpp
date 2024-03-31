#include "include/symtable.hpp"
#include "include/error.hpp"

extern TreeNode *root;
symbolTable *currTable = new symbolTable("__GLOBAL__", NULL);
symbolTable *tempDotTable = NULL;
symbolTable *globTable = currTable;
map<int, string> recordTypeMap;
map<string, int> typeMap;
int type_offset = 1;
vector<int> in_loop(1, 0);
int in_function = 0;
int self_ind =  -1;
int fun_call = 0;
string self_type = "";
int funcInClass = 0;


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
			if (entries[i] -> recordType != recordType::TYPE_FUNCTION)
			{
				if (entries[i] -> recordType == recordType::CONST_STRING)
					continue;

				return entries[i];
			}
		}

		// count of matched functions after typecasting
		int cnt = 0;
		vector<int> index;
		for (auto &i: indices)
		{
			// the type must now be class or function
			if (entries[i] -> recordType == recordType::TYPE_FUNCTION && params)
			{
				bool match = true;
				tableRecord* entry = entries[i];
				symbolTable* table = entry -> symTab;

				if (params -> size() != table -> numParams) continue;

				int num;
				for (num = 0; num < params -> size(); num++)
				{
					if (self_ind != -1)
					{
						if(num == self_ind && (*params)[num]->name == "self" && (*params)[num]->recordType == recordType::CLASS_SELF)
						{
							continue;
						}
						else if(! ((*params)[num]->name == "self" && (*params)[num]->recordType == recordType::CLASS_SELF))
						{
							match = false;
							break;
						}
					}
					// if (name == "__init__") cout << params->size() << endl << ((table -> entries)[num]) -> name << endl;
					if ((*params)[num] -> type != ((table -> entries)[num]) -> type)
						match = false;

					string final = isCompatible((*params)[num] -> type, ((table -> entries)[num]) -> type);
					if (!final.length())
						break;
					
				}
				if (num == params -> size()) 
				{
					if (!match)
					{
						index.push_back(i);
						cnt++;
						continue;
					}
					return entry;
				}
			}
		}

		if (cnt > 1)
		{
			TreeNode* node = new TreeNode("");
			for (auto&i: index)
			{
				node -> name = entries[i] -> name;
				node -> lineno = entries[i] -> lineno;
				node -> column = entries[i] -> column;
				raise_error(ERR::CANDIDATE, node);
			}
		}

		return NULL;
	}

	if (recordType == recordType::CLASS_CONSTRUCTOR)
	{
		for (auto &i: indices)
		{
			if (entries[i] -> recordType == recordType::TYPE_CLASS)
				continue;

			if (entries[i] -> recordType == recordType::CONST_STRING)
				continue;

			return entries[i];
		}

		return NULL;
	}

	if (recordType == recordType::TYPE_CLASS)
	{
		for (auto &i: indices)
		{
			if (entries[i] -> recordType == recordType::CONST_STRING)
				continue;

			return entries[i];
		}

		return NULL;
	}

	if(indices.size())
	{
		record = entries[indices[0]];
		return record;
	}

	return NULL;
}

// for variables
tableRecord* symbolTable::lookup(string name, int recordType, vector<tableRecord*> *params)
{
	tableRecord* record = lookup_table(name, recordType, params);
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
	// cout << name << " " << lineno << " " << column << " " << " " << type << " " << recordTypeMap[recordType] << " " << __size << endl; 
	TreeNode *tempNode = new TreeNode(name, lineno, column);

	if (recordType != recordType::TYPE_FUNCTION)
	{
		tableRecord* entry = lookup_table(name, recordType);
		if (entry && entry->recordType != recordType::KEYWORD)
		{
			raise_error (ERR::REDIFINITION, tempNode);
			print_note (NOTE::PREV_DECL, entry);
			free(tempNode);
			return -1;
		}
		else if (entry && entry->recordType == recordType::KEYWORD)
		{
			raise_error (ERR::KEYWORD_DECL, tempNode);
			free(tempNode);
			return -1;
		}
	}

	else
	{
		// cout << name << " " << lineno << " " << column << " " << " " << type << " " << recordTypeMap[recordType] << " " << __size << endl; 
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
			else if (entry->recordType == recordType::KEYWORD)
			{
				raise_error (ERR::KEYWORD_DECL, tempNode);
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

	if (recordType == recordType::TYPE_FUNCTION)
		record->offset = -1;
	else
		record -> offset = size;

	// size of the table not updated when function entry
	if (recordType != recordType::TYPE_FUNCTION)
		size += __size;
	
	if (recordType == recordType::CLASS_ATTRIBUTE)
	{
		staticIndices.insert(currentIndex);
		size += SIZE_PTR - __size; 			// only one pointer needs to be stored
	}

	currentIndex++;
	
	if (!isValidType(record -> type) && (record -> recordType == recordType::TYPE_CLASS))
	{
		typeMap[record -> type] = type_offset++;
		string list_type = "list[" + record->type + "]";
		typeMap[list_type] = type_offset++;

		// set up table for len
		symTable* lenTable = new symTable("len", globTable);
		lenTable -> numParams = 1;
		lenTable -> tableType = tableType::FUNCTION;
		tableRecord* lenParam = new tableRecord("#len", list_type, SIZE_PTR);
		tableRecord* lenFunc = new tableRecord("len", "int", SIZE_PTR, 0, 0, recordType::TYPE_FUNCTION);
		lenTable -> insert(lenParam);
		globTable -> insert(lenFunc, lenTable);

	}

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

// root is the expression here
int handle_bool_expressions(TreeNode* root)
{
	string type = root -> dataType;
	if (!isCompatible(type, "bool").length())
	{
		raise_error(ERR::EXPECTED_BOOL, root);
		return -1;
	}
	return 0;
}

// root is function def here
int handle_function_declaration(TreeNode* root)
{
	// a function definition has 6 children
	if (((root -> children)[0] -> name).compare("main")
					 && ((root -> children)[0] -> name).compare("__init__"))

	{

		if ((root -> children).size() == 5)
		{
			raise_error (ERR::NO_RET_TYPE, (root ->children)[0]);
			return -1;
		}
	}

	else 
		assert((root -> children).size() == 5 || (root -> children).size() == 6);


	symbolTable *Table = new symbolTable(((root -> children)[0]) -> name, currTable);
	Table -> tableType = tableType::FUNCTION;

	int numParam = 0;

	int cntSelf = 0;
	int index = 0;
	int last_pos = index;

	for (auto &itr: ((root -> children)[2]) -> children)
	{
		if ((currTable -> tableType != tableType::CLASS && (itr -> name).compare("self") == 0))
		{
			raise_error(ERR::TYPE_NOT_DECL_PARAM, itr);
			free(Table);
			return -1;
		}

		else if ((itr -> name).compare(":") == 0)
			numParam++;

		else if ((currTable -> tableType == tableType::CLASS && (itr -> name).compare("self") == 0))
		{
			cntSelf++;
			numParam++;
			last_pos = index;
		}

		index++;
	}

	if (cntSelf > 1)
	{
		raise_error(ERR::DUPLICATE_SELF, (((root -> children)[2]) -> children)[last_pos]);
		return -1;
	}


	if (currTable -> tableType == tableType::CLASS && cntSelf == 0)
	{
		raise_error(ERR::NO_SELF, (((root -> children)[2]) -> children)[last_pos]);
		return -1;
	}

	currTable = Table;
	currTable -> numParams = numParam;

	TreeNode* node = ((root -> children)[0]);
	string type = "None";
	if (((root -> children)[0] -> name).compare("main") && ((root -> children)[0] -> name).compare("__init__"))
	{
		int ret = handle_to(((root -> children)[4]));
		if (ret < 0)
			return ret;
		type = (((root -> children)[4]) -> children)[0] -> dataType;
	}
	node -> dataType = type;

	tableRecord* record = new tableRecord(node -> name, type, currTable -> size, node -> lineno, node -> column, recordType::TYPE_FUNCTION);
	
	assert(currTable -> parentSymtable);
	int err = currTable -> parentSymtable -> insert(record, currTable);

	free(record);
	record = NULL;

	if (err < 0)
		return err;

	if (cntSelf)
	{
		// cout << cntSelf << "  " << numParam << endl;
		// record = currTable->lookup("self", recordType::CLASS_SELF);
		// assert (record);
		// tableRecord* entry = new tableRecord("self", , record->size, record->lineno, record->column, record->recordType);
		
		tableRecord* entry = new tableRecord("self", currTable -> parentSymtable -> name, SIZE_PTR, (((root -> children)[2]) -> children)[last_pos]->lineno, (((root -> children)[2]) -> children)[last_pos]->column, recordType::CLASS_OBJECT);
		err = currTable -> insert(entry);
		free(entry);
	}

	if (err < 0)
		return err;


	in_loop.push_back(0);
	in_function++;

	return 0;
}

// int fillTables(symbolTable* Table, symbolTable* parentTable)
// {
// 	int err = 0;

// 	for (int entry = 0, child = 0; entry < parentTable->currentIndex; entry++)
// 	{
// 		if(!((parentTable->entries)[entry] -> name == "self" && (parentTable->entries)[entry] -> recordType == recordType::CLASS_SELF))
// 		{
// 			if (child < (parentTable->childIndices).size() && parentTable->childIndices[child] == entry && !((parentTable->entries)[entry]->name == "__init__"))
// 			{
// 				err = Table->insert((parentTable->entries)[entry], (parentTable->entries)[entry] -> symTab);
// 				// cout << (newTable->entries)[0] -> name << endl;
// 				child++;
// 				// cout << newTable->currentIndex << endl;
// 			}
// 			else if (!((parentTable->entries)[entry]->name == "__init__"))
// 			{
// 				err = Table->insert((parentTable->entries)[entry], (parentTable->entries)[entry]->symTab);
// 			}
// 		}
// 		if (err < 0)
// 		{
// 			raise_error(ERR::IMPOSSIBLE, root);
// 			return err;
// 		}
// 	}
// 	return err;

// }

int fillTables(symbolTable* Table, symbolTable* parentTable, string type)
{
	int err = 0;

	for (int entry = 0, child = 0; entry < parentTable->currentIndex; entry++)
	{
		if(!((parentTable->entries)[entry] -> name == "self" && (parentTable->entries)[entry] -> recordType == recordType::CLASS_SELF))
		{


			if (child < (parentTable->childIndices).size() && parentTable->childIndices[child] == entry && (parentTable->entries)[entry]->name == "__init__")
			{
				child++;
				continue;
			}

			if (child < (parentTable->childIndices).size() && parentTable->childIndices[child] == entry)
			{
				symbolTable* newTable = new symbolTable((parentTable->entries)[entry]->name, Table);
				newTable->numParams = (parentTable->entries)[entry]->symTab->numParams;
				newTable->tableType = (parentTable->entries)[entry]->symTab->tableType;
				fillTables(newTable, (parentTable->entries)[entry]->symTab, type);
				tableRecord* newRecord = new tableRecord((parentTable->entries)[entry]->name, (parentTable->entries)[entry]->type, (parentTable->entries)[entry]->size, (parentTable->entries)[entry]->lineno, (parentTable->entries)[entry]->column, (parentTable->entries)[entry]->recordType);
				err = Table->insert(newRecord, newTable);
				free(newRecord);
				child++;
			}
			else
			{
				tableRecord* newRecord = new tableRecord((parentTable->entries)[entry]->name, (parentTable->entries)[entry]->type, (parentTable->entries)[entry]->size, (parentTable->entries)[entry]->lineno, (parentTable->entries)[entry]->column, (parentTable->entries)[entry]->recordType);
				err = Table->insert(newRecord, Table);
				free(newRecord);
			}
		}
		else
		{
			tableRecord* selfRecord = new tableRecord((parentTable->entries)[entry]->name, type, (parentTable->entries)[entry]->size, (parentTable->entries)[entry]->lineno, (parentTable->entries)[entry]->column, (parentTable->entries)[entry]->recordType);
			err = Table->insert(selfRecord, Table);
			free(selfRecord);
		}
		if (err < 0)
		{
			raise_error(ERR::IMPOSSIBLE, root);
			return err;
		}
	}
	return err;
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
	if ((root -> children.size() > 2))
	{
		// find parent first
		node = (root -> children)[2];
		if((node -> type).compare("IDENTIFIER") == 0)
		{
			tableRecord* entry = currTable -> lookup(node -> name, recordType::TYPE_CLASS);
			if(entry)
			{
				if (entry -> recordType != recordType::TYPE_CLASS)
				{
					raise_error(ERR::ILL_PARENT, node);
					return -1;
				}	
			}

			assert(entry -> symTab);
			parent = entry -> symTab;
		}
	}

	symbolTable *Table = new symbolTable(((root -> children)[0]) -> name, parent);
	Table -> tableType = tableType::CLASS;
	currTable = Table;

	node = ((root -> children)[0]);
	node -> dataType = node -> name;
	tableRecord* record = new tableRecord(node -> name, node -> name, currTable -> size, node -> lineno, node -> column, recordType::TYPE_CLASS);
	int err = globTable -> insert(record, currTable);
	
	free(record);
	record = NULL;
	if (err < 0)
		return err;

	// record = new tableRecord("self", node -> name, SIZE_PTR, node -> lineno, node -> column, recordType::CLASS_SELF);
	// err = currTable -> insert(record);

	// free(record);
	// record = NULL;
	// if (err < 0)
	// 	return err;

	
	if (parent != globTable)
	{
		err = fillTables(currTable, parent, currTable -> name);
		if (err < 0)
			return err;
	}

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

// root is const int here
int handle_const_int(TreeNode* root)
{
	tableRecord* record;
	string type = "int";
	record = new tableRecord(root -> name, type, SIZE_INT, root -> lineno, root -> column, recordType::CONST_INT);
	if(!globTable -> lookup(record -> name, recordType::CONST_INT))
		globTable -> insert(record, NULL);

	root -> dataType = type;

	free (record);
	return 0;
}

// root is const float here
int handle_const_float(TreeNode* root)
{
	tableRecord* record;
	string type = "float";
	record = new tableRecord(root -> name, type, SIZE_FLOAT, root -> lineno, root -> column, recordType::CONST_FLOAT);
	if(!globTable -> lookup(record -> name, recordType::CONST_FLOAT))
		globTable -> insert(record, NULL);

	root -> dataType = type;

	free (record);
	return 0;
}

// root is True / False here
int handle_const_bool(TreeNode* root)
{
	tableRecord* record;
	string type = "bool";
	record = new tableRecord(root -> name, type, SIZE_BOOL, root -> lineno, root -> column, recordType::CONST_BOOL);
	if(!globTable -> lookup(record -> name, recordType::CONST_BOOL))
		globTable -> insert(record, NULL);

	root -> dataType = type;

	free (record);
	return 0;
}

// root is colon here
int set_record_fields(TreeNode* root, tableRecord* record)
{
	assert((root->children).size() == 2);
	record -> type = validateType((root -> children)[1]);
	if ((record -> type).length() == 0)
		return -1;

	if ((record -> type).compare(0,4, "list") == 0)
		record -> size = SIZE_PTR;

	else if ((record -> type).compare("None") == 0)
		record -> size = 0;

	else 
	{
		tableRecord* entry = globTable -> lookup_table(record -> type, recordType::TYPE_CLASS);
		assert (entry);
		record -> size = entry -> size;
	}

	return 0;
}

// root is head of the type
string validateType(TreeNode* root)
{
	root -> type = "DATATYPE";
	string type = root -> name;

	// type of list constructed here
	if ((root -> name).compare("list_access") == 0)
	{
		assert((root -> children).size() > 3);
		type = (root -> children)[0] -> name + "[" + (root -> children)[2] -> name + "]";
		assert((root -> children)[0] -> name == "list"); 
		(root -> children)[0] -> type = "LIST";
	}

	if (!isValidType(type))
	{
		raise_error(ERR::UNKNOWN_TYPE, root);
		return "";
	}

	root -> dataType = type;
	return type;
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

	assert (node->type == "IDENTIFIER" || node->name == ".");

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
	int err = set_record_fields(root, record);
	if (err < 0)
		return err;

	node -> dataType = record -> type;
	root -> dataType = node -> dataType;

	if (((root -> children)[0]->name).compare(".") == 0)
		(root -> children)[0] -> dataType = node -> dataType;
	
	err = tempTable -> insert(record);
	if (err < 0)
		return err;

	assert(record->symTab);
	
	free(record);
	record = NULL;

	if (err < 0)
		return err;

	return 0;
}

int handle_in(TreeNode* root)
{
	assert ((root -> children).size() == 2);
	TreeNode* left = (root -> children)[0];
	TreeNode* right = (root -> children)[1];
	assert (left -> type == "IDENTIFIER" || left -> name == "." || left -> name == "list_access");
	assert (right -> type == "IDENTIFIER" || right -> name == "." || right -> name == "function_call");

	if ((right->dataType).compare(0, 4, "list") && (right->dataType).compare("string"))
	{
		raise_error(ERR::NOT_ITERABLE, root);
		// print_note(NOTE::PREV_DECL, entry2);
		return -1;
	}

	else if ((right->dataType).compare(0, 4, "list") == 0)
	{
		if (isCompatible( (right -> dataType).substr(5, (left -> dataType).length()), left ->dataType).length() == 0)
		{
			raise_error(ERR::TYPE_MISMATCH, root);
			// print_note(NOTE::PREV_DECL, entry1);
			// print_note(NOTE::PREV_DECL, entry2);
			return -1;
		}
	}
	else
	{
		if (left -> dataType != right->dataType)
		{
			raise_error(ERR::TYPE_MISMATCH, root);
			// print_note(NOTE::PREV_DECL, entry1);
			// print_note(NOTE::PREV_DECL, entry2);
			return -1;
		}
	}

	root -> dataType = left -> dataType;

	return 0;

}

int handle_list(TreeNode* root)
{
	vector<TreeNode*>children = root -> children;
	if (children[1] -> name =="]")
	{
		raise_error(ERR::EMPTY_LIST, children[0]);
		return -1;
	}
	string type1 = children[1] -> dataType;
	if (type1.compare(0, 4, "list") == 0)
	{
		raise_error(ERR::NESTED_LIST, children[1]);
		return -1;
	}
	
	for (int i = 2; i < children.size() - 1; i++)
	{
		string type2 = children[i] -> dataType;
		if (isCompatible(type1, type2).length() == 0)
		{
			raise_error(ERR::MIXED_LIST, children[i]);
			return -1;
		}
		else
		{
			if (type1 == type2)
				continue;

			if ( type1 == "int" && type2 == "bool" )
				type1 = "bool";
			
			if ( type1 == "int" && type2 == "float" )
				type1 = "float";
		}
	}
	root -> dataType = "list[" + type1 + "]";
	return 0;
}

int post_handle_dot(TreeNode* root)
{
	assert((root->children).size() == 2);
	root -> dataType = (root->children)[1]->dataType;
	tempDotTable = NULL;
	
	return 0;
}

int pre_handle_dot(TreeNode* root)
{
	assert((root->children).size() == 2);

	int ret = handle_identifier((root->children)[0]);
	if (ret < 0)
		return ret;

	if (((root->children)[0] -> dataType).compare(0, 4, "list") == 0)
	{
		raise_error(ERR::CLASS_NO_MATCH_ATTR, (root->children)[1]);
		return -1;
	}

	tableRecord* classRecord = globTable->lookup_table((root->children)[0] -> dataType, recordType::TYPE_CLASS);
	
	// in cases of dot
	if (!classRecord)
	{
		raise_error(ERR::CLASS_NO_MATCH_ATTR, (root->children)[1]);
		return -1;
	}
	
	assert(classRecord);
	// cout << currTable->name << endl << classRecord->symTab->name << endl << classRecord->symTab->parentSymtable->name << endl;
	tempDotTable = classRecord -> symTab;
	funcInClass = 1;

	// if ((root->children)[0] -> dataType == (root->children)[0] -> name)
	// {
	// 	(root->children)[0] -> type = "self";
	// 	(root->children)[0] -> dataType = "self";
	// 	return 0;
	// }

	(root->children)[0] -> type = "CLASS_OBJ";
	(root->children)[0] -> dataType = classRecord -> type;

	if (root->children[1]->name == "function_call" && root->children[1]->type == "NON_TERMINAL" && classRecord->name != (root->children)[0] -> name)
	{
		fun_call = 1;
		self_type = classRecord -> type;
	}

	return 0;
}

int isValidType(string type1)
{

	if (typeMap.find(type1) == typeMap.end())
		return 0;

	return 1;
}

string isCompatible(string type1, string type2)
{
	if(type1 == type2)
		return type1;

	if (!(type1.compare("float") || type2.compare("int")))
		return type1;

	if (!(type1.compare("int") || type2.compare("float")))
		return type2;

	if (!(type1.compare("int") || type2.compare("bool")))
		return type1;

	if (!(type1.compare("bool") || type2.compare("int")))
		return type2;

	if (type1.compare(0, 4, "list") == 0 && type2.compare(0, 4, "list") == 0)
		return isCompatible(type1.substr(5, type1.length() - 6), type2.substr(5, type2.length() - 6));

	return "";
}

int handle_operators(TreeNode* root)
{
	assert((root->children).size() > 0);
	string category = root->type;
	TreeNode* left = (root -> children)[0];
	tableRecord* record1 = new tableRecord(left->dataType, left->dataType, left->lineno, left->column);
	string type1 = left->dataType;
	string final = type1; 
	
	assert((root -> type).compare(0, 2, "OP") == 0 );
	assert((root->children).size() == 2 || (root->children).size() == 1);

	if ((root -> children).size() == 2)
	{
		TreeNode* right = (root -> children)[1];
		string type2 = right->dataType;
		tableRecord* record2 = new tableRecord(right->dataType, right->dataType, right->lineno, right->column);

		final = isCompatible(type1, type2);
		if (final.length() == 0)
		{
			raise_error(ERR::TYPE_MISMATCH, root);
			print_note(NOTE::SHOW_TYPE, record1);
			print_note(NOTE::SHOW_TYPE, record2);
			free(record1);
			free(record2);
			return -1;
		}

		if (category == "OP_ASSIGNMENT")
		{
			if (left -> type == "INT_LITERAL" || left -> type == "FLOAT_LITERAL" || left -> type == "STRING_LITERAL")
			{
				raise_error(ERR::BAD_LVAL, left);
				return -1;
			}
			root -> dataType = final;
			return 0;
		}

		if (category == "OP_RELATIONAL")
		{

			if (final.compare("str") && final.compare("bool") && final.compare("int") && final.compare("float"))
			{
				raise_error(ERR::OP_OVERLOAD, root);
				print_note(NOTE::PREV_DECL, record1);
				print_note(NOTE::PREV_DECL, record2);
				free(record1);
				free(record2);
				return -1;
			}

			root -> dataType = "bool";
			return 0;
		}

		if (category == "OP_ARITHMETIC")
		{
			if (final.compare("int") && final.compare("float") && final.compare("bool"))
			{
				raise_error(ERR::OP_OVERLOAD, root);
				print_note(NOTE::PREV_DECL, record1);
				print_note(NOTE::PREV_DECL, record2);
				free(record1);
				free(record2);
				return -1;
			}

			root -> dataType = final;
			return 0;
		}

		if (category == "OP_BITWISE" || category == "OP_LOGICAL")
		{
			if (final.compare("int") && final.compare("bool"))
			{
				raise_error(ERR::OP_OVERLOAD, root);
				print_note(NOTE::PREV_DECL, record1);
				print_note(NOTE::PREV_DECL, record2);
				free(record1);
				free(record2);
				return -1;
			}


			root -> dataType = final;
			if(category == "OP_LOGICAL") root -> dataType = "bool";

			return 0;
		}

		free(record2);
		return 0;
	}

	// unary operators
	assert ((root -> name).compare("-") == 0 || (root -> name).compare("+") == 0 || (root -> name).compare("not") == 0 );

	if((root->name).compare("not"))
	{
		final = isCompatible("int", left -> dataType);
		if (final.length() == 0)
		{
			raise_error(ERR::OP_OVERLOAD, root);
			print_note(NOTE::PREV_DECL, record1);
			free(record1);
			return -1;
		}
		root -> dataType = final;
		return 0;
	}

	// handle not
	final = isCompatible("bool", left -> dataType);
	if (final.length() == 0)
	{
		raise_error(ERR::OP_OVERLOAD, root);
		print_note(NOTE::PREV_DECL, record1);
		free(record1);
		return -1;
	}
	root -> dataType = final;

	free(record1);


	return 0;

}

string handle_list_access(TreeNode* root)
{
	assert(root -> name == "list_access");
	assert(root -> type == "NON_TERMINAL");
	assert((root -> children).size() == 4);

	if((root->dataType).compare(0, 4, "list") == 0)
		return root->dataType;

	string final = isCompatible((root->children)[2]->dataType, "bool");
	if (final.length() == 0)
	{
		raise_error(ERR::EXPECTED_INT, (root->children)[2]);
		return "";
	}
	string type = (root->children)[0] -> dataType;
	if (type.compare(0, 4, "list") == 0)
		root -> dataType = type.substr(5, type.length() - 6);
	else
	{
		raise_error(ERR::BAD_LIST, (root->children)[0]);
		return "";
	}
	
	return type;
}

string handle_function_call(TreeNode* root)
{
	assert(root -> name == "function_call");
	assert(root -> type == "NON_TERMINAL");

	int nparams = ((root -> children)[2]->children).size();
	vector<tableRecord*> params;

	if(fun_call == 1)
	{
		TreeNode* node = (root -> children)[0];
		tableRecord* record = new tableRecord("self", self_type);
		params.push_back(record);
		fun_call = 0;
		self_type = "";
	}


	for(int i = 0; i < nparams; i++)
	{
		TreeNode* node = ((root -> children)[2]->children)[i];
		tableRecord* record = new tableRecord(node -> dataType, node -> dataType);
		if(record->name == "self" && record -> recordType == recordType::CLASS_SELF)
		{
			self_ind = i;
			continue;
		}
		params.push_back(record);
	}

	tableRecord* funcEntry = currTable -> lookup((root -> children)[0] -> name, recordType::TYPE_FUNCTION, &params);
	if (tempDotTable) funcEntry = tempDotTable -> lookup_table((root -> children)[0] -> name, recordType::TYPE_FUNCTION, &params);
	tempDotTable = NULL;

	for (auto &i: params)
		free(i);
		
	if (!funcEntry)
	{
		raise_error(ERR::FUNC_OVERLOAD, (root -> children)[0]);
		return "";
	}

	

	root -> dataType = funcEntry -> type;
	return root -> dataType;

}

// root is -> here
int handle_to(TreeNode* root)
{
	assert(root -> name == "->");
	assert ((root -> children).size() == 1);
	
	string type = validateType((root -> children)[0]);
	if (!type.length())
		return -1;
	
	return 0;

}

int handle_identifier(TreeNode* root)
{
	tableRecord* entry = currTable -> lookup(root -> name);
	if (funcInClass)
	{
		if (tempDotTable) entry = tempDotTable -> lookup_table(root -> name);
		funcInClass = 0;
	}

	if (!entry)
	{
		raise_error(ERR::UNDECLARED, root);
		return -1;
	}

	root -> dataType = entry -> type;
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

	if ((root -> type).compare("NON_TERMINAL") == 0 && ((root -> name).compare("for_stmt") == 0 || (root -> name).compare("while_stmt") == 0))
	{
		in_loop.back()++;
	}

	if ((root -> name ).compare(".") == 0)
	{
		int ret = pre_handle_dot(root);
	}


	if ((root -> type).compare("DELIMITER") == 0 && (root -> name).compare(":") == 0)
	{
		int ret = handle_type_declarations(root);
		return ret;
	}

	// dfs //
	vector<TreeNode *> &children = root -> children;
	for (int nchild = 0; nchild < (root -> children).size(); nchild++)
	{
		children[nchild] -> scope = currTable;
		int ret = generate_symtable(children[nchild]);
		if (ret < 0)
			return ret;
	}

	if ((root -> type).compare("NON_TERMINAL") == 0 && ((root -> name).compare("for_stmt") == 0 || (root -> name).compare("while_stmt") == 0))
	{
		if (in_loop.back() == 0)
		{
			raise_error(ERR::IMPOSSIBLE, root);
			assert(false);
			return -1;
		}
		in_loop.back()--;
	}

	if ((root -> type).compare("KEYWORD") == 0 && ((root -> name).compare("break") == 0 || (root -> name).compare("continue") == 0))
	{
		if (in_loop.back() == 0)
		{
			raise_error(ERR::LOOP, root);
			return -1;
		}
	}

	if ((root -> type).compare("STRING_LITERAL") == 0)
	{
		int ret = handle_const_strings(root);
		return ret;
	}
	
	if ((root -> type).compare("INT_LITERAL") == 0)
	{
		int ret = handle_const_int(root);
		return ret;
	}
	
	if ((root -> type).compare("FLOAT_LITERAL") == 0)
	{
		int ret = handle_const_float(root);
		return ret;
	}

	if ((root -> type).compare("KEYWORD") == 0 && ((root -> name).compare("True") == 0 || (root -> name).compare("False") == 0))
	{
		int ret = handle_const_bool(root);
		return ret;
	}

	if ((root -> name ).compare(".") == 0)
	{
		int ret = post_handle_dot(root);
		return ret;
	}

	if (root->type == "NON_TERMINAL" && ((root->name) == "while_stmt" || (root->name) == "elif_stmt" || (root->name) == "if_stmt"))
	{
		assert ((root ->children).size() >= 1);
		int ret = handle_bool_expressions((root -> children)[0]);
		return ret;
	}

	if (root->type == "NON_TERMINAL" && (root->name).compare("group") == 0)
	{
		assert((root ->children).size() == 3);
		root -> dataType = ((root -> children)[1])->dataType;
		return 0;
	}

	if (root->type == "NON_TERMINAL" && (root->name).compare("return_stmt") == 0)
	{
		int size = (root ->children).size();
		assert(size <= 1);
		assert (currTable -> parentSymtable);
		tableRecord* record1 = currTable -> parentSymtable->lookup(currTable -> name);
		assert(record1 && record1 -> recordType == recordType::TYPE_FUNCTION);
		string type1 = record1 -> type;
		if (size == 0)
		{
			if (type1.length() == 0 || type1 == "None")
				return 0;
			
			raise_error(ERR::RETURN_TYPE_MISMATCH, root);
			return -1;
		}

		string type2 = (root->children)[0]->dataType;
		string final = isCompatible(type1, type2);
		if(!final.length())
		{
			raise_error(ERR::RETURN_TYPE_MISMATCH, root);
			return -1;
		}

		return 0;
	}


	if (root->type == "NON_TERMINAL" && (root->name).compare("list_access") == 0)
	{
		string ret = handle_list_access(root);
		if (!ret.length())
			return -1;
		return 0;
	}

	if (root->type == "NON_TERMINAL" && (root->name).compare("function_call") == 0)
	{
		string ret = handle_function_call(root);
		if (!ret.length())
			return -1;
		return 0;
	}

	if((root -> name).compare("in") == 0)
	{
		int ret = handle_in(root);
		return ret;
	}

	if (isOperator(root))
	{
		int ret = handle_operators(root);
		return ret;
	}

	if (root -> type == "IDENTIFIER")
	{
		int ret = handle_identifier(root);
		return ret;
	}

	if (root -> name == "list" && root->type == "NON_TERMINAL")
	{
		int ret = handle_list(root);
		return ret;
	}

	if (root -> name == "typedecl" && root->type == "NON_TERMINAL")
	{
		root -> dataType = (root ->children)[0] -> dataType; 
		return 0;
	}

	if (root -> name == "for_expr" && root->type == "NON_TERMINAL")
	{
		root -> dataType = (root ->children)[0] -> dataType; 
		return 0;
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


		for (int i = 0; i < entry->symTab->numParams; i++)
		{
			entry -> size -= (entry->symTab->entries)[i]->size;
		}

		// setting the class constructor function
		if(entry -> name == "__init__")
		{
			string name = entry -> symTab -> parentSymtable -> name;
			tableRecord* record = new tableRecord(name, name, entry -> size, entry -> lineno, entry -> column, recordType::CLASS_CONSTRUCTOR);
			int ret = globTable -> insert(record, entry -> symTab);
			if (ret < 0)
				return -1;
		}
		
		currTable = currTable -> parentSymtable;
		if (in_loop.back() > 0)
		{
			raise_error(ERR::IMPOSSIBLE, root);
			assert (false);
		}
		in_loop.pop_back();
		if (in_function == 0)
		{
			raise_error(ERR::IMPOSSIBLE, root);
			assert (false);
		}
		in_function--;
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
	return generate_symtable(root);
}
