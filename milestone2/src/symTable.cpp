#include "include/symtable.hpp"
#include "include/error.hpp"

extern int table_flag;

#define SIZE_INT 4
#define SIZE_PTR 8
#define SIZE_FLOAT 8
#define SIZE_BOOL 1
#define SIZE_STRING(x) (1 + (x))
#define SIZE_LIST(x) (8 * ((x)-2))

extern TreeNode *root;
symbolTable *currTable = new symbolTable("__GLOBAL__", NULL);
symbolTable *globTable = currTable;

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

// lookonly inside the table, donot go one level up
tableRecord* symbolTable::lookup_table(string name)
{
	vector<int>indices = name_to_indices[name];
	tableRecord* record = NULL;
	if(indices.size())
	{
		assert(indices.size() == 1);
		record = entries[indices[0]];
		return record;
	}

	return NULL;
}

// for variables
tableRecord* symbolTable::lookup(string name, int line_no, int column, bool err)
{
	tableRecord* record = lookup_table(name);
	if (record)
		return record;

	if(!parentSymtable) 
	{
		if (err)
			printErrorMsg(line_no, column, RED, " no previous definition found for \'", name, "\'", RESET);

		return NULL;
	}

	record = parentSymtable->lookup(name, line_no, column, err);
	return record;
}

// for functions
tableRecord* symbolTable::lookup(string name, vector<tableRecord*> &params, int line_no, int column, bool err)
{
	vector<int>indices = name_to_indices[name];
	tableRecord* record = NULL;
	if (indices.size())
	{
		for (auto idx: indices)
		{
			tableRecord* entry = entries[idx];
			symbolTable* table = entry->symTab;
			if (params.size() != table->offset) continue;
			
			int num;
			for (num = 0; num < params.size(); num++)
			{
				if (params[num]->type != ((table->entries)[num])->type)
					break;
			}
			if (num == params.size()) return entry;
		}
	}

	if(!parentSymtable) 
	{
		if (err)
			printErrorMsg(line_no, column, RED, " no previous definition found for \'", name, "\'", RESET);
		return NULL;
	}

	record = parentSymtable->lookup(name, params, line_no, column, err);
	return record;	
}

int symbolTable::insert(tableRecord* inputRecord, symbolTable* funcTable)
{
	string name = inputRecord->name;
	string type = inputRecord->type;
	int lineno = inputRecord->lineno;
	int column = inputRecord->column;
	int __size = inputRecord->size;
	int recordType = inputRecord->recordType;

	vector<int> indices = name_to_indices[name];

	if (recordType != recordType::TYPE_FUNCTION)
	{
		assert(indices.size() == 0 || indices.size() == 1);
		if (indices.size())
		{
			tableRecord* entry = entries[0];
			printErrorMsg(lineno, column, RED, "redefintion of \'", name, "\'", RESET);
			printErrorMsg(entry->lineno, entry->column, BLUE, " previous definition of \'", name, "\' with type ", entry->type, RESET);
			return -1;
		}
		
	}
	
	else if (indices.size())
	{
		for (auto idx: indices)
		{
			tableRecord* entry = entries[idx];
			symbolTable* table = entry->symTab;
			if(!table) 
			{
				printErrorMsg(lineno, column, RED, name, " redeclared as different kind of symbol", RESET);
				printErrorMsg(entry->lineno, entry->column, BLUE, name, " was declared previously as ", entry->type, RESET);

				return -1;
			}
		}

		for (auto idx: indices)
		{
			tableRecord* entry = entries[idx];
			symbolTable* table = entry->symTab;

			assert(funcTable);
			if (funcTable->offset != table->offset) continue;
			
			int num;
			for (num = 0; num < funcTable->offset; num++)
			{
				if (((funcTable->entries)[num])->type != ((table->entries)[num])->type)
					break;
			}

			if (num == funcTable->offset)
			{
				printErrorMsg(lineno, column, RED, "redefintion of \'", name, "\'", RESET);
				printErrorMsg(entry->lineno, entry->column, BLUE, " previous definition of \'", name, "\' with type ", entry->type, RESET);
				return -1;
			}

		}

	}

	tableRecord* record = new tableRecord(name, type, __size, lineno, column, recordType);
	record->symTab = this;
	
	name_to_indices[name].push_back(currentIndex);
	record->index = currentIndex;
	entries[currentIndex] = record;

	// size of the table not updated when function entry
	if (!funcTable)
		size += __size;
	else
		childIndices.push_back(currentIndex);

	// static
	if (recordType == recordType::CLASS_ATTRIBUTE)
	{
		staticIndices.insert(currentIndex);
		// only one pointer needs to be stored
		size += SIZE_PTR - __size;
	}
	
	currentIndex++;

	return 0;
}

int symbolTable::UpdateRecord(tableRecord* newRecord)
{

	tableRecord* record = lookup_table(newRecord->name);
	if(!record)
	{
		printErrorMsg(newRecord->lineno, newRecord->column, RED, "Use of undeclared variable \'", newRecord->name, "\'", RESET);
		return -1;
	}

	if((newRecord->type).length()) record->type = newRecord->type;
	if(newRecord->size) 
	{
		size += newRecord->size - record->size;
		record->size = newRecord->size;
	}
	record->lineno = newRecord->lineno;
	record->column = newRecord->column;
	record->recordType = newRecord->recordType;

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
	CSV << index << ", " << name << ", " << type << ", " << size << ", " << lineno << endl;
	return;
}


void symbolTable::dumpCSV(ofstream &CSV)
{
	CSV << "# Table Name: " << name;
	if (parentSymtable)
		CSV << ", Parent Table: " << parentSymtable->name;
	
	CSV << endl;

	int index = 0;

	if (offset)
	{
		CSV << "\n # Incoming Parameters: \n";
		CSV << "index, name, type, size, line no. \n";
		for (; index < offset; index++)
			(entries[index])->dumpCSV(CSV);
	}

	CSV << "\n # Local Variables: \n";
	CSV << "index, name, type, size, line no. \n";
	for (; index < currentIndex; index++)
		(entries[index])->dumpCSV(CSV);

	for(auto index : childIndices) 
	{
		CSV << "\n\n\n";
		assert(entries[index]->symTab);
		((entries[index])->symTab)->dumpCSV(CSV);
	}

	return;
}

int generate_symtable(TreeNode *root, tableRecord* &record)
{

	// handle functions
	if ((root->type).compare("NON_TERMINAL") == 0 && (root->name).compare("function_def") == 0)
	{

		// a function definition has 6 children
		if (((root->children)[0]->name).compare("main") && ((root->children)[0]->name).compare("__init__"))
			assert((root->children).size() == 6);

		else 
			assert((root->children).size() == 5);

		symbolTable *Table = new symbolTable(((root->children)[0])->name, currTable);
		Table->tableType = tableType::FUNCTION;

		currTable = Table;
		for(auto &itr: ((root->children)[2])->children)
		{
			if ((itr->name).compare("self"))
				currTable->offset++;
		}
	}

	// handle classes
	if ((root->type).compare("NON_TERMINAL") == 0 && (root->name).compare("class_def") == 0)
	{
		
		symbolTable* parent = NULL;
		if((root->children.size() > 2))
		{
			TreeNode* node = (root->children)[2];

			if((node->type).compare("IDENTIFIER") == 0)
			{
				tableRecord* entry = currTable->lookup(node->name, root->lineno, root->column, true);
				if(!entry)
					return -1;

				if(!entry->symTab)
				{
					printErrorMsg(root->lineno, root->column, RED, node->name, " must be declared as a class type", RESET);
					printErrorMsg(entry->lineno, entry->column, BLUE, entry->name, " was declared previously as ", entry->type, RESET);
					return -1;
				}

				parent = entry->symTab;

			}
		}
		symbolTable *Table = new symbolTable(((root->children)[0])->name, parent);
		Table->tableType = tableType::CLASS;

		currTable = Table;

	}

	// apply dfs here
	vector<TreeNode *> &children = root->children;
	for (int nchild = 0; nchild < (root->children).size(); nchild++)
	{
		int ret = generate_symtable(children[nchild], record);

		if (ret < 0)
			return ret;
	}

	// constant integers
	if ((root->type).compare("INT_LITERAL") == 0)
	{
		tableRecord* tempRecord = record;
		record = new tableRecord(root->name, "int", SIZE_INT, root->lineno, root->column, recordType::CONST_INT);
		if(!globTable->lookup(record->name, record->lineno, record->column, false))
			globTable->insert(record, NULL);
		free (record);
		record = tempRecord;
		return 0;
	}

	// constant floating point literals
	if ((root->type).compare("FLOAT_LITERAL") == 0)
	{
		tableRecord* tempRecord = record;
		record = new tableRecord(root->name, "float", SIZE_FLOAT, root->lineno, root->column, recordType::CONST_FLOAT);
		if(!globTable->lookup(record->name, record->lineno, record->column, false))
			globTable->insert(record, NULL);
		free (record);
		record = NULL;
		record = tempRecord;
		return 0;
	}

	// constant string literals
	if ((root->type).compare("STRING_LITERAL") == 0)
	{
		tableRecord* tempRecord = record;
		string str_type = "str";
		formatString(root->name, str_type);
		record = new tableRecord(root->name, str_type, SIZE_STRING((root->name).length()), root->lineno, root->column, recordType::CONST_STRING);
		if(!globTable->lookup(record->name, record->lineno, record->column, false))
			globTable->insert(record, NULL);
		free (record);
		record = NULL;
		record = tempRecord;
		return 0;
	}

	// handle type declarations
	if ((root->type).compare("DELIMITER") == 0 && (root->name).compare(":") == 0)
	{
		// colon will have exactly 2 chidren
		assert((root->children).size() == 2);
		TreeNode* node = ((root->children)[0]);
		symbolTable* tempTable = currTable;
		bool isStatic = false;
		int recordType = recordType::VARIABLE;


		if((node->name).compare(".") == 0)
		{
			assert((node->children).size() == 2);

			if(((node->children)[0]->name).compare("self"))
			{
				printErrorMsg((node->children)[0]->lineno, (node->children)[0]->column, RED, "attribute declaration of class other than self class is forbidden", RESET);
				return -1;
			}

			if((currTable->name).compare("__init__"))
			{
				printErrorMsg((node->children)[0]->lineno, (node->children)[0]->column, RED, "attribute declaration of self class allowed only within constructor", RESET);
				return -1;
			}

			if(!(currTable->parentSymtable) || (currTable->parentSymtable)->tableType != tableType::CLASS)
			{
				printErrorMsg((node->children)[0]->lineno, (node->children)[0]->column, RED, "constructor function \"__init__\" can only be defined inside a class", RESET);
				return -1;
			}

			// declaration inside ctor of a class
			tempTable = currTable->parentSymtable;
			node = (node->children)[1];
			recordType = recordType::OBJECT_ATTRIBUTE;
		}

		else if(tempTable->tableType == tableType::CLASS)
		{
			recordType = recordType::CLASS_ATTRIBUTE;
		}
		
		record = new tableRecord(node->name, "", 0, node->lineno, node->column, recordType);

		node = (root->children)[1];

		// type of list constructed here
		if ((node->name).compare("list") == 0)
		{
			// some explicit type inside list 
			assert((node->children).size() > 2);

			string type = node->name + "[" + (node->children)[1]->name + "]";
			record->type = type;
		}

		else
		{
			record->type = ((root->children)[1])->name;

			if ((record->type).compare("int") == 0)
				record->size = SIZE_INT;

			else if ((record->type).compare("float") == 0)
				record->size = SIZE_FLOAT;

			else if ((record->type).compare("bool") == 0)
				record->size = SIZE_BOOL;

			else if ((record->type).compare("str") == 0)
				record->size = SIZE_PTR;

			// dealing with classes
			else if ((record->type).compare(0, 4, "list"))
			{
				tableRecord *entry = tempTable->lookup(record->type, record->lineno, record->column, true);
				if (!entry)
				{
					return -1;
				}
				record->size = entry->size;
			}

		}

		int err = tempTable->insert(record, NULL);
		// cout <<"name = " << record->name << " type = " << record->type << " size = " << record->size << "lineno = " << record->lineno << endl;
		
		if (err < 0)
			return err;

		if (record->size)
		{
			free(record);
			record = NULL;
		}

		return 0;
	}

	// Update the sizes of the records, in case of lists and strings
	if ((root->type).compare("OPERATOR") == 0 && (root->name).compare("=") == 0)
	{
		assert((root->children).size() == 2);
		if (record)
		{
			if (!(record->size))
			{
				
				// = will have only 2 child
				assert((root->children).size() == 2);

				TreeNode* node = (root->children)[1];
				if ((record->type).compare(0, 4, "list") == 0)
				{
					int num = (node->children).size() - 2;
					string type = record->type;

					// remove the characters l i s t [ ] = 6
					string category = type.substr(5, type.length() - 6);

					if (category.compare("int") == 0)
						record->size = num * SIZE_INT;

					else if (category.compare("float") == 0)
						record->size = num * SIZE_FLOAT;

					else if (category.compare("bool") == 0)
						record->size = num * SIZE_BOOL;

					else if (category.compare("str") == 0)
						record->size = num * SIZE_PTR;
						
					// class objects
					else
					{
						tableRecord *entry = currTable->lookup(record->type, record->lineno, record->column, true);
						if (!entry)
						{
							return -1;
						}
						record->size = num * entry->size;
					}
					
				}

				int ret = currTable->UpdateRecord(record);
				if (ret < 0)
					return ret;

			}

			free(record);
			record = NULL;
		}

		else
		{
			// assignment in ctor
			if ((currTable->name).compare("__init__") == 0)
			{

				TreeNode* node = (root->children)[0];
				assert(currTable->parentSymtable);
				assert(currTable->parentSymtable->tableType == tableType::CLASS);

				if ((node->name).compare(".") == 0)
				{
					assert((node->children).size() == 2);
					if (((node->children)[0]->name).compare("self") == 0)
					{

						tableRecord* entry = currTable->parentSymtable->lookup_table((node->children)[1]->name);
						if (!entry)
						{
							node = (node->children)[1];
							entry = currTable->parentSymtable->lookup(node->name, node->lineno, node->column, true);
							if (!entry)
								return -1;

							// since the above lookup returned true
							assert (currTable->parentSymtable->parentSymtable);
							if (entry->symTab->tableType != tableType::CLASS)
							{
								printErrorMsg((node->children)[1]->lineno, (node->children)[1]->column, RED, (node->children)[1]->name, " doesnot match with any of the attributes of the class", RESET);
								return -1;
							}

							int err = currTable->parentSymtable->insert(entry);
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
	if ((root->type).compare("NON_TERMINAL") == 0 && (root->name).compare("function_def") == 0)
	{
		TreeNode* node = ((root->children)[0]);
		string type = "None";
		if (((root->children)[0]->name).compare("main") && ((root->children)[0]->name).compare("__init__"))
			type = (((root->children)[4])->children)[0]->name;

		record = new tableRecord(node->name, type, currTable->size, node->lineno, node->column, recordType::TYPE_FUNCTION);
		
		assert(currTable->parentSymtable);
		int err = currTable->parentSymtable->insert(record, currTable);
		if (err < 0)
			return err;

		currTable = currTable->parentSymtable;

		free(record);
		record = NULL;
	}

	// dealing with classes again on coming back
	if ((root->type).compare("NON_TERMINAL") == 0 && (root->name).compare("class_def") == 0)
	{
		TreeNode* node = ((root->children)[0]);

		record = new tableRecord(node->name, node->name, currTable->size, node->lineno, node->column, recordType::TYPE_CLASS);

		int err = globTable->insert(record, currTable);
		if (err < 0)
			return err;

		currTable = globTable;

		free(record);
		record = NULL;
	}

	return 0;
}

int symTable_Maker(TreeNode *root)
{
	globTable->tableType = tableType::GLOBAL;
	tableRecord* record = NULL;
	return generate_symtable(root, record);
}
