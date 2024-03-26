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

void recordTypeInit()
{
	recordTypeMap[recordType::TYPE_FUNCTION] = "TYPE_FUNCTION";
	recordTypeMap[recordType::TYPE_CLASS] = "TYPE_CLASS";
	recordTypeMap[recordType::CONST_INT] = "CONST_INT";
	recordTypeMap[recordType::CONST_FLOAT] = "CONST_FLOAT";
	recordTypeMap[recordType::CONST_STRING] = "CONST_STRING";
	recordTypeMap[recordType::CLASS_ATTRIBUTE] = "CLASS_ATTRIBUTE";
	recordTypeMap[recordType::OBJECT_ATTRIBUTE] = "OBJECT_ATTRIBUTE";
	recordTypeMap[recordType::VARIABLE] = "VARIABLE";
	
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
			if (entries[i] -> recordType != TYPE_CLASS && entries[i] -> recordType != TYPE_FUNCTION)
			{
				return entries[i];
			}
		}

		for (auto &i: indices)
		{
			// the type must now be class or function
			if (entries[i] -> recordType == TYPE_FUNCTION && params)
			{
				tableRecord* entry = entries[i];
				symbolTable* table = entry -> symTab;
				if (params -> size() != table -> offset) continue;
		
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
			if (entries[i] -> recordType != TYPE_FUNCTION)
			{
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

	if (recordType != recordType::TYPE_FUNCTION)
	{
		tableRecord* entry = lookup_table(name, recordType);
		if (entry)
		{
			raise_error (ERR::REDIFINITION, inputRecord);
			print_note (NOTE::PREV_DECL, entry);
			return -1;
		}
	}

	else
	{
		assert(funcTable);
		vector<tableRecord*> params;
		for (int i = 0; i < funcTable -> offset; i++)
			params.push_back((funcTable -> entries)[i]);

		tableRecord* entry = lookup_table(name, recordType, &params);
		if(entry)
		{
			if (entry -> recordType == recordType::TYPE_FUNCTION)
			{
				raise_error (ERR::REDIFINITION, inputRecord);
				print_note (NOTE::PREV_DECL, entry);
				return -1;
			}
			raise_error (ERR::TYPE_REDECLARATION, inputRecord);
			print_note (NOTE::PREV_DECL, entry);
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

	// size of the table not updated when function entry
	if (recordType != recordType::TYPE_FUNCTION)
		size += __size;
	
	if (recordType == recordType::CLASS_ATTRIBUTE)
	{
		staticIndices.insert(currentIndex);
		size += SIZE_PTR - __size; 			// only one pointer needs to be stored
	}
	
	currentIndex++;
	return 0;
}

int symbolTable::UpdateRecord(tableRecord* newRecord)
{

	tableRecord* record = NULL;
	assert(newRecord);
	assert(newRecord -> symTab);

	record = lookup(newRecord -> name, newRecord -> recordType, NULL);
	assert(record);

	if((newRecord -> type).length()) record -> type = newRecord -> type;
	if(newRecord -> size) 
	{
		size += newRecord -> size - record -> size;
		record -> size = newRecord -> size;
	}
	record -> lineno = newRecord -> lineno;
	record -> column = newRecord -> column;
	record -> recordType = newRecord -> recordType;

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


void tableRecord::dumpCSV(ofstream &CSV)
{
	CSV << index << ", " << name << ", " << type << ", " << recordTypeMap[recordType] << ", " << size << ", " << lineno << endl;
	return;
}


void symbolTable::dumpCSV(ofstream &CSV)
{
	CSV << "# Table Name: " << name;
	if (parentSymtable)
		CSV << ", Parent Table: " << parentSymtable -> name;
	
	CSV << endl;

	int index = 0;

	if (tableType == tableType::FUNCTION)
	{
		CSV << "\n# Incoming Parameters: \n";
		CSV << "index, name, type, recordType, size, line no. \n";
		for (; index < offset; index++)
			(entries[index]) -> dumpCSV(CSV);
	}

	CSV << "\n# Local Variables: \n";
	CSV << "index, name, type, recordType, size, line no. \n";
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

int generate_symtable(TreeNode *root, tableRecord* &record)
{


	// handle functions
	if ((root -> type).compare("NON_TERMINAL") == 0 && (root -> name).compare("function_def") == 0)
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
				currTable -> offset++;
		}

		TreeNode* node = ((root -> children)[0]);
		string type = "None";
		if (((root -> children)[0] -> name).compare("main") && ((root -> children)[0] -> name).compare("__init__"))
			type = (((root -> children)[4]) -> children)[0] -> name;

		record = new tableRecord(node -> name, type, currTable -> size, node -> lineno, node -> column, recordType::TYPE_FUNCTION);
		
		assert(currTable -> parentSymtable);
		int err = currTable -> parentSymtable -> insert(record, currTable);
		if (err < 0)
			return err;

		free(record);
		record = NULL;

	}


	// handle classes
	if ((root -> type).compare("NON_TERMINAL") == 0 && (root -> name).compare("class_def") == 0)
	{
		TreeNode* node = ((root -> children)[0]);
		tableRecord* tempRecord = new tableRecord(node -> name, node -> name, currTable -> size, node -> lineno, node -> column, recordType::TYPE_CLASS);
		if (currTable != globTable)
		{
			raise_error(ERR::CLASS_NOT_GLOBAL, tempRecord);
			free (tempRecord);
			return -1;
		}
		
		symbolTable* parent = globTable;
		if((root -> children.size() > 2))
		{
			node = (root -> children)[2];
			if((node -> type).compare("IDENTIFIER") == 0)
			{
				tableRecord* entry = currTable -> lookup(node -> name, recordType::TYPE_CLASS);
				tempRecord->name = entry->name;
				tempRecord->lineno = entry->lineno;
				tempRecord->column = entry->column;
				if(entry)
				{
					if (entry -> recordType != recordType::TYPE_CLASS)
					{
						raise_error(ERR::ILL_PARENT, tempRecord);
						free (tempRecord);
						return -1;
					}	
				}

				assert(entry -> symTab);
				parent = entry -> symTab;
				free (tempRecord);
				tempRecord = NULL;
			}
		}

		symbolTable *Table = new symbolTable(((root -> children)[0]) -> name, parent);
		Table -> tableType = tableType::CLASS;
		currTable = Table;

		node = ((root -> children)[0]);
		record = new tableRecord(node -> name, node -> name, currTable -> size, node -> lineno, node -> column, recordType::TYPE_CLASS);
		
		int err = globTable -> insert(record, currTable);
		if (err < 0)
			return err;

		free(record);
		record = NULL;

	}

	// apply dfs here
	vector<TreeNode *> &children = root -> children;
	for (int nchild = 0; nchild < (root -> children).size(); nchild++)
	{
		int ret = generate_symtable(children[nchild], record);

		if (ret < 0)
			return ret;
	}


	// constant integers
	if ((root -> type).compare("INT_LITERAL") == 0)
	{
		tableRecord* tempRecord = record;
		record = new tableRecord(root -> name, "int", SIZE_INT, root -> lineno, root -> column, recordType::CONST_INT);
		if(!globTable -> lookup(record -> name, recordType::CONST_INT))
			globTable -> insert(record, NULL);
		free (record);
		record = tempRecord;
		return 0;
	}



	// constant floating point literals
	if ((root -> type).compare("FLOAT_LITERAL") == 0)
	{
		tableRecord* tempRecord = record;
		record = new tableRecord(root -> name, "float", SIZE_FLOAT, root -> lineno, root -> column, recordType::CONST_FLOAT);
		if(!globTable -> lookup(record -> name, recordType::CONST_FLOAT))
			globTable -> insert(record, NULL);
		free (record);
		record = NULL;
		record = tempRecord;
		return 0;
	}



	// constant string literals
	if ((root -> type).compare("STRING_LITERAL") == 0)
	{
		tableRecord* tempRecord = record;
		string str_type = "str";
		formatString(root -> name, str_type);
		record = new tableRecord(root -> name, str_type, SIZE_STRING((root -> name).length()), root -> lineno, root -> column, recordType::CONST_STRING);
		if(!globTable -> lookup(record -> name, recordType::CONST_STRING))
			globTable -> insert(record, NULL);
		free (record);
		record = NULL;
		record = tempRecord;
		return 0;
	}



	// handle type declarations
	if ((root -> type).compare("DELIMITER") == 0 && (root -> name).compare(":") == 0)
	{
		// colon will have exactly 2 chidren
		assert((root -> children).size() == 2);
		TreeNode* node = ((root -> children)[0]);
		symbolTable* tempTable = currTable;
		int recordType = recordType::VARIABLE;
		int size = 0;

		if (tempTable->offset > tempTable->currentIndex)
		{
			// function parameter is set here
			node = ((root -> children)[1]);
			if ((node -> name).compare("list") == 0)
			{
				size = SIZE_PTR;
			}
			node = ((root -> children)[0]);
		}


		if((node -> name).compare(".") == 0)
		{
			assert((node -> children).size() == 2);
			tableRecord* tempRecord = new tableRecord((node -> children)[0] -> name, "", 0, (node -> children)[0] -> lineno,  (node -> children)[0] -> column);

			if(((node -> children)[0] -> name).compare("self"))
			{
				raise_error(ERR::CLASS_ATTRIBUTE_DECL_SELF, tempRecord);
				return -1;
			}

			if((currTable -> name).compare("__init__"))
			{
				raise_error(ERR::CLASS_ATTRIBUTE_DECL_CTOR, tempRecord);
				return -1;
			}

			if(!(currTable -> parentSymtable) || (currTable -> parentSymtable) -> tableType != tableType::CLASS)
			{
				raise_error(ERR::CLASS_CTOR, tempRecord);
				return -1;
			}

			// declaration inside ctor of a class
			tempTable = currTable -> parentSymtable;
			node = (node -> children)[1];
			recordType = recordType::OBJECT_ATTRIBUTE;
		}

		else if(tempTable -> tableType == tableType::CLASS)
		{
			recordType = recordType::CLASS_ATTRIBUTE;
		}
		
		record = new tableRecord(node -> name, "", size, node -> lineno, node -> column, recordType);

		node = (root -> children)[1];

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
			record -> type = ((root -> children)[1]) -> name;

			if ((record -> type).compare("int") == 0)
				record -> size = SIZE_INT;

			else if ((record -> type).compare("float") == 0)
				record -> size = SIZE_FLOAT;

			else if ((record -> type).compare("bool") == 0)
				record -> size = SIZE_BOOL;

			else if ((record -> type).compare("str") == 0)
				record -> size = SIZE_PTR;

			// dealing with classes
			else if ((record -> type).compare(0, 4, "list"))
			{
				tableRecord *entry = tempTable -> lookup(record -> type, recordType::TYPE_CLASS);
				if (!entry)
				{
					raise_error(ERR::UNKNOWN_TYPE, record);
					return -1;
				}
				record -> size = entry -> size;
				record -> recordType = recordType::CLASS_OBJECT;
			}

		}

		int err = tempTable -> insert(record);
		// cout <<"name = " << record -> name << " type = " << record -> type << " size = " << record -> size << "lineno = " << record -> lineno << endl;

		assert(record->symTab);
		
		if (err < 0)
			return err;

		if (record -> size)
		{
			free(record);
			record = NULL;
		}

		return 0;
	}

	

	// Update the sizes of the records, in case of lists and strings
	if ((root -> type).compare("OPERATOR") == 0 && (root -> name).compare("=") == 0)
	{
		assert((root -> children).size() == 2);
		if (record)
		{
			if (!(record -> size))
			{
				
				// = will have only 2 child
				assert((root -> children).size() == 2);

				TreeNode* node = (root -> children)[1];
				if ((record -> type).compare(0, 4, "list") == 0)
				{
					int num = (node -> children).size() - 2;
					string type = record -> type;

					// remove the characters l i s t [ ] = 6
					string category = type.substr(5, type.length() - 6);

					if (category.compare("int") == 0)
						record -> size = num * SIZE_INT;

					else if (category.compare("float") == 0)
						record -> size = num * SIZE_FLOAT;

					else if (category.compare("bool") == 0)
						record -> size = num * SIZE_BOOL;

					else if (category.compare("str") == 0)
						record -> size = num * SIZE_PTR;
						
					// class objects
					else
					{
						tableRecord *entry = currTable -> lookup(record -> type, recordType::TYPE_CLASS);
						if (!entry)
						{
							raise_error(ERR::UNDECLARED, record);
							return -1;
						}
						record -> size = num * entry -> size;
					}
					
				}


				assert (record -> symTab);
				int ret = currTable -> UpdateRecord(record);

				if (ret < 0)
					return ret;

			}

			free(record);
			record = NULL;
		}

		else
		{
			// assignment in ctor
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
							entry = currTable -> parentSymtable -> lookup(node -> name);
							if (!entry)
							{
								raise_error(ERR::CLASS_NO_MATCH_ATTR, record);
								return -1;
							}

							// since the above lookup returned true
							assert (currTable -> parentSymtable -> parentSymtable);
							if (entry -> symTab -> tableType != tableType::CLASS)
							{
								tableRecord* tempRecord = new tableRecord((node -> children)[1] -> name, "", 0, (node -> children)[1] -> lineno, (node -> children)[1] -> column);
								raise_error(ERR::CLASS_NO_MATCH_ATTR, tempRecord);
								return -1;
							}

							int err = currTable -> parentSymtable -> insert(entry);
							if (err < 0)
								return -1;
						}

						// else do nothing, this is of the same class

					}
				}
			}
		}

		return 0;
	}



	// dealing with functions again, after completing the function symbol table
	if ((root -> type).compare("NON_TERMINAL") == 0 && (root -> name).compare("function_def") == 0)
	{
		assert ((currTable -> name).compare((root -> children)[0] -> name) == 0);
		vector<tableRecord*> params;
		for (int i = 0; i < currTable -> offset; i++)
			params.push_back((currTable -> entries)[i]);
		
		assert(currTable -> parentSymtable);
		tableRecord* entry = currTable -> parentSymtable -> lookup_table((root->children)[0]->name, recordType::TYPE_FUNCTION, &params);
		assert(entry);
		entry -> size = currTable -> size;
		currTable = currTable -> parentSymtable;
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
	}

	return 0;
}

int symTable_Maker(TreeNode *root)
{
	globTable -> tableType = tableType::GLOBAL;
	tableRecord* record = NULL;
	recordTypeInit();
	return generate_symtable(root, record);
}
