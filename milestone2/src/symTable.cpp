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

tableRecord::symRecord(string __name, string __type, int __size, int __lineno, int __column, symbolTable* __symTab, bool __isStatic)
{
	name = __name;
	type = __type;
	size = __size;
	lineno = __lineno;
	column = __column;
	symTab = __symTab;
	isStatic = __isStatic;
}


symbolTable::symTable(string __name, symbolTable* __parentSymtable)
{
	name = __name;
	parentSymtable = __parentSymtable;
}

tableRecord* symbolTable::lookup(string name, int line_no, int column, bool err)
{
	vector<int>indices = name_to_indices[name];
	tableRecord* record = NULL;
	if(indices.size())
	{
		assert(indices.size() == 1);
		record = entries[indices[0]];
		return record;
	}

	if(!parentSymtable) 
	{
		if (err)
			printErrorMsg(line_no, column, RED, " no previous definition found for \'", name, RESET);
		return NULL;
	}

	record = parentSymtable->lookup(name, line_no, column, err);
	return record;

}

// error must be explicitly mentioned outside
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
			printErrorMsg(line_no, column, RED, " no previous definition found for \'", name, RESET);
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
	bool isStatic = inputRecord->isStatic;

	vector<int> indices = name_to_indices[name];

	if (!funcTable)
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

	tableRecord* record = new tableRecord(name, type, __size, lineno, column, funcTable, isStatic);
	
	name_to_indices[name].push_back(currentIndex);
	record->index = currentIndex;
	entries[currentIndex] = record;

	// size of the table not updated when function or class entry
	if (!funcTable)
		size += __size;
	else
		childIndices.push_back(currentIndex);

	if (isStatic)
		staticIndices.insert(currentIndex);
	
	currentIndex++;

	return 0;
}

int symbolTable::UpdateRecord(tableRecord* newRecord)
{
	vector<int>indices = name_to_indices[newRecord->name];
	if(indices.size() == 0)
	{
		printErrorMsg(newRecord->lineno, newRecord->column, RED, "Use of undeclared variable \'", newRecord->name, "\'", RESET);
		return -1;
	}

	assert(indices.size() == 1);
	int index = indices[0];
	tableRecord* record = entries[index];

	if((newRecord->type).length()) record->type = newRecord->type;
	if(newRecord->size) 
	{
		size += newRecord->size - record->size;
		record->size = newRecord->size;
	}
	record->lineno = newRecord->lineno;
	record->column = newRecord->column;

	return 0;	
}

void formatString(string &name);

void tableRecord::dumpCSV(ofstream &CSV)
{
	CSV << index << ", ";
	
	if (type.compare("str") == 0)
	{
		for (char c : name)
		{
			if (c == '\n') {
				CSV << "\r\n";
			}
			else if (c == '\\')
			{
				CSV << '\\' << '\\';
			}
			else
			{
				CSV << c;
			}
		}
	}
	else
		CSV << name;
	
	CSV << ", " << type << ", " << size << ", " << lineno << endl;
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
		CSV << "\n\n\n\n\n";
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
		currTable->offset = (((root->children)[2])->children).size();
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
		record = new tableRecord(root->name, "int", SIZE_INT, root->lineno, root->column);
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
		record = new tableRecord(root->name, "float", SIZE_FLOAT, root->lineno, root->column);
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
		// if (!table_flag)
		// 	formatString(root->name);
		record = new tableRecord(root->name, "str", SIZE_STRING((root->name).length()), root->lineno, root->column);
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

			if((currTable->parentSymtable)->tableType != tableType::CLASS)
			{
				printErrorMsg((node->children)[0]->lineno, (node->children)[0]->column, RED, "constructor function \"__init__\" can only be defined inside a class", RESET);
				return -1;
			}

			tempTable = currTable->parentSymtable;
			node = (node->children)[1];
		}

		else if(currTable->tableType == tableType::CLASS)
		{
			isStatic = true;
		}
		
		record = new tableRecord(node->name, "", 0, node->lineno, node->column, NULL, isStatic);

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

			// dealing with classes
			else if ((record->type).compare("str") && (record->type).compare(0, 4, "list"))
			{
				tableRecord *entry = currTable->lookup(record->type, record->lineno, record->column, true);
				if (!entry)
				{
					return -1;
				}
				record->size = entry->size;
			}

		}

		int err = currTable->insert(record, NULL);
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
					{
						for(auto &c: node->children)
						{
							record->size += SIZE_STRING((c->name).length());
						}
					}
						
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

				else if ((record->type).compare("str") == 0)
					record->size = SIZE_STRING((node->name).length());

				int ret = currTable->UpdateRecord(record);
				if (ret < 0)
					return ret;

			}

			free(record);
			record = NULL;
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

		record = new tableRecord(node->name, type, currTable->size, node->lineno, node->column);

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

		record = new tableRecord(node->name, node->name, currTable->size, node->lineno, node->column);

		int err = globTable->insert(record, currTable);
		if (err < 0)
			return err;

		currTable = globTable;

		free(record);
		record = NULL;
	}

	return 0;
}

void symTable_Maker(TreeNode *root)
{
	globTable->tableType = tableType::GLOBAL;
	tableRecord* record = NULL;
	generate_symtable(root, record);
}
