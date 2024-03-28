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
	recordTypeMap[recordType::TYPE_FUNCTION]		=		 "TYPE_FUNCTION";
	recordTypeMap[recordType::TYPE_CLASS]		 	=		 "TYPE_CLASS";
	recordTypeMap[recordType::CONST_STRING]		 	=		 "CONST_STRING";
	recordTypeMap[recordType::CONST_INT]		 	=		 "CONST_INT";
	recordTypeMap[recordType::CONST_FLOAT]		 	=		 "CONST_FLOAT";
	recordTypeMap[recordType::CONST_BOOL]		 	=		 "CONST_BOOL";
	recordTypeMap[recordType::CLASS_ATTRIBUTE]		=		 "CLASS_ATTRIBUTE";
	recordTypeMap[recordType::CLASS_OBJECT]		 	=		 "CLASS_OBJECT";
	recordTypeMap[recordType::CLASS_CONSTRUCTOR]	=		 "CLASS_CONSTRUCTOR";
	recordTypeMap[recordType::OBJECT_ATTRIBUTE]		=		 "OBJECT_ATTRIBUTE";
	recordTypeMap[recordType::VARIABLE]		 		=		 "VARIABLE";


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



	tableRecord* tempRecord = new tableRecord("int", "int", SIZE_INT, 0, 0, recordType::TYPE_CLASS);
	globTable -> insert(tempRecord, globTable);
	free(tempRecord);
	tempRecord = NULL;
	tempRecord = new tableRecord("float", "float", SIZE_FLOAT, 0, 0, recordType::TYPE_CLASS);
	globTable -> insert(tempRecord, globTable);
	free(tempRecord);
	tempRecord = NULL;
	tempRecord = new tableRecord("bool", "bool", SIZE_BOOL, 0, 0, recordType::TYPE_CLASS);
	globTable -> insert(tempRecord, globTable);
	free(tempRecord);
	tempRecord = NULL;
	tempRecord = new tableRecord("str", "str", SIZE_PTR, 0, 0, recordType::TYPE_CLASS);
	globTable -> insert(tempRecord, globTable);
	free(tempRecord);
	tempRecord = NULL;
	tempRecord = new tableRecord("list", "list", SIZE_PTR, 0, 0, recordType::TYPE_CLASS);
	globTable -> insert(tempRecord, globTable);
	free(tempRecord);
	tempRecord = NULL;
	
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

				return NULL;
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

				if (entries[i] -> recordType != recordType::TYPE_CLASS)
					return NULL;

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
	
	if (!isValidType(record -> type))
		typeMap[record -> type] = type_offset++;

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

// root is function def here
int handle_function_declaration(TreeNode* root)
{
	// a function definition has 6 children
	if (((root -> children)[0] -> name).compare("main")
					 && ((root -> children)[0] -> name).compare("__init__"))
		assert((root -> children).size() == 6);

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
			last_pos = index;
		}

		index++;
	}

	if (cntSelf > 1)
	{
		raise_error(ERR::DUPLICATE_SELF, (((root -> children)[2]) -> children)[last_pos]);
		return -1;
	}

	currTable = Table;
	currTable -> numParams = numParam;

	TreeNode* node = ((root -> children)[0]);
	string type = "None";
	if (((root -> children)[0] -> name).compare("main") && ((root -> children)[0] -> name).compare("__init__"))
		type = (((root -> children)[4]) -> children)[0] -> name;
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
		record = currTable->lookup("self", recordType::CLASS_SELF);
		assert (record);
		tableRecord* entry = new tableRecord(record->name, record->type, record->size, record->lineno, record->column, record->recordType);
		err = currTable -> insert(entry);
		free(entry);
	}

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
		// find parent first
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
	node -> dataType = node -> name;
	tableRecord* record = new tableRecord(node -> name, node -> name, currTable -> size, node -> lineno, node -> column, recordType::TYPE_CLASS);
	int err = globTable -> insert(record, currTable);
	
	free(record);
	record = NULL;
	if (err < 0)
		return err;

	record = new tableRecord("self", node -> name, SIZE_PTR, node -> lineno, node -> column, recordType::CLASS_SELF);
	err = currTable -> insert(record);

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
void set_record_size(TreeNode* root, tableRecord* record)
{
	assert((root->children).size() == 2);
	TreeNode* node = (root->children)[1];

	// type of list constructed here
	if ((node -> name).compare("list_access") == 0)
	{
		// some explicit type inside list 
		assert((node -> children).size() > 2);

		string type = (node -> children)[0] -> name + "[" + (node -> children)[2] -> name + "]";
		record -> type = type;
		record -> size = SIZE_PTR;
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

	assert ((root -> children)[0]->type == "IDENTIFIER" || (root -> children)[0]->name == ".");

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
	root -> dataType = node -> dataType;

	if (((root -> children)[0]->name).compare(".") == 0)
		(root -> children)[0] -> dataType = node -> dataType;
	
	int err = tempTable -> insert(record);
	assert(record->symTab);
	
	free(record);
	record = NULL;

	if (err < 0)
		return err;

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
	
	if ((root -> type).compare("INT_LITERAL") == 0)
	{
		int ret = handle_const_int(root);
		if (ret < 0)
			return ret;
		return 0;
	}
	
	if ((root -> type).compare("FLOAT_LITERAL") == 0)
	{
		int ret = handle_const_float(root);
		if (ret < 0)
			return ret;
		return 0;
	}

	if ((root -> type).compare("KEYWORD") == 0 && ((root -> name).compare("True") == 0 || (root -> name).compare("False") == 0))
	{
		int ret = handle_const_bool(root);
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


	if ((root -> name ).compare(".") == 0)
	{
		// assert((root->children).size() == 2);
		// bool retVal = checkDeclaration((root->children)[0], recordType::CLASS_OBJECT);
		// if (!retVal)
		// 	return -1;

		// tableRecord* entry = currTable->lookup((root->children)[0] -> name, recordType::CLASS_OBJECT);
		// assert(entry);

		// tableRecord* classRecord = globTable->lookup_table(entry->type, recordType::TYPE_CLASS);
		// assert(classRecord);

		// entry = (classRecord->symTab)->lookup_table((root->children)[1]->name);
		// if(!entry)
		// {
		// 	raise_error(ERR::CLASS_NO_MATCH_ATTR, (root->children)[1]);
		// 	return -1;
		// }

		// root -> dataType = (root->children)[1]->dataType;
		// return 0;
	}


	if (root->type == "NONTERMINAL" && (root->name).compare("list_access"))
	{
		string ret = handle_list_access(root);
		if (!ret.length())
			return -1;
		return 0;
	}

	if (root->type == "NONTERMINAL" && (root->name).compare("function_call"))
	{
		string ret = handle_function_call(root);
		if (!ret.length())
			return -1;
		return 0;
	}

		

	if((root -> name).compare("in") == 0)
	{
		assert ((root -> children).size() == 2);
		TreeNode* left = (root -> children)[0];
		TreeNode* right = (root -> children)[1];
		assert (left -> type == "IDENTIFIER" || left -> name == "." || left -> name == "list_access");
		assert (right -> type == "IDENTIFIER" || right -> name == "." || right -> name == "function_call");

		tableRecord* entry1 = currTable -> lookup(left -> name);
		assert(entry1);
		tableRecord* entry2 = currTable -> lookup(right -> name);
		assert(entry2);

		if ((entry2->type).compare(0, 4, "list") && (entry2->type).compare("string"))
		{
			raise_error(ERR::TYPE_MISMATCH, root);
			print_note(NOTE::PREV_DECL, entry1);
			print_note(NOTE::PREV_DECL, entry2);
			return -1;
		}
		else if ((entry2->type).compare(0, 4, "list") == 0)
		{
			if ((entry2 -> type).compare(5, (entry1 ->type).length(), entry1 ->type))
			{
				raise_error(ERR::TYPE_MISMATCH, root);
				print_note(NOTE::PREV_DECL, entry1);
				print_note(NOTE::PREV_DECL, entry2);
				return -1;
			}
		}
		else
		{
			if (entry1 -> type != entry2->type)
			{
				raise_error(ERR::TYPE_MISMATCH, root);
				print_note(NOTE::PREV_DECL, entry1);
				print_note(NOTE::PREV_DECL, entry2);
				return -1;
			}
		}

		return 0;

	}


	if (isOperator(root))
	{
		int ret = handle_operators(root);
		if (ret < 0)
			return ret;

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

	return "";
}

bool checkDeclaration(TreeNode* root, int recordType, vector<tableRecord*> *params)
{
	if (root -> type != "IDENTIFIER")
		return true;

	tableRecord* entry = currTable->lookup(root->name, recordType, params);
	if (!entry)
	{
		raise_error(ERR::UNDECLARED, root);
		return false;
	}
	return true;
}

int handle_operators(TreeNode* root)
{
	assert((root->children).size() > 0);
	string category = root->dataType;
	TreeNode* left = (root -> children)[0];
	bool returnVal = checkDeclaration(left);
	if(!returnVal) 
		return -1;
	tableRecord* record1 = new tableRecord(left->dataType, left->dataType, left->lineno, left->column);
	string type1 = left->dataType;
	string final = type1; 
	
	assert((root -> type).compare(0, 2, "OP") == 0 );
	assert((root->children).size() == 2 || (root->children).size() == 1);

	if ((root -> children).size() == 2)
	{
		TreeNode* right = (root -> children)[1];
		returnVal = checkDeclaration(right);
		if (!returnVal)
			return -1;

		string type2 = right->dataType;
		tableRecord* record2 = new tableRecord(right->dataType, right->dataType, right->lineno, right->column);

		final = isCompatible(type1, type2);
		if (final.length() == 0)
		{
			raise_error(ERR::TYPE_MISMATCH, root);
			print_note(NOTE::PREV_DECL, record1);
			print_note(NOTE::PREV_DECL, record2);
			free(record1);
			free(record2);
			return -1;
		}

		if (category == "OP_ASSIGNMENT")
		{
			root -> dataType = final;
			return 0;
		}

		if (category == "OP_RELATIONAL")
		{

			if (final.compare("str") && final.compare("bool") && final.compare("int") && final.compare("float"))
			{
				raise_error(ERR::OVERLOAD, root);
				print_note(NOTE::PREV_DECL, record1);
				print_note(NOTE::PREV_DECL, record2);
				free(record1);
				free(record2);
				return -1;
			}

			root -> dataType = final;
			return 0;
		}

		if (category == "OP_ARITHMETIC")
		{
			if (final.compare("int") && final.compare("float") && final.compare("bool"))
			{
				raise_error(ERR::OVERLOAD, root);
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
				raise_error(ERR::OVERLOAD, root);
				print_note(NOTE::PREV_DECL, record1);
				print_note(NOTE::PREV_DECL, record2);
				free(record1);
				free(record2);
				return -1;
			}

			root -> dataType = final;
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
			raise_error(ERR::OVERLOAD, root);
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
		raise_error(ERR::OVERLOAD, root);
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
	assert(root -> type == "NONTERMINAL");
	assert((root -> children).size() == 4);

	tableRecord* entry = currTable -> lookup((root->children)[0]->name);
	if (!entry)
	{
		raise_error(ERR::UNDECLARED, (root->children)[0]);
		return "";
	}

	assert((entry->type).compare(0, 4, "list") == 0);

	string final = isCompatible((root->children)[2]->dataType, "bool");
	if (final.length() == 0)
	{
		raise_error(ERR::INTEGER_EXECTED, (root->children)[1]);
		return "";
	}
	root -> dataType = "int";
	return (entry -> type).substr(5, (entry -> type).length() - 6);
}

string handle_function_call(TreeNode* root)
{
	assert(root -> name == "function_call");
	assert(root -> type == "NONTERMINAL");

	int nparams = (root -> children).size() - 2;
	vector<tableRecord*> params;
	for(int i = 1; i < nparams - 1; i++)
	{
		// string name;
		// tableRecord* entry = currTable -> lookup();
		// tableRecord* record = new tableRecord("", )

		// if ()

	}


}

string findTypeSingle(TreeNode* root)
{
	assert(root -> type == "IDENTIFIER");

	tableRecord* entry = currTable -> lookup(root->name);
	if (!entry)
	{
		raise_error(ERR::UNDECLARED, (root->children)[0]);
		return "";
	}

	return entry -> type;
}

int handleSelf(TreeNode* root)
{
	assert(root -> name == ".");
	assert((root -> children)[0] -> name  == "self");
	tableRecord* selfEntry = currTable -> lookup("self");
	if (! selfEntry)
	{
		raise_error(ERR::UNDECLARED, (root -> children)[0]);
		return -1;
	}

	if (selfEntry -> recordType == recordType::CLASS_SELF)
	{
		symTable* classTable = selfEntry -> symTab;
		assert (classTable -> tableType == tableType::CLASS);

		TreeNode* node = (root -> children)[1];

		/* 3 possible cases 
		
		1. self.name
		2. self.func()
		3. self.list[index]

		*/

		/* case 1 */
		if (node -> type == "IDENTIFIER")
		{	
			tableRecord* entry = classTable -> lookup((root -> children)[0] -> name);
			if (!entry)
			{
				raise_error(ERR::CLASS_NO_MATCH_ATTR, node);
				return -1;
			}
			node -> dataType = entry -> type;
			return 0;
		}


		/* change symtable for functions called to be operable */
		symTable* tempTable = currTable;
		currTable = classTable;

		if (node -> type == "NONTERMINAL" && node -> name == "list_access")
		{
			string type = handle_list_access(node);
			if (type.length() == 0)
				return -1;
			node -> dataType = type;
			return 0;
		}

		if (node -> type == "NONTERMINAL" && node -> name == "function_call")
		{
			string type = handle_function_call(node);
			if (type.length() == 0)
				return -1;
			node -> dataType = type;
			return 0;
		}

		/* reset the symtable */
		currTable = tempTable;

	}

	// else
}

string typeFinder(TreeNode* root)
{

	/* possible cases

	1. name,
	2. name[index]

	3. name[index].name
	4. name[index].func()
	5. name.func()
	6. name.name
	7. name.name[index]

	if name could be self

	1,5,6,7 hold

	*/

	if (root -> name == ".")
	{
		TreeNode* left = (root -> children)[0];
		TreeNode* right = (root -> children)[1];
		tableRecord* classEntry;
		int checkSum = 0;

		if (left -> type == "NONTERMINAL" && left -> name == "list_access")
		{
			string type = handle_list_access(left);
			if (type.length() == 0)
				return type;

			assert (isValidType(type));

			// type must correspond to a class since we are in dot
			classEntry = globTable -> lookup_table(type, recordType::TYPE_CLASS);
			assert (classEntry);
			checkSum++;
		}

		else if (left -> type == "IDENTIFIER")
		{
			findTypeSingle(root);
			checkSum++;
		}
		assert(checkSum == 1);
	}

	else
		return findTypeSingle(root);

	return "";
}
